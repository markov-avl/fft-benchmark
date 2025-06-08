#include "GoodThomas_I.h"

#include <cmath>
#include <iostream>
#include <numeric>

#include "algorithm/utils/operation.h"


size_t find_optimal_factor(const size_t n) {
    size_t n1 = 1;
    size_t min_max = n;

    for (size_t N1 = 2; static_cast<double>(N1) <= std::sqrt(n); ++N1) {
        if (n % N1 != 0) {
            continue;
        }
        size_t N2 = n / N1;
        if (std::gcd(N1, N2) != 1) {
            continue;
        }

        if (const size_t max_len = std::max(N1, N2); max_len < min_max) {
            min_max = max_len;
            n1 = N1;
        }
    }

    return n1;
}

static void dft(const size_t n, ft_complex *data) {
    auto *temp = new ft_complex[n];
    for (size_t k = 0; k < n; ++k) {
        temp[k][0] = 0.0, temp[k][1] = 0.0;

        for (size_t m = 0; m < n; ++m) {
            ft_complex w;
            ft_polar(-2.0 * std::numbers::pi * static_cast<double>(m * k) / static_cast<double>(n), w);
            ft_mul(w, data[m]);
            ft_add(temp[k], w);
        }
    }

    for (size_t k = 0; k < n; ++k) {
        ft_copy(temp[k], data[k]);
    }

    delete[] temp;
}

static void fft(const size_t n, const size_t n1, const size_t n2, const ft_complex *in, ft_complex *out) {
    auto *transposed = new ft_complex[n];

    for (size_t k1 = 0; k1 < n1; ++k1) {
        for (size_t k2 = 0; k2 < n2; ++k2) {
            ft_copy(in[n1 * k2 + k1], out[k1 * n2 + k2]);
        }
    }

    for (size_t k1 = 0; k1 < n1; ++k1) {
        dft(n2, out + k1 * n2);
    }
    for (size_t k1 = 0; k1 < n1; ++k1) {
        for (size_t k2 = 0; k2 < n2; ++k2) {
            ft_complex w;
            ft_polar(-2.0 * std::numbers::pi * static_cast<double>(k1 * k2) / static_cast<double>(n), w);
            ft_mul(w, out[k1 * n2 + k2], transposed[k2 * n1 + k1]);
        }
    }
    for (size_t k2 = 0; k2 < n2; ++k2) {
        dft(n1, transposed + k2 * n1);
    }

    for (size_t k1 = 0; k1 < n1; ++k1) {
        for (size_t k2 = 0; k2 < n2; ++k2) {
            ft_copy(transposed[k2 * n1 + k1], out[n2 * k1 + k2]);
        }
    }

    delete[] transposed;
}

void GoodThomas_I::forward(const size_t n, ft_complex *in, ft_complex *out) {
    const size_t n1 = find_optimal_factor(n);
    fft(n, n1, n / n1, in, out);
}
