#include "CooleyTukey_I_Barrier_BRP_Stride_R2.h"

#include <barrier>
#include <cmath>
#include <thread>
#include <vector>

#include "algorithm/utils/brp.h"
#include "algorithm/utils/operation.h"
#include "multiprocessing/threads.h"


static void fft(const size_t n, ft_complex *out, const size_t thread_count = 1) {
    const size_t threads_num = std::min(thread_count, static_cast<size_t>(std::log2(n)));
    std::barrier barrier(static_cast<long>(threads_num));
    std::vector<std::thread> threads;

    auto task = [&](const size_t t) {
        for (size_t step = 2; step <= n; step <<= 1) {
            const size_t half = step / 2;
            const size_t groups = n / step;
            const size_t start = t * (groups / threads_num) + std::min(t, groups % threads_num);
            const size_t end = start + groups / threads_num + (t < groups % threads_num ? 1 : 0);

            for (ft_complex *group = out + start * step; group < out + end * step; group += step) {
                for (size_t k = 0; k < half; ++k) {
                    ft_complex w;
                    FT_POLAR(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(half), w);
                    FT_RMUL(w, group[half + k]);
                    FT_SUB(group[k], w, group[half + k]);
                    FT_RADD(group[k], w);
                }
            }

            barrier.arrive_and_wait();
        }
    };

    for (size_t t = 1; t < threads_num; ++t) {
        threads.emplace_back(task, t);
    }
    task(0);

    for (auto &t: threads) {
        t.join();
    }
}


void CooleyTukey_I_Barrier_BRP_Stride_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    bit_reversal_permutation(n, in, out);
    fft(n, out, get_max_threads());
}
