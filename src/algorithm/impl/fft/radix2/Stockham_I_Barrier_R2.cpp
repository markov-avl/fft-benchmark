#include "Stockham_I_Barrier_R2.h"

#include <barrier>
#include <cmath>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


void Stockham_I_Barrier_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    if (n == 1) {
        FT_COPY(in[0], out[0]);
        return;
    }

    const size_t T = get_max_threads();
    std::barrier barrier(static_cast<long>(T));
    std::vector<std::thread> threads;

    auto task = [&](const size_t t) {
        ft_complex *x = in;
        ft_complex *y = out;

        for (size_t half = n >> 1, s = 1; half > 1; half >>= 1, s <<= 1) {
            const double theta = std::numbers::pi / static_cast<double>(half);
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

            std::swap(x, y);
            barrier.arrive_and_wait();
        }
    };

    for (size_t t = 1; t < T; ++t) {
        threads.emplace_back(task, t);
    }
    task(0);
    for (auto &t: threads) {
        t.join();
    }

    const size_t half = n >> 1;
    const ft_complex *x = static_cast<int>(std::log2(n)) % 2 ? in : out;
    for (size_t q = 0; q < half; ++q) {
        ft_complex a, b;
        FT_COPY(x[q], a);
        FT_COPY(x[q + half], b);
        FT_ADD(a, b, out[q]);
        FT_SUB(a, b, out[q + half]);
    }
}
