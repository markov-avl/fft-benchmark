#include "GoodThomas_I.h"

#include <cmath>
#include <future>
#include <numeric>
#include <thread>

#include "algorithm/utils/operation.h"


static size_t find_optimal_coprime(const size_t n) {
    for (auto N1 = static_cast<size_t>(std::sqrt(n)); N1 > 1; --N1) {
        if (n % N1 == 0 && std::gcd(N1, n / N1) == 1) {
            return N1;
        }
    }

    return 1;
}

// TODO: Избавиться от использования DFT
static void dft(const size_t n, ft_complex *data) {
    auto *temp = new ft_complex[n];
    for (size_t k = 0; k < n; ++k) {
        temp[k][0] = 0.0, temp[k][1] = 0.0;

        for (size_t m = 0; m < n; ++m) {
            ft_complex w;
            FT_POLAR(-2.0 * std::numbers::pi * static_cast<double>(m * k) / static_cast<double>(n), w);
            FT_RMUL(w, data[m]);
            FT_RADD(temp[k], w);
        }
    }

    for (size_t k = 0; k < n; ++k) {
        FT_COPY(temp[k], data[k]);
    }

    delete[] temp;
}

// TODO: Нет многотопоточности
static void fft(const size_t n, const size_t n1, const size_t n2, const ft_complex *in, ft_complex *out) {
    auto *transposed = new ft_complex[n];

    for (size_t k1 = 0; k1 < n1; ++k1) {
        for (size_t k2 = 0; k2 < n2; ++k2) {
            FT_COPY(in[n1 * k2 + k1], out[k1 * n2 + k2]);
        }
    }

    for (size_t k1 = 0; k1 < n1; ++k1) {
        dft(n2, out + k1 * n2);
    }

    for (size_t k1 = 0; k1 < n1; ++k1) {
        for (size_t k2 = 0; k2 < n2; ++k2) {
            ft_complex w;
            FT_POLAR(-2.0 * std::numbers::pi * static_cast<double>(k1 * k2) / static_cast<double>(n), w);
            FT_MUL(w, out[k1 * n2 + k2], transposed[k2 * n1 + k1]);
        }
    }

    for (size_t k2 = 0; k2 < n2; ++k2) {
        dft(n1, transposed + k2 * n1);
    }

    for (size_t k1 = 0; k1 < n1; ++k1) {
        for (size_t k2 = 0; k2 < n2; ++k2) {
            FT_COPY(transposed[k2 * n1 + k1], out[n2 * k1 + k2]);
        }
    }

    delete[] transposed;
}

void GoodThomas_I::forward(const size_t n, ft_complex *in, ft_complex *out) {
    const size_t n1 = find_optimal_coprime(n);
    fft(n, n1, n / n1, in, out);
}
