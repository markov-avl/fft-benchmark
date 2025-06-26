#include "GoodThomas_I.h"

#include <barrier>
#include <cmath>
#include <numeric>
#include <thread>

#include "algorithm.h"
#include "multiprocessing.h"


static size_t find_optimal_coprime(const size_t n) {
    for (auto n1 = static_cast<size_t>(std::sqrt(n)); n1 > 1; --n1) {
        if (n % n1 == 0 && std::gcd(n1, n / n1) == 1) {
            return n1;
        }
    }

    return 1;
}

void GoodThomas_I::check_preconditions(const size_t n, ft_complex *in, ft_complex *out) {
    n1 = find_optimal_coprime(n);
    if (n1 == 1) {
        throw std::invalid_argument("n should be decomposed as coprimes");
    }
}

void GoodThomas_I::initialize(const size_t n, ft_complex *in, ft_complex *out) {
    n2 = n / n1;
}

// TODO: В алгоритме зашито DFT
void GoodThomas_I::forward(const size_t n, ft_complex *in, ft_complex *out) {
    const auto transposed = new ft_complex[n];
    const size_t thread_count = get_max_threads();
    std::vector<std::thread> threads;
    std::barrier barrier(static_cast<long>(thread_count));

    auto task = [&](const size_t t) {
        const auto [n1_start, n1_end] = thread_range(n1, t, thread_count);
        for (size_t k1 = n1_start; k1 < n1_end; ++k1) {
            for (size_t k2 = 0; k2 < n2; ++k2) {
                const double angle_delta = -2.0 * std::numbers::pi * static_cast<double>(k2) / static_cast<double>(n2);
                double angle = angle_delta;

                FT_COPY(in[k1], out[k1 * n2 + k2]);

                for (size_t m = 1; m < n2; ++m, angle += angle_delta) {
                    ft_complex w;
                    FT_POLAR(angle, w);
                    FT_RMUL(w, in[n1 * m + k1]);
                    FT_RADD(out[k1 * n2 + k2], w);
                }

                ft_complex w;
                FT_POLAR(-2.0 * std::numbers::pi * static_cast<double>(k1 * k2) / static_cast<double>(n), w);
                FT_MUL(w, out[k1 * n2 + k2], transposed[k2 * n1 + k1]);
            }
        }

        barrier.arrive_and_wait();

        const auto [n2_start, n2_end] = thread_range(n2, t, thread_count);
        for (size_t k2 = n2_start; k2 < n2_end; ++k2) {
            for (size_t k1 = 0; k1 < n1; ++k1) {
                const double angle_delta = -2.0 * std::numbers::pi * static_cast<double>(k1) / static_cast<double>(n1);
                double angle = angle_delta;

                FT_COPY(transposed[k2 * n1], out[n2 * k1 + k2]);

                for (size_t m = 1; m < n1; ++m, angle += angle_delta) {
                    ft_complex w;
                    FT_POLAR(angle, w);
                    FT_RMUL(w, transposed[k2 * n1 + m]);
                    FT_RADD(out[n2 * k1 + k2], w);
                }
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

    delete[] transposed;
}
