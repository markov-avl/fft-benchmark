#include "Bluestein_I_Stockham_R4.h"

#include <cmath>
#include <thread>

#include "algorithm.h"


static void stockham_forward(const size_t n, ft_complex *sequence, const size_t thread_count = 1) {
    auto *work_area = new ft_complex[n];
    ft_complex *x = sequence;
    ft_complex *y = work_area;

    for (size_t n1 = n >> 2, s = 1; n1 > 0; n1 >>= 2, s <<= 2) {
        constexpr ft_complex j = {0.0, 1.0};
        const size_t n2 = n1 + n1;
        const size_t n3 = n1 + n2;
        const double theta = std::numbers::pi / static_cast<double>(n2);
        std::vector<std::thread> threads;

        auto task = [&](const size_t t) {
            for (size_t p = t; p < n1; p += thread_count) {
                const double angle = static_cast<double>(p) * theta;
                const ft_complex w1 = {std::cos(angle), -std::sin(angle)};
                ft_complex w2, w3;
                FT_MUL(w1, w1, w2);
                FT_MUL(w1, w2, w3);

                for (size_t q = 0; q < s; ++q) {
                    ft_complex a, b, c, d, apc, amc, bpd, jbmd;

                    FT_COPY(x[q + s * p], a);
                    FT_COPY(x[q + s * (p + n1)], b);
                    FT_COPY(x[q + s * (p + n2)], c);
                    FT_COPY(x[q + s * (p + n3)], d);

                    FT_ADD(a, c, apc);
                    FT_SUB(a, c, amc);
                    FT_ADD(b, d, bpd);
                    FT_SUB(b, d, jbmd);
                    FT_RMUL(jbmd, j);

                    FT_ADD(apc, bpd, y[q + s * (4 * p)]);
                    FT_SUB(amc, jbmd, y[q + s * (4 * p + 1)]);
                    FT_RMUL(y[q + s * (4 * p + 1)], w1);
                    FT_SUB(apc, bpd, y[q + s * (4 * p + 2)]);
                    FT_RMUL(y[q + s * (4 * p + 2)], w2);
                    FT_ADD(amc, jbmd, y[q + s * (4 * p + 3)]);
                    FT_RMUL(y[q + s * (4 * p + 3)], w3);
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

void Bluestein_I_Stockham_R4::initialize(const size_t n, ft_complex *in, ft_complex *out) {
    const size_t k = 4 * n - 1;
    for (l = 1; l < k; l <<= 2) {
    }
}

void Bluestein_I_Stockham_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    std::vector<std::thread> threads;
    const size_t thread_count = get_max_threads();
    auto *u = new ft_complex[l];
    auto *v = new ft_complex[l];

    auto task = [&](const size_t t) {
        for (size_t i = t; i < n; i += thread_count) {
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
