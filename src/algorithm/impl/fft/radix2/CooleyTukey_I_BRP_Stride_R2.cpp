#include "CooleyTukey_I_BRP_Stride_R2.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/brp.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, ft_complex *out, const size_t thread_count = 1) {
    for (size_t step = 2; step <= n; step <<= 1) {
        const size_t half = step / 2;
        const size_t groups = n / step;
        const size_t threads_num = std::min(thread_count, groups);

        auto task = [&](const size_t t) {
            const size_t start = t * (groups / threads_num) + std::min(t, groups % threads_num);
            const size_t end = start + groups / threads_num + (t < groups % threads_num ? 1 : 0);

            for (ft_complex* group = out + start * step; group < out + end * step; group += step) {
                for (size_t k = 0; k < half; ++k) {
                    ft_complex w;
                    ft_polar(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(half), w);
                    ft_mul(w, group[half + k]);
                    ft_sub(group[k], w, group[half + k]);
                    ft_add(group[k], w);
                }
            }
        };

        std::vector<std::thread> threads;
        for (size_t t = 1; t < threads_num; ++t) {
            threads.emplace_back(task, t);
        }
        task(0);

        for (auto& t : threads) {
            t.join();
        }
    }
}


void CooleyTukey_I_BRP_Stride_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    bit_reversal_permutation(n, in, out);
    fft(n, out, get_max_threads());
}