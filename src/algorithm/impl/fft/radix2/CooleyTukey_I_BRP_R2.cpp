#include "CooleyTukey_I_BRP_R2.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/brp.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, ft_complex *data) {
    const size_t threads_num = get_max_threads();

    for (size_t step = 2; step <= n; step *= 2) {
        const size_t half = step / 2;
        const size_t num_groups = n / step;
        const size_t groups_per_thread = (num_groups + threads_num - 1) / threads_num;
        const size_t threads_to_spawn = std::min(threads_num - 1, num_groups);
        std::vector<std::thread> threads;

        auto task = [=](const size_t t) {
            const size_t group_start = t * groups_per_thread;
            const size_t group_end = std::min(group_start + groups_per_thread, num_groups);

            for (size_t g = group_start; g < group_end; ++g) {
                ft_complex *group = data + g * step;
                for (size_t k = 0; k < half; ++k) {
                    ft_complex temp;
                    ft_polar(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(half), temp);
                    ft_mul(temp, group[half + k]);
                    ft_sub(group[k], temp, group[half + k]);
                    ft_add(group[k], temp);
                }
            }
        };

        for (size_t t = 0; t < threads_to_spawn; ++t) {
            threads.emplace_back(task, t);
        }
        task(threads_to_spawn);

        for (auto &t: threads) {
            t.join();
        }
    }
}


void CooleyTukey_I_BRP_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    bit_reversal_permutation(n, in, out);
    fft(n, out);
}
