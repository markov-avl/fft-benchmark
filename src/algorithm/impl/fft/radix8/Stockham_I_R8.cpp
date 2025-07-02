#include "Stockham_I_R8.h"

#include <cmath>
#include <thread>

#include "algorithm.h"
#include "multiprocessing.h"


void Stockham_I_R8::forward(const size_t n, ft_complex *in, ft_complex *out) {
    if (n == 1) {
        FT_COPY(in[0], out[0]);
        return;
    }

    const size_t T = get_max_threads();
    ft_complex *x = in;
    ft_complex *y = out;

    for (size_t n1 = n >> 2, s = 1; n1 > 0; n1 >>= 2, s <<= 2) {
        constexpr ft_complex j = {0.0, 1.0};
        const size_t n2 = n1 + n1;
        const size_t n3 = n1 + n2;
        const double theta = std::numbers::pi / static_cast<double>(n2);
        std::vector<std::thread> threads;

        auto task = [&](const size_t t) {
            const auto [start, end] = thread_range(n1, t, T);
            for (size_t p = start; p < end; ++p) {
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

        for (size_t t = 1; t < T; ++t) {
            threads.emplace_back(task, t);
        }
        task(0);

        for (auto &t: threads) {
            t.join();
        }

        std::swap(x, y);
    }

    if (std::countr_zero(n) / 2 % 2 == 0) {
        FT_ARRAY_COPY(n, in, out);
    }
}
