#include "CooleyTukey_I_Barrier_DRP_Stride_R4.h"

#include <barrier>
#include <cmath>
#include <thread>
#include <vector>

#include "multiprocessing.h"
#include "algorithm/utils/drp.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, ft_complex *out, const size_t thread_count = 1) {
    const size_t threads_num = std::min(thread_count, static_cast<size_t>(std::log2(n) / 2));
    std::barrier barrier(static_cast<long>(threads_num));

    auto task = [&](const size_t t) {
        for (size_t step = 4; step <= n; step <<= 2) {
            const size_t quarter = step / 4;
            const size_t groups = n / step;
            const size_t start = t * (groups / threads_num) + std::min(t, groups % threads_num);
            const size_t end = start + groups / threads_num + (t < groups % threads_num ? 1 : 0);

            for (ft_complex* group = out + start * step; group < out + end * step; group += step) {
                for (size_t k = 0; k < quarter; ++k) {
                    ft_complex w1, w2, w3;
                    ft_polar(-0.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w1);
                    ft_polar(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w2);
                    ft_polar(-1.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w3);

                    ft_complex m, m1w, m2w, m3w;
                    ft_copy(group[k], m);
                    ft_mul(group[quarter + k], w1, m1w);
                    ft_mul(group[2 * quarter + k], w2, m2w);
                    ft_mul(group[3 * quarter + k], w3, m3w);

                    group[k][0] += m1w[0] + m2w[0] + m3w[0];
                    group[k][1] += m1w[1] + m2w[1] + m3w[1];
                    group[quarter + k][0] = m[0] + m1w[1] - m2w[0] - m3w[1];
                    group[quarter + k][1] = m[1] - m1w[0] - m2w[1] + m3w[0];
                    group[2 * quarter + k][0] = m[0] - m1w[0] + m2w[0] - m3w[0];
                    group[2 * quarter + k][1] = m[1] - m1w[1] + m2w[1] - m3w[1];
                    group[3 * quarter + k][0] = m[0] - m1w[1] - m2w[0] + m3w[1];
                    group[3 * quarter + k][1] = m[1] + m1w[0] - m2w[1] - m3w[0];
                }
            }
            barrier.arrive_and_wait();
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


void CooleyTukey_I_Barrier_DRP_Stride_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    digit_reversal_permutation(4, n, in, out);
    fft(n, out, get_max_threads());
}
