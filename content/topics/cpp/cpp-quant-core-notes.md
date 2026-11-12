---
{
  'schemaVersion': 1,
  'id': 'cpp-quant-core-notes',
  'title': 'C++ Quant Developer 核心知识速记：对象、多态、Cache 与并发',
  'description': '从个人速记串联 RAII、智能指针、移动语义、CRTP、Proxy 与 SBO、容器布局、CPU Cache、MESI、伪共享、atomic、CAS 和内存序，并连接已有专题与练习。',
  'category': 'cpp',
  'areas': ['Modern C++', 'STL / C++ Object Model', 'Template / Compile Time'],
  'tags':
    [
      'cpp-core-notes',
      'raii',
      'move-semantics',
      'crtp',
      'proxy',
      'sbo',
      'cpu-cache',
      'false-sharing',
      'atomic',
      'memory-order',
    ],
  'difficulty': 'L1',
  'roles': ['C++ Developer', 'Quant Developer', 'Low-Latency C++ Developer'],
  'companyTypes': ['高频交易', '量化私募', 'Trading Firm'],
  'status': 'published',
  'updated': '2026-11-12',
  'reviewed': '2026-11-12',
  'standard': 'C++20',
  'estimatedMinutes': 65,
  'prerequisites': ['基本 C++ 类、指针与模板语法', '进程、线程与共享内存的基本概念'],
  'related':
    [
      'raii-exception-safety',
      'move-value-categories',
      'object-lifetime-layout',
      'vector-invalidation',
      'vtable-devirtualization',
      'cpp-memory-model',
      'aba-reclamation',
      'cpu-cache-false-sharing',
      'benchmark-methodology',
    ],
  'demo':
    {
      'file': 'examples/cpp-quant-core-notes.cpp',
      'platform': 'portable',
      'exercise': '核对所有权转移和 CAS 失败后的 expected；为借用视图补充生命周期约束说明，并设计槽位重复发布需要的反向同步，勿直接删除发布配对。',
    },
  'references':
    [
      {
        'title': 'C++ working draft: Atomic ordering',
        'url': 'https://eel.is/c++draft/atomics.order',
        'kind': 'standard',
        'accessed': '2026-11-12',
      },
      {
        'title': 'C++ working draft: vector capacity',
        'url': 'https://eel.is/c++draft/vector.capacity',
        'kind': 'standard',
        'accessed': '2026-11-12',
      },
      {
        'title': 'Proxy original repository and migration notice',
        'url': 'https://github.com/microsoft/proxy',
        'kind': 'implementation',
        'accessed': '2026-11-12',
      },
      {
        'title': 'Proxy maintained repository',
        'url': 'https://github.com/ngcpp/proxy',
        'kind': 'implementation',
        'accessed': '2026-11-12',
      },
      {
        'title': 'Intel 64 and IA-32 architecture manuals',
        'url': 'https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html',
        'kind': 'manual',
        'accessed': '2026-11-12',
      },
    ],
  'questions':
    [
      {
        'id': 'cpp-quant-core-notes-q01',
        'level': 'L1',
        'prompt': 'unique_ptr 移动后，堆上的对象是否也被移动？',
        'answer': '普通拥有型 unique_ptr 移动转移的是指针及管理责任，被管理对象通常仍在原地址，源 unique_ptr 变为空。对象内部是否支持移动与这个指针管理器的移动是两个问题。',
        'rubric': ['所有权转移', '对象地址稳定', '源指针为空'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q02',
        'level': 'L1',
        'prompt': 'RAII 与 RTTI 各自解决什么问题？',
        'answer': 'RAII 将资源清理绑定到管理对象生命周期，适用于内存、锁与文件；RTTI 提供运行时类型信息，例如 typeid 与 dynamic_cast。运行时类型检查本身不承担资源释放。',
        'rubric': ['资源生命周期', '运行时类型信息', '两者责任不同'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q03',
        'level': 'L1',
        'prompt': 'virtual、CRTP 与类型擦除的主要区别是什么？',
        'answer': 'virtual 可以按对象动态类型分发，CRTP 在模板实例化时绑定具体类型，类型擦除把不同业务类型包装到统一运行时接口。选择时还要明确继承要求、目标所有权与异构集合需求。',
        'rubric': ['类型绑定时机', '运行时统一接口', '所有权边界'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q04',
        'level': 'L1',
        'prompt': 'vector::reserve 会改变 size 并构造相应元素吗？',
        'answer': '不会。reserve 只确保容量满足请求，不增加已有元素数量。若触发重分配，元素指针、引用与迭代器会失效；之后仍要按具体插入或删除操作判断失效范围。',
        'rubric': ['容量与大小区分', '不构造新元素', '重分配失效'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q05',
        'level': 'L1',
        'prompt': '什么是伪共享，它与真共享有什么区别？',
        'answer': '伪共享是不同核心修改不同变量，但变量落在同一缓存行引起写权限争夺；真共享是访问同一共享对象造成的争用。原子性和对齐不能自动消除所有一致性成本。',
        'rubric': ['不同变量同缓存行', '同对象真共享', '原子性不消除争用'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q06',
        'level': 'L1',
        'prompt': 'atomic 与 lock-free 为什么不能直接画等号？',
        'answer': 'atomic 规定相应操作的原子语义，具体类型和平台可能借助锁实现。即便某条原子操作无锁，整个算法仍可能在分配、回收或等待中阻塞，需要分别检查实现与进展条件。',
        'rubric': ['类型和平台', '原子性定义', '完整算法进展'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q07',
        'level': 'L2',
        'prompt': 'std::move 后为什么仍有可能发生复制？',
        'answer': 'std::move 只改变表达式的值类别，重载决议仍可能选择复制操作。类型可能没有适合的移动重载，或 const 等限定使移动接口无法绑定；不能由出现 std::move 就断言零拷贝。',
        'rubric': ['值类别转换', '重载决议', 'const 或缺少移动'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q08',
        'level': 'L2',
        'prompt': 'Proxy 的 Facade 和所包装指针分别决定哪些事情？',
        'answer': 'Facade 描述操作和复制移动等约束，包装指针或存储形式则参与决定目标所有权与生命周期。裸指针通常借用，拥有型指针负责释放；API 与约束需按实际锁定版本核对。',
        'rubric': ['能力约束', '存储及所有权', '版本契约'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q09',
        'level': 'L2',
        'prompt': 'CAS 失败后为何要重新考虑 expected 和 desired？',
        'answer': '失败会将观察值写回 expected，因此依赖旧值计算的 desired 应基于新的 expected 重新计算。weak 还可能虚假失败，重试逻辑需要保证范围和副作用正确，不能无限重复不安全操作。',
        'rubric': ['expected 被更新', '重新计算依赖值', 'weak 虚假失败'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q10',
        'level': 'L2',
        'prompt': '一次 release/acquire 发布怎样让普通 price 的读取安全？',
        'answer': '先写 price，再 release store 到 ready，消费者 acquire load 读到相应发布值后读取 price，建立跨线程 happens-before。该结论要求发布后没有未经同步的继续写入，槽位复用需另设协议。',
        'rubric': ['读到对应发布', 'happens-before', '复用限制'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q11',
        'level': 'L2',
        'prompt': '为何两个 O(n) 遍历可能产生不同实际耗时？',
        'answer': 'vector 常按连续元素访问，list 下一地址依赖当前节点指针且节点可能分散，因此缓存命中、硬件预取和访存并行不同。比较时还需控制数据、分配器、访问顺序及编译优化。',
        'rubric': ['连续与指针追逐', 'Cache 和预取', '测量控制变量'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q12',
        'level': 'L2',
        'prompt': 'alignas(64) 能把对象固定放在 L1 Cache 中吗？',
        'answer': '不能，它规定对象地址对齐要求，不指定缓存层级。只有确认实际缓存行大小、对象地址和数组步长，才可判断是否隔离了写热点；缓存驻留还受访问与替换行为影响。',
        'rubric': ['地址对齐', '核对缓存行和步长', '不指定 Cache 层级'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q13',
        'level': 'L3',
        'prompt': '如何安全地把历史类型适配成一个运行时行情接口？',
        'answer': '先定义读取能力、异常和线程契约，再决定是借用还是拥有目标，用 Adapter 或类型擦除统一操作。借用目标必须覆盖全部回调与接口调用期；验证取消、销毁、复制移动和错误路径，再比较分发成本。',
        'rubric': ['接口与线程契约', '生命周期证明', '失败路径和测量'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q14',
        'level': 'L3',
        'prompt': '高频计数器改成 relaxed 之后仍很慢，应如何定位？',
        'answer': '先确认该计数不承担发布语义，固定工作量与核位置，观察共享写入和缓存行竞争。比较单共享计数、对齐独立计数与线程分片，记录汇总成本和 p99，不能仅凭更弱内存序许诺提速。',
        'rubric': ['语义先行', '控制竞争及布局', '汇总成本和分布'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q15',
        'level': 'L3',
        'prompt': '行情槽位发布后立即被生产者覆盖，改成 seq_cst 足够吗？',
        'answer': '不够，更强排序不能替代消费者已经读完的所有权确认。应建立复用的反向同步、序列或队列协议，验证慢消费者、关闭与溢出；否则普通数据可能仍有读写冲突。',
        'rubric': ['所有权回收', '双向同步', '慢消费者和关闭'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q16',
        'level': 'L3',
        'prompt': '怎样验证 SBO 是否适合某个策略对象包装器？',
        'answer': '固定库版本、对象尺寸、对齐、目标类型分布与复制移动频率，测量分配次数、包装器占用和完整调用路径。比较吞吐与尾延迟，同时核对异常与生命周期，不能只测一次创建。',
        'rubric': ['尺寸和分配', '完整调用负载', '正确性和尾延迟'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q17',
        'level': 'L3',
        'prompt': '如何比较 CAS 循环与 mutex 的业务收益？',
        'answer': '先给两者相同的状态不变量与失败语义，控制竞争率、临界区工作量和线程拓扑，测重试、等待、CPU 与延迟分布。再检查回收、ABA 与关闭路径，单条 CAS 无锁不能替代整个算法的评估。',
        'rubric': ['相同正确性契约', '受控负载', '进展与回收边界'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
      {
        'id': 'cpp-quant-core-notes-q18',
        'level': 'L3',
        'prompt': '用 shared_ptr 保存异步回调目标时，还需要检查什么？',
        'answer': '强引用可以延长目标寿命，但仍要明确捕获链是否构成循环、如何取消回调及目标字段怎样同步。weak_ptr 可避免部分循环，却要求 lock 失败有定义路径；引用计数本身不保护业务数据竞争。',
        'rubric': ['循环和取消', 'weak lock 失败', '业务字段同步'],
        'source':
          {
            'kind': 'derived',
            'rationale': '由本篇对象生命周期、分发、缓存与同步机制及其工程边界推导。',
          },
        'companies': [],
      },
    ],
}
---

## 30 秒面试回答

高性能 C++ 先确定对象由谁拥有、何时销毁，再选择数据布局、分发方式与线程同步。RAII 约束资源生命周期，移动语义允许转移资源，virtual、CRTP 和类型擦除解决不同的多态需求。连续数据通常更容易利用 Cache，但共享写入仍可能造成缓存行竞争。`std::atomic` 只对相应原子对象提供原子性；发布普通数据还需要正确的 happens-before 关系。具体性能要结合分配、访存、竞争和编译结果测量。

本篇由个人速记整理，是跨专题复习入口。已有细节通过末尾链接展开；这里的演示覆盖标准 C++20，不依赖第三方 Proxy 库，也不实现生产级无锁容器。

## 核心概念

### 对象、存储期与 RAII

`Dog dog;` 若位于普通函数块中，通常具有自动存储期，离开作用域时析构；C++ 语言不要求它一定物理存放在栈上。`new Dog()` 创建动态存储期对象，裸指针变量离开作用域不会自动销毁该对象。`std::make_unique<Dog>()` 让拥有型对象负责销毁动态对象。直接创建对象通常最清楚，需要独立的动态生命周期时再选择拥有型指针。

RAII 把资源释放绑定到管理对象的析构过程，适用于内存、锁、文件描述符和连接。资源也可能由工厂函数或移动操作转入，不要求每种 RAII 类型都在默认构造时申请资源。返回局部对象不代表必须堆分配，返回值优化或移动可能避免额外复制。

| 类型               | 所有权与操作               | 生命周期边界                                                  |
| ------------------ | -------------------------- | ------------------------------------------------------------- |
| `unique_ptr<T>`    | 独占，支持移动，不支持复制 | 正常移动后源指针为空；被管理对象地址保持不变                  |
| `shared_ptr<T>`    | 复制时共享所有权           | 复制管理器不等于深复制 `T`；引用计数不自动保护 `T` 的并发读写 |
| `weak_ptr<T>`      | 不增加强引用计数的观察者   | 通过 `lock()` 获取可能为空的临时强引用，不能假定对象仍存在    |
| 裸指针、引用、视图 | 通常表示借用               | 使用期间目标必须存活，释放责任由接口约定决定                  |

### 移动与零拷贝

`std::move(x)` 将表达式转换成允许移动的值类别，实际调用哪个构造或赋值由重载决议决定。类型可能没有移动操作，或因 `const` 等原因走复制。移动 `unique_ptr` 转移管理权，不搬动堆上的对象；移动内嵌缓冲区可能需要逐元素处理。移动后的普通对象应按其类型契约使用，不能一概认为为空。零拷贝还需说明省掉的是对象复制、系统调用复制还是网络传输中的哪一层复制。

### RTTI 与 RAII

RTTI 是运行时类型信息，典型操作是 `typeid` 与 `dynamic_cast`；RAII 是资源生命周期管理。对多态基类指针做向下转换时，指针形式的 `dynamic_cast` 失败会得到空指针。它不承担资源释放责任。通过基类指针删除派生对象时，普通拥有型设计应使用虚析构函数。

## 原理深入

### virtual、CRTP、类型擦除与回调

| 机制                 | 类型何时确定                         | 适用情况                           | 需要验证的边界                                       |
| -------------------- | ------------------------------------ | ---------------------------------- | ---------------------------------------------------- |
| virtual              | 调用时可按动态类型分发               | 已有继承体系、运行时插件或策略对象 | 对象生命周期、虚析构、去虚拟化条件                   |
| CRTP                 | 模板实例化时知道派生类型             | 编译期确定类型的公共算法框架       | 基类到派生类转换必须对应真实对象，代码体积与编译成本 |
| 类型擦除             | 包装器统一类型，调用目标可运行时决定 | 无公共基类的第三方或历史类型       | 包装器拥有什么、复制和移动如何定义                   |
| `std::function` 回调 | 包装可调用对象                       | 订阅、通知或单个行为接口           | 捕获对象生命周期、可能分配、同步或异步由调用方决定   |

虚函数表是常见实现方式，语言标准不规定其固定布局。CRTP 中 `Derived : Base<Derived>` 让基类在成员函数中转到具体类型，通常有利于内联，但 CRTP 本身不会把不同实例统一成一种运行时类型。回调只描述一个调用入口；对象协议包含多个操作时，Facade 或显式接口可能更合适。Adapter 可以用成员包装、自由函数与重载把不同 API 转成同一种能力。

### Microsoft Proxy、Facade 与 SBO

原笔记中的 Microsoft Proxy 指 C++ 类型擦除库，不是网络代理。其 `proxy<Facade>` 用统一包装器承载不同具体类型，业务类型通常无需继承同一基类。Facade 说明支持哪些操作及约束，分发可覆盖成员、自由函数等表达式。`PRO_DEF_MEM_DISPATCH` 定义成员调用分发，`facade_builder::add_convention` 加入操作，`add_facade` 组合已有能力；使用时应以锁定版本的命名空间和签名为准。

包装裸指针通常表示借用；包装拥有型指针则可承载所有权。`make_proxy` 按类型及 Facade 约束选择存储方式，`make_proxy_inplace` 要求符合就地存储条件。Facade 的复制支持（如 `support_copy`）约束可用包装类型；不能只看被指向的业务对象是否可复制。持有 `unique_ptr` 的包装器和就地持有对象的包装器，移动成本可能不同。

SBO（Small Buffer Optimization）在包装器内预留小缓冲区，符合大小、对齐和操作约束的对象可放在其中，减少独立分配；代价可能包括更大的包装器和移动时的数据处理。`std::function` 也可能使用小对象优化，但不能把任意捕获都当成免分配。具体阈值、布局与性能不属于通用语言保证。

截至 2026-11-12，Microsoft 的仓库已归档并指向 [ngcpp/proxy](https://github.com/ngcpp/proxy) 继续开发；[原仓库说明](https://github.com/microsoft/proxy) 保留迁移记录。学习旧笔记时需要区分所用版本，不直接混用不同版本的示例接口。本篇通过自包含的非拥有型视图演示类型擦除机制，不声称复现该库的存储、约束或性能。

### 原子性与内存序

普通共享 `counter++` 包含读、修改、写；若多个线程进行冲突访问且没有 happens-before 关系，就可能构成数据竞争并产生未定义行为。`fetch_add` 把针对该原子对象的读改写作为原子操作，不能据此推导旁边的普通变量也已同步。`load`、`store`、`exchange`、`fetch_sub` 和 CAS 分别表达不同操作；不要用分开的 `load` 加 `store` 代替有并发竞争的原子自增。

| 内存序  | 本篇复习含义                       | 常见使用边界                                        |
| ------- | ---------------------------------- | --------------------------------------------------- |
| relaxed | 保留原子性与相应原子对象的修改顺序 | 计数本身不承担普通数据的发布职责                    |
| release | 发布此前操作                       | 用于适合的写入或读改写；单纯 `load` 不能用 release  |
| acquire | 与读到的发布建立同步               | 用于适合的读取或读改写；单纯 `store` 不能用 acquire |
| acq_rel | 读改写同时具有两侧语义             | 不能机械套给普通 load/store                         |
| seq_cst | 增加相关顺序一致操作的统一排序约束 | 常见默认内存序，仍不自动修复错误的对象生命周期协议  |

生产者先写普通 `price`，再对 `ready` 做 release store；消费者用 acquire load 读到该 store 发布的值后，再读 `price`，才能建立所需同步链。本篇只演示一次发布且发布后不再写 `price`。若生产者反复覆盖同一个槽位，还需要消费者释放使用权的反向同步或其他所有权协议。

### CAS 的 expected 与重试

`compare_exchange_strong(expected, desired)` 比较当前值与 `expected`；成功则写入 `desired`，失败则把观察到的值写回 `expected`。`weak` 允许虚假失败，通常放在重试循环里。重试时必须基于更新后的 `expected` 重新计算依赖旧值的 `desired`，同时检查计数范围。弱 CAS 的虚假失败不意味着观察到了一次真实竞争。

CAS 可用于状态转换、任务所有权竞争和无锁结构，但单个原语不证明整个算法 lock-free。内存分配、回收、ABA、等待策略和关闭流程也属于算法的一部分。`is_lock_free()` 或 `is_always_lock_free` 描述相应类型/对象的实现保证，不能只凭使用 `atomic` 就承诺无锁。Lock-free 关心整体系统进展，wait-free 才要求每个操作在相应有界步骤内完成；二者都不直接等于更低尾延迟。

## 数据结构/系统内部实现

### vector、list 与 reserve

| 比较         | vector                         | list                                           |
| ------------ | ------------------------------ | ---------------------------------------------- |
| 元素布局     | 连续元素存储                   | 节点通常分别分配，实际位置取决于分配器         |
| 随机访问     | 常数时间                       | 按序定位为线性时间                             |
| 遍历         | 线性，通常利于空间局部性与预取 | 线性，但下一地址依赖当前节点指针               |
| 已知位置插入 | 移动后续元素，另需考虑扩容     | 已持有有效迭代器时链接操作为常数，仍有分配成本 |

`reserve(n)` 预留容量，不构造 n 个元素，不改变 `size()`。发生重分配会使原有元素指针、引用与迭代器失效；未重分配的插入也可能使插入点及其后的迭代器或引用失效，末尾迭代器有自己的规则。应区分容器对象地址、元素地址和元素内部资源地址，不能把它们视为同一稳定性承诺。

### Cache、局部性与指针追逐

寄存器、L1/L2/L3、RAM 和持久化存储构成不同访问路径，命中层级、依赖关系与并行度共同决定成本。SSD/HDD 不属于普通 CPU load 可直接访问的 Cache 层级；寄存器分配也由编译器参与决定。不要给所有机器套用固定纳秒数。

典型机器上 L1 与部分 L2 为核心私有，某些末级 Cache 由多个核心共享；SMT 线程可共享所在核心资源。具体拓扑需要查处理器型号。Cache Hit 表示在相应层级命中，Miss 可能由更下层、其他核心缓存或内存服务。许多现代 x86 平台使用 64 字节缓存行，但可移植程序不能把它当成所有架构的常量。

空间局部性指附近地址接续被访问，时间局部性指近期数据再次使用。链表的 pointer chasing 必须先读当前节点才能得到下一节点地址，节点分散和地址依赖会限制预取及访存并行。堆上的连续数组同样可以命中 Cache；存储期并不决定命中层级。

### 伪共享、对齐与 MESI

不同核心频繁写不同变量，而变量恰好位于同一缓存行时，会争夺缓存行写入权限，形成 false sharing；写同一变量则属于真共享。使用原子变量避免数据竞争，并不会自动消除这种一致性成本。读读共享通常不需要反复争夺写权限，但实际行为仍与其他访问有关。

| MESI 状态 | 教学含义                                       |
| --------- | ---------------------------------------------- |
| Modified  | 当前副本已修改，与内存版本不同                 |
| Exclusive | 当前一致性域内仅该缓存持有有效副本，内容干净   |
| Shared    | 可由多个缓存持有干净有效副本                   |
| Invalid   | 该缓存副本不可作为有效数据使用，不要求物理清零 |

这是简化的协议模型；真实 CPU 可以使用 MESIF、MOESI 等变种，不能据此猜测所有层级的精确状态转移。写入需要相应权限，可能使其他副本失效；最新数据可由其他 Cache 提供，不要求每次先写回 RAM。硬件缓存一致性不替代 C++ 的线程同步。

在缓存行为 64 字节的前提下，将两个写热点分别放入适当 `alignas(64)` 的独立对象，可帮助隔离；需检查对象地址、步长和实际缓存行大小。对齐不表示把对象固定放在 L1。Padding、thread-local 分片和批量汇总也可减少共享写入，但会增加占用或引入汇总延迟。对齐无法消除对同一变量的真共享。

## C++ runnable demo

示例核对 unique_ptr 转移后的对象地址、shared_ptr/weak_ptr 生命周期、CRTP、virtual、非拥有型类型擦除和同步回调。随后验证 reserve 不改变大小、一次 release/acquire 发布、两个线程的 relaxed 计数，以及失败 CAS 更新 expected 的行为。

```cpp include=examples/cpp-quant-core-notes.cpp

```

```bash
g++ -std=c++20 -O2 -Wall -Wextra -Wpedantic -Werror -pthread examples/cpp-quant-core-notes.cpp -o /tmp/cpp-quant-core-notes
/tmp/cpp-quant-core-notes
```

预期输出：

```text
ownership, polymorphism and reserve: OK
published price: 100; atomic count after CAS: 2001
```

这是正确性演示，不是性能比较。非拥有型 QuoteView 的目标在整个调用期间存活；示例没有提供 owning Proxy、SBO、通用复制协议或并发对象回收。price 仅写一次，线程 join 后才由主线程读取消费者结果。没有依赖线程恰好按某个时序运行，也没有为 false sharing、lock-free 性能或 Cache 命中率设置必然通过的速度断言。

## 高频追问

### shared_ptr 是否让被管理对象线程安全？

不同 shared_ptr 对象可以按其库契约共享控制块，但对被管理对象的并发冲突访问仍需同步。同一个 shared_ptr 实例的并发修改也需另行处理；选择锁或原子 shared_ptr 等机制时，应确认所用标准与操作。引用计数解决所有权，不提供业务字段的互斥访问。

### SBO、CRTP 或无锁是否必然更快？

都不能仅靠名称判断。SBO 可减少独立分配，却扩大包装器；CRTP 可能更利于内联，也可能增加代码体积；CAS 在高竞争下可反复失败并产生缓存行通信。应控制对象大小、调用目标分布、线程拓扑、分配器和编译选项，再测量整个路径。

### 把原子计数改成 relaxed 能消除伪共享吗？

不能。relaxed 省去的是某些排序要求，不会关闭缓存一致性，也不会改变两个写热点处于同一缓存行的事实。可以先验证计数是否仅用于统计，再分别对内存序和数据布局做控制变量实验。

### 用 seq_cst 是否就能重复覆盖发布槽位？

不能。更强内存序无法告诉生产者消费者是否已用完上一条普通数据。反复复用需要确认读者不再访问，再写下一条；否则仍可能读写冲突。需要设计所有权转移、序列号、双向同步或使用已验证的队列协议。

## 容易答错的点

| 容易混淆的说法                | 应保留的边界                                   |
| ----------------------------- | ---------------------------------------------- |
| move 一定移动且零拷贝         | std::move 只是值类别转换，实际操作依类型决定   |
| shared_ptr 复制对象           | 通常复制管理器并共享对象，业务状态仍需同步     |
| Proxy 一定拥有目标            | 由包装的指针或存储策略决定，裸指针通常借用     |
| CRTP 可以直接替代所有 virtual | 编译期组合与运行时异构集合有不同要求           |
| reserve 后引用永久有效        | 只限制容量范围内的重分配，其他修改仍有失效规则 |
| 堆数据不在 Cache              | Cache 关注地址及访问行为，堆数组也可命中       |
| atomic 等于 lock-free         | 需要检查类型、平台和完整算法的进展条件         |
| MESI 已保证线程同步           | 一致性协议不消除语言层的数据竞争               |
| CAS 失败后 expected 不变      | 失败会写回观察值，weak 还允许虚假失败          |
| 64 字节对齐解决所有共享竞争   | 要先核对缓存行与布局，对齐不能消除真共享       |

## 性能分析

先确认结果和生命周期，再分析分配次数、对象尺寸、访问步长、Cache/TLB miss、分支、共享写入和线程调度。比较 virtual、CRTP 与类型擦除时，确保业务工作量与目标类型分布一致，避免编译器把整段调用消掉；比较容器时同时计入定位、分配、插入和完整遍历。

| 实验               | 需要控制                             | 观察内容                           |
| ------------------ | ------------------------------------ | ---------------------------------- |
| vector 与 list     | 元素总量、分配器、访问顺序、预热     | 整体耗时、分配量与访存事件         |
| 包装器与 SBO       | 对象尺寸、对齐、复制移动频率、库版本 | 包装器体积、分配次数、吞吐与尾延迟 |
| 分片计数与共享计数 | 固定线程与核位置、更新总量、汇总频率 | 一致性通信、吞吐、p99 与内存占用   |
| CAS 与 mutex       | 竞争率、临界区大小、等待策略         | 重试、等待、CPU 占用和延迟分布     |

记录 CPU 型号、拓扑、编译器、优化参数、数据分布、重复次数和原始测量。示例的成功输出仅核对有限执行的语义，不给出通用提速百分比，也不承诺某个 Cache 层级的固定延迟。

## Quant/Low-Latency 场景

### 将速记映射到系统组件

| 组件或场景             | 相关机制                        | 需要确认                                       |
| ---------------------- | ------------------------------- | ---------------------------------------------- |
| 行情批次和订单簿遍历   | 连续存储、reserve、冷热字段拆分 | 元素指针失效、工作集和访问模式                 |
| 股票代码索引           | 哈希表及负载因子                | 冲突、rehash、键生命周期和最坏情况             |
| 高频计数和观测         | atomic relaxed、线程分片        | 是否只是统计，汇总是否允许滞后                 |
| 行情生产消费           | release/acquire、SPSC 协议      | 发布、复用、关闭与消费者落后的处理             |
| 编译期策略计算         | CRTP、模板、内联                | 代码体积、依赖和实际编译结果                   |
| 运行时策略或第三方接口 | virtual、Proxy、Adapter、回调   | 目标所有权、异常、线程与回调取消               |
| 实时与历史数据         | Hot/Warm/Cold 分层              | 实时工作集在内存中，历史归档可用 NVMe/对象存储 |

Hot/Warm/Cold 是访问频率与时效分类，不是保证实时对象一定驻留 Cache 的机制。关键行情通常需要内存中的工作集；当日中间数据、历史 Tick 与回测数据可按容量、恢复时间和成本选择不同持久化层。

### 后续学习清单

- 并发：严格的 happens-before、ABA 与安全回收、SPSC/MPSC/MPMC、mutex/spinlock/futex、fence、lock-free 与 wait-free 的进展定义。
- 数据结构：unordered_map、负载因子、开放寻址、Robin Hood、Swiss Table 与内存池，先区分标准容器保证和具体实现。
- CPU 与操作系统：分支预测、SIMD、TLB、NUMA、CPU affinity、上下文切换、系统调用及用户态/内核态。
- 零拷贝：span/string_view 的借用期、RVO/NRVO、mmap/sendfile、环形缓冲区读写与槽位复用。
- 编译优化：内联与禁止内联、汇编依赖和流水线、LTO、PGO、自动向量化。以编译器输出和测量验证预期。

## 相关专题

- [RAII 与异常安全](raii-exception-safety.md)：资源、异常路径与提交边界。
- [move 与值类别](move-value-categories.md)：移动重载与返回值优化。
- [对象生命周期与布局](object-lifetime-layout.md)：存储、构造与借用。
- [vector 失效规则](vector-invalidation.md)：容量变化和元素引用。
- [虚函数与去虚拟化](vtable-devirtualization.md)：运行时调用与优化条件。
- [C++ 内存模型](../concurrency/cpp-memory-model.md)：发布与复用的双向同步。
- [ABA 与安全回收](../concurrency/aba-reclamation.md)：原子更新之后的对象寿命。
- [CPU Cache 与伪共享](../performance/cpu-cache-false-sharing.md)：布局、争用与实验控制。
- [Benchmark 方法](../performance/benchmark-methodology.md)：控制变量和尾延迟。

## 分层面试题

L1 先区分所有权、值类别、分发方式、缓存行与原子性。L2 要能沿一次移动、一次发布和一次失败 CAS 解释状态变化。L3 从行情队列、异构策略接口或高竞争计数器出发，给出生命周期不变量、失败路径和可重放的测量方案。先独立作答，再展开参考答案与评分点。
