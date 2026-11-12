#include <atomic>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

struct Instrument {
    inline static int alive = 0;
    Instrument() { ++alive; }
    ~Instrument() { --alive; }
    Instrument(const Instrument&) = delete;
    Instrument& operator=(const Instrument&) = delete;
};

template<class Derived>
struct StaticQuote {
    int quote() const { return static_cast<const Derived&>(*this).quote_impl(); }
};
struct CashQuote : StaticQuote<CashQuote> {
    int quote_impl() const { return 100; }
};
struct FutureQuote { int quote() const { return 101; } };

// Non-owning type-erased view: the target must outlive every call through this view.
// This explains the mechanism; it is not an implementation of the Proxy library.
class QuoteView {
    const void* target_;
    int (*dispatch_)(const void*);
public:
    template<class T>
    explicit QuoteView(const T& target) : target_(&target), dispatch_([](const void* p) {
        return static_cast<const T*>(p)->quote();
    }) {}
    int quote() const { return dispatch_(target_); }
};

struct VirtualQuote {
    virtual int quote() const = 0;
    virtual ~VirtualQuote() = default;
};
struct BondQuote final : VirtualQuote { int quote() const override { return 102; } };

int main() {
    assert(Instrument::alive == 0);
    {
        auto first = std::make_unique<Instrument>();
        const auto address = first.get();
        auto second = std::move(first);
        assert(!first && second.get() == address && Instrument::alive == 1);
    }
    assert(Instrument::alive == 0);
    std::weak_ptr<Instrument> observer;
    {
        auto owner = std::make_shared<Instrument>();
        observer = owner;
        auto another = owner;
        assert(owner.get() == another.get() && Instrument::alive == 1);
        assert(!observer.expired());
    }
    assert(observer.expired() && Instrument::alive == 0);

    const CashQuote cash;
    const FutureQuote future;
    const std::vector<QuoteView> quotes{QuoteView(cash), QuoteView(future)};
    assert(quotes[0].quote() == 100 && quotes[1].quote() == 101);
    const std::unique_ptr<VirtualQuote> bond = std::make_unique<BondQuote>();
    assert(bond->quote() == 102 && dynamic_cast<BondQuote*>(bond.get()) != nullptr);
    int callback_value = 0;
    const std::function<void(int)> callback = [&](int value) { callback_value = value; };
    callback(cash.quote()); // A callback can execute synchronously.
    assert(callback_value == 100);

    std::vector<int> data;
    data.reserve(8);
    assert(data.empty() && data.capacity() >= 8);
    data.push_back(1);
    auto* stable = data.data();
    data.push_back(2); // Capacity is sufficient, so no reallocation occurs.
    assert(data.data() == stable && data[1] == 2);

    std::atomic<unsigned> count{0};
    int price = 0;
    std::atomic<bool> ready{false};
    unsigned observed_price = 0;
    std::thread producer([&] {
        price = 100;
        ready.store(true, std::memory_order_release);
        for (unsigned i = 0; i < 1000; ++i) count.fetch_add(1, std::memory_order_relaxed);
    });
    std::thread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) std::this_thread::yield();
        observed_price = static_cast<unsigned>(price);
        for (unsigned i = 0; i < 1000; ++i) count.fetch_add(1, std::memory_order_relaxed);
    });
    producer.join();
    consumer.join();
    assert(observed_price == 100 && count.load() == 2000);
    unsigned expected = 1999;
    assert(!count.compare_exchange_strong(expected, 3000));
    assert(expected == 2000 && count.load() == 2000);
    assert(count.compare_exchange_strong(expected, 2001));
    std::cout << "ownership, polymorphism and reserve: OK\n";
    std::cout << "published price: " << observed_price << "; atomic count after CAS: " << count.load() << '\n';
}
