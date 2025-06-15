#include "Stockham_I_R4.h"

#include <cmath>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, const size_t s, const bool eo, ft_complex *x, ft_complex *y) {
    const size_t n1 = n / 4;
    const size_t n2 = n / 2;
    const size_t n3 = n1 + n2;
    const double theta = std::numbers::pi / static_cast<double>(n2);

    if (n == 1) {
        if (eo) {
            for (size_t q = 0; q < s; ++q) {
                ft_copy(x[q], y[q]);
            }
        }
    } else if (n == 2) {
        for (size_t q = 0; q < s; ++q) {
            ft_complex a, b;
            ft_copy(x[q + 0], a);
            ft_copy(x[q + s], b);
            ft_add(a, b, y[q]);
            ft_sub(a, b, y[q + s]);
        }

        fft(1, 2 * s, !eo, y, x);
    } else if (n > 2) {
        constexpr ft_complex j = {0.0, 1.0};
        const size_t max_threads = get_max_threads();
        const size_t thread_count = std::min(n1, max_threads);

        auto task = [&](const size_t t) {
            const size_t p_start = t * n1 / thread_count;
            const size_t p_end = t == thread_count - 1 ? n1 : (t + 1) * n1 / thread_count;

            for (size_t p = p_start; p < p_end; ++p) {
                const double angle = static_cast<double>(p) * theta;
                ft_complex w1 = {std::cos(angle), -std::sin(angle)}, w2, w3;
                ft_mul(w1, w1, w2);
                ft_mul(w1, w2, w3);

                for (size_t q = 0; q < s; ++q) {
                    ft_complex a, b, c, d, apc, amc, bpd, jbmd;

                    ft_copy(x[q + s * p], a);
                    ft_copy(x[q + s * (p + n1)], b);
                    ft_copy(x[q + s * (p + n2)], c);
                    ft_copy(x[q + s * (p + n3)], d);

                    ft_add(a, c, apc);
                    ft_sub(a, c, amc);
                    ft_add(b, d, bpd);
                    ft_sub(b, d, jbmd);
                    ft_mul(jbmd, j);

                    ft_add(apc, bpd, y[q + s * (4 * p)]);
                    ft_sub(amc, jbmd, y[q + s * (4 * p + 1)]);
                    ft_mul(y[q + s * (4 * p + 1)], w1);
                    ft_sub(apc, bpd, y[q + s * (4 * p + 2)]);
                    ft_mul(y[q + s * (4 * p + 2)], w2);
                    ft_add(amc, jbmd, y[q + s * (4 * p + 3)]);
                    ft_mul(y[q + s * (4 * p + 3)], w3);
                }
            }
        };

        std::vector<std::thread> threads;

        for (size_t t = 1; t < thread_count; ++t) {
            threads.emplace_back(task, t);
        }
        task(0);

        for (auto &thread: threads) {
            thread.join();
        }

        fft(n1, 4 * s, !eo, y, x);
    }
}

void Stockham_I_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, false, in, out);
    for (size_t i = 0; i < n; ++i) {
        ft_copy(in[i], out[i]);
    }
}
