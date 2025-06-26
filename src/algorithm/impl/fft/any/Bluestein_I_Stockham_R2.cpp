#include "Bluestein_I_Stockham_R2.h"

#include <cmath>
#include <thread>

#include "algorithm.h"


static void stockham_forward(const size_t n, ft_complex *sequence, const size_t thread_count = 1) {
    auto *work_area = new ft_complex[n];
    ft_complex *x = sequence;
    ft_complex *y = work_area;

    for (size_t half = n >> 1, s = 1; half > 1; half >>= 1, s <<= 1) {
        const double theta = std::numbers::pi / static_cast<double>(half);
        std::vector<std::thread> threads;

        auto task = [&](const size_t t) {
            const auto [start, end] = thread_range(half, t, thread_count);

            for (size_t p = start; p < end; ++p) {
                const double angle = static_cast<double>(p) * theta;
                const ft_complex w = {std::cos(angle), -std::sin(angle)};

                for (size_t q = 0; q < s; ++q) {
                    ft_complex a, b;
                    FT_COPY(x[q + s * p], a);
                    FT_COPY(x[q + s * (p + half)], b);
                    FT_ADD(a, b, y[q + s * (2 * p)]);
                    FT_SUB(a, b, y[q + s * (2 * p + 1)]);
                    FT_RMUL(y[q + s * (2 * p + 1)], w);
                }
            }
        };

        for (size_t t = 1; t < thread_count; ++t) {
            threads.emplace_back(task, t);
        }
        task(0);

        for (auto &t: threads) {
            t.join();
        }

        std::swap(x, y);
    }

    const size_t half = n >> 1;
    for (size_t q = 0; q < half; ++q) {
        ft_complex a, b;
        FT_COPY(x[q], a);
        FT_COPY(x[q + half], b);
        FT_ADD(a, b, sequence[q]);
        FT_SUB(a, b, sequence[q + half]);
    }

    delete[] work_area;
}

static void stockham_inverse(const size_t n, ft_complex *sequence, const size_t thread_count = 1) {
    for (size_t i = 0; i < n; ++i) {
        FT_RCONJ(sequence[i]);
    }

    stockham_forward(n, sequence, thread_count);

    for (size_t i = 0; i < n; ++i) {
        sequence[i][0] = sequence[i][0] / static_cast<double>(n);
        sequence[i][1] = -sequence[i][1] / static_cast<double>(n);
    }
}

void Bluestein_I_Stockham_R2::initialize(const size_t n, ft_complex *in, ft_complex *out) {
    const size_t k = 2 * n - 1;
    for (l = 1; l < k; l <<= 1) {
    }
}

void Bluestein_I_Stockham_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    std::vector<std::thread> threads;
    const size_t thread_count = get_max_threads();
    auto *u = new ft_complex[l];
    auto *v = new ft_complex[l];

    auto task = [&](const size_t t) {
        const auto [start, end] = thread_range(n, t, thread_count);
        for (size_t i = start; i < end; ++i) {
            FT_POLAR(-std::numbers::pi * i * (static_cast<double>(i) / n), out[i]);
            FT_MUL(in[i], out[i], u[i]);
            FT_CONJ(v[i], out[i]);
        }
    };

    for (size_t t = 1; t < thread_count; ++t) {
        threads.emplace_back(task, t);
    }
    task(0);
    for (auto &t: threads) {
        t.join();
    }

    for (size_t i = n; i < l; ++i) {
        FT_ZERO(u[i]);
        FT_ZERO(v[i]);
    }
    for (size_t i = 1; i < n; ++i) {
        FT_COPY(v[i], v[l - i]);
    }

    if (thread_count > 1) {
        std::thread t(stockham_forward, l, u, std::max(1ul, thread_count / 2));
        stockham_forward(l, v, std::max(1ul, thread_count / 2));
        t.join();
    } else {
        stockham_forward(l, u);
        stockham_forward(l, v);
    }

    for (size_t i = 0; i < l; ++i) {
        FT_RMUL(u[i], v[i]);
    }

    stockham_inverse(l, u, thread_count);

    for (size_t i = 0; i < n; ++i) {
        FT_RMUL(out[i], u[i]);
    }

    delete[] u;
    delete[] v;
}
