#include "Stockham_R_R4.h"

#include <cmath>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n,
                const size_t s,
                const size_t q,
                const bool eo,
                ft_complex *x,
                ft_complex *y,
                const size_t thread_count = 1) {
    if (n == 2) {
        ft_complex *z = eo ? y : x;

        ft_complex a, b;
        FT_COPY(x[q], a);
        FT_COPY(x[q + s], b);
        FT_ADD(a, b, z[q]);
        FT_SUB(a, b, z[q + s]);
    } else if (n > 2) {
        const size_t half = n / 2;
        const double theta = std::numbers::pi / static_cast<double>(half);

        auto task = [&](const size_t t) {
            const size_t p_start = t * half / thread_count;
            const size_t p_end = t == thread_count - 1 ? half : (t + 1) * half / thread_count;

            for (size_t p = p_start; p < p_end; ++p) {
                const double angle = static_cast<double>(p) * theta;
                const ft_complex w = {std::cos(angle), -std::sin(angle)};
                ft_complex a, b;

                FT_COPY(x[q + s * p], a);
                FT_COPY(x[q + s * (p + half)], b);
                FT_ADD(a, b, y[q + s * (2 * p)]);
                FT_SUB(a, b, y[q + s * (2 * p + 1)]);
                FT_RMUL(y[q + s * (2 * p + 1)], w);
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

        if (thread_count > 1 && n >= 4) {
            std::thread t1(fft, half, 2 * s, q, !eo, y, x, thread_count / 2);
            fft(half, 2 * s, q + s, !eo, y, x, thread_count / 2);
            t1.join();
        } else {
            fft(half, 2 * s, q, !eo, y, x);
            fft(half, 2 * s, q + s, !eo, y, x);
        }
    }
}

void Stockham_R_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, 0, false, in, out, get_max_threads());
    for (size_t i = 0; i < n; ++i) {
        FT_COPY(in[i], out[i]);
    }
}
