#include "CooleyTukey_I_DRP_Stride_R8.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/drp.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, ft_complex *out, const size_t T = 1) {
    for (size_t step = 8; step <= n; step <<= 3) {
        const size_t eighth = step / 8;
        const size_t groups = n / step;
        const size_t threads_num = std::min(T, groups);
        std::vector<std::thread> threads;

        auto task = [&](const size_t t) {
            const auto [start, end] = thread_range(groups, t, threads_num);

            for (ft_complex *group = out + start * step; group < out + end * step; group += step) {
                for (size_t k = 0; k < eighth; ++k) {
                }
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
}


void CooleyTukey_I_DRP_Stride_R8::forward(const size_t n, ft_complex *in, ft_complex *out) {
    digit_reversal_permutation(8, n, in, out);
    fft(n, out, get_max_threads());
}
