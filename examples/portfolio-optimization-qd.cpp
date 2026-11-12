#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

// Small dense teaching solver, not a general QP or production numerical library.
Vector solve(Matrix a, Vector b) {
    const auto n = a.size();
    if (n == 0 || b.size() != n) throw std::invalid_argument("dimension mismatch");
    double scale = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i].size() != n || !std::isfinite(b[i]))
            throw std::invalid_argument("invalid row or right-hand side");
        for (const auto value : a[i]) {
            if (!std::isfinite(value)) throw std::invalid_argument("non-finite coefficient");
            scale = std::max(scale, std::abs(value));
        }
    }
    if (scale == 0) throw std::runtime_error("singular matrix");
    for (std::size_t col = 0; col < n; ++col) {
        auto pivot = col;
        for (std::size_t row = col + 1; row < n; ++row)
            if (std::abs(a[row][col]) > std::abs(a[pivot][col])) pivot = row;
        if (std::abs(a[pivot][col]) <= 1e-12 * scale)
            throw std::runtime_error("singular or poorly scaled teaching case");
        std::swap(a[col], a[pivot]);
        std::swap(b[col], b[pivot]);
        for (std::size_t row = col + 1; row < n; ++row) {
            const double factor = a[row][col] / a[col][col];
            a[row][col] = 0;
            for (std::size_t j = col + 1; j < n; ++j) a[row][j] -= factor * a[col][j];
            b[row] -= factor * b[col];
        }
    }
    Vector x(n);
    for (std::size_t row = n; row-- > 0;) {
        double rhs = b[row];
        for (std::size_t j = row + 1; j < n; ++j) rhs -= a[row][j] * x[j];
        x[row] = rhs / a[row][row];
    }
    return x;
}

double relative_residual(const Matrix& a, const Vector& x, const Vector& b) {
    double residual = 0, matrix_norm = 0, rhs_norm = 0, x_norm = 0;
    for (const auto value : x) x_norm = std::max(x_norm, std::abs(value));
    for (std::size_t i = 0; i < a.size(); ++i) {
        double ax = 0, row_norm = 0;
        for (std::size_t j = 0; j < x.size(); ++j) {
            ax += a[i][j] * x[j];
            row_norm += std::abs(a[i][j]);
        }
        residual = std::max(residual, std::abs(ax - b[i]));
        matrix_norm = std::max(matrix_norm, row_norm);
        rhs_norm = std::max(rhs_norm, std::abs(b[i]));
    }
    const auto denominator = matrix_norm * x_norm + rhs_norm;
    return denominator == 0 ? residual : residual / denominator;
}

int main() {
    const Matrix equality{{1, 0, 1}, {0, 1, 1}, {1, 1, 0}};
    const Vector equality_rhs{0, 0, 1};
    const auto x = solve(equality, equality_rhs);
    assert(std::abs(x[0] - 0.5) < 1e-10 && std::abs(x[1] - 0.5) < 1e-10);
    assert(relative_residual(equality, x, equality_rhs) < 1e-12);
    const auto objective = [](const Vector& w) { return 0.5 * (w[0] * w[0] + w[1] * w[1]); };
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "equality optimum: " << x[0] << ", " << x[1]
              << "; objective=" << objective(x) << '\n';

    // Last row encodes the assumed active upper bound w1 = 0.4.
    const Matrix capped{{1, 0, 1, 1}, {0, 1, 1, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}};
    const Vector capped_rhs{0, 0, 1, 0.4};
    const auto w = solve(capped, capped_rhs);
    const double upper_multiplier = w[3];
    assert(std::abs(w[0] - 0.4) < 1e-10 && std::abs(w[1] - 0.6) < 1e-10);
    assert(std::abs(w[0] + w[1] - 1) < 1e-10 && w[0] <= 0.4 + 1e-10);
    assert(upper_multiplier >= 0);
    assert(std::abs(upper_multiplier * (w[0] - 0.4)) < 1e-10);
    assert(relative_residual(capped, w, capped_rhs) < 1e-12);
    assert(objective(w) > objective(x));
    std::cout << "capped optimum: " << w[0] << ", " << w[1]
              << "; multiplier=" << upper_multiplier << "; objective=" << objective(w) << '\n';

    // With an upper bound of 0.6 the unconstrained equality solution is feasible.
    assert(x[0] < 0.6);
    const Vector caps(8, 0.1);
    assert(std::accumulate(caps.begin(), caps.end(), 0.0) < 1.0 - 1e-10);
    std::cout << "8 assets with 10% caps and 100% budget: infeasible by bounds\n";

    bool singular_rejected = false;
    try { (void)solve({{1, 1}, {2, 2}}, {1, 2}); }
    catch (const std::runtime_error&) { singular_rejected = true; }
    assert(singular_rejected);
    bool dimensions_rejected = false;
    try { (void)solve({{1}}, {1, 2}); }
    catch (const std::invalid_argument&) { dimensions_rejected = true; }
    assert(dimensions_rejected);
}
