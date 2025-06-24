#include "Stockham_I_TP_R2.h"

#include <cmath>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n,
                const size_t s,
                const bool eo,
                ft_complex *x,
                ft_complex *y,
                ThreadPool *thread_pool) {
    if (n == 2) {
        ft_complex *z = eo ? y : x;

        for (size_t q = 0; q < s; ++q) {
            ft_complex a, b;
            FT_COPY(x[q], a);
            FT_COPY(x[q + s], b);
            FT_ADD(a, b, z[q]);
            FT_SUB(a, b, z[q + s]);
        }
    } else if (n > 2) {
        const size_t half = n / 2;
        const size_t thread_count = thread_pool->size() + 1;
        const double theta = std::numbers::pi / static_cast<double>(half);

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
            thread_pool->enqueue(task, t);
        }
        task(0);
        thread_pool->wait_all();

        fft(half, 2 * s, !eo, y, x, thread_pool);
    }
}

void Stockham_I_TP_R2::initialize(const size_t n, ft_complex *in, ft_complex *out) {
    thread_pool = new ThreadPool(get_max_threads() - 1);
}

void Stockham_I_TP_R2::finalize(const size_t n, ft_complex *in, ft_complex *out) {
    delete thread_pool;
}

void Stockham_I_TP_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, false, in, out, thread_pool);
    FT_ARRAY_COPY(n, in, out);
}
