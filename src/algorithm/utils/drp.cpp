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

static uint64_t octal_reverse64(size_t i, const size_t log8_n) {
    size_t result = 0;
    for (size_t j = 0; j < log8_n; ++j) {
        result = result << 3 | i & 7;
        i >>= 3;
    }
    return result;
}

void quaternary_reversal_permutation(const size_t n, const ft_complex *in, ft_complex *out) {
    const size_t log4_n = std::countr_zero(n) / 2;
    const size_t T = get_max_threads();
    std::vector<std::thread> threads;

    auto permutate = [&](const size_t t) {
        const auto [start, end] = thread_range(n, t, T);
        for (size_t i = start; i < end; ++i) {
            FT_COPY(in[i], out[quaternary_reverse64(i, log4_n)]);
        }
    };

    for (size_t i = 1; i < T; ++i) {
        threads.emplace_back(permutate, i);
    }
    permutate(0);

    for (auto &t: threads) {
        t.join();
    }
}


void octal_reversal_permutation(const size_t n, const ft_complex *in, ft_complex *out) {
    const size_t log8_n = std::countr_zero(n) / 3;
    const size_t T = get_max_threads();
    std::vector<std::thread> threads;

    auto permutate = [&](const size_t t) {
        const auto [start, end] = thread_range(n, t, T);
        for (size_t i = start; i < end; ++i) {
            FT_COPY(in[i], out[octal_reverse64(i, log8_n)]);
        }
    };

    for (size_t i = 1; i < T; ++i) {
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
        case 8:
            octal_reversal_permutation(n, in, out);
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
