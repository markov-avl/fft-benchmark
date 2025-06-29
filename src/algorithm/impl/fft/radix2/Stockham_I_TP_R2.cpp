#include "Stockham_I_TP_R2.h"

#include <cmath>
#include <thread>

#include "algorithm.h"


void Stockham_I_TP_R2::initialize(const size_t n, ft_complex *in, ft_complex *out) {
    thread_pool = new ThreadPool(get_max_threads() - 1);
}

void Stockham_I_TP_R2::finalize(const size_t n, ft_complex *in, ft_complex *out) {
    delete thread_pool;
}

void Stockham_I_TP_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    if (n == 1) {
        FT_COPY(in[0], out[0]);
        return;
    }

    const size_t T = get_max_threads();
    ft_complex *x = in;
    ft_complex *y = out;

    for (size_t half = n >> 1, s = 1; half > 1; half >>= 1, s <<= 1) {
        const double theta = std::numbers::pi / static_cast<double>(half);

        auto task = [&](const size_t t) {
            const auto [start, end] = thread_range(half, t, T);
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

        for (size_t t = 1; t < T; ++t) {
            thread_pool->enqueue(task, t);
        }
        task(0);
        thread_pool->wait_all();

        std::swap(x, y);
    }

    const size_t half = n >> 1;
    for (size_t q = 0; q < half; ++q) {
        ft_complex a, b;
        FT_COPY(x[q], a);
        FT_COPY(x[q + half], b);
        FT_ADD(a, b, out[q]);
        FT_SUB(a, b, out[q + half]);
    }
}
