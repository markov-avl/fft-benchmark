#include "drp.h"

#include <bit>
#include <cmath>
#include <thread>
#include <vector>

#include "multiprocessing.h"
#include "operation.h"


static uint64_t quaternary_reverse64(size_t i, const size_t log4_n) {
    size_t result = 0;
    for (size_t j = 0; j < log4_n; ++j) {
        result = result << 2 | i & 3;
        i >>= 2;
    }
    return result;
}


void quaternary_reversal_permutation(const size_t n, const ft_complex *in, ft_complex *out) {
    const size_t log4_n = std::countr_zero(n) / 2;
    const size_t thread_count = get_max_threads();
    std::vector<std::thread> threads;

    auto permutate = [&](const size_t t) {
        for (size_t i = t; i < n; i += thread_count) {
            FT_COPY(in[i], out[quaternary_reverse64(i, log4_n)]);
        }
    };

    for (size_t i = 1; i < thread_count; ++i) {
        threads.emplace_back(permutate, i);
    }
    permutate(0);

    for (auto &t: threads) {
        t.join();
    }
}


void digit_reversal_permutation(const size_t base, const size_t n, const ft_complex *in, ft_complex *out) {
    switch (base) {
        case 4:
            quaternary_reversal_permutation(n, in, out);
            return;
        default:
            // TODO: Нет многопоточности
            const auto logb = static_cast<size_t>(std::log(n) / std::log(base));

            for (size_t i = 0; i < n; ++i) {
                size_t reversed = 0, x = i;
                for (size_t j = 0; j < logb; ++j) {
                    reversed = reversed * base + x % base;
                    x /= base;
                }
                FT_COPY(in[i], out[reversed]);
            }
    }
}
