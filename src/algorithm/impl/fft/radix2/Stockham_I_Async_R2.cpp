#include "Stockham_I_Async_R2.h"

#include <cmath>
#include <future>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n,
                const size_t s,
                const bool eo,
                ft_complex *x,
                ft_complex *y,
                const size_t task_count = 1) {
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
        const size_t tasks = std::min(task_count, half);
        const double theta = std::numbers::pi / static_cast<double>(half);
        std::vector<std::future<void> > futures;

        for (size_t t = 0; t < tasks; ++t) {
            futures.push_back(std::async(std::launch::async, [&, t] {
                const size_t p_start = t * half / tasks;
                const size_t p_end = t == tasks - 1 ? half : (t + 1) * half / tasks;

                for (size_t p = p_start; p < p_end; ++p) {
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
            }));
        }

        if (half > 1) {
            auto task = std::async(std::launch::async, [&] {
                fft(half, 2 * s, !eo, y, x, std::max(1ul, task_count / 2));
            });

            for (auto &f: futures) {
                f.get();
            }
            task.get();
        } else {
            for (auto &f: futures) {
                f.get();
            }
        }
    }
}

void Stockham_I_Async_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, false, in, out, get_max_threads());
    FT_ARRAY_COPY(n, in, out);
}
