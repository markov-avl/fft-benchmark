#include "GoodThomas_I.h"

#include <cmath>
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

// TODO: Нет многотопоточности
// TODO: В алгоритме зашито DFT
static void fft(const size_t n, const size_t n1, const size_t n2, const ft_complex *in, ft_complex *out) {
    auto *transposed = new ft_complex[n];

    for (size_t k1 = 0; k1 < n1; ++k1) {
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

    for (size_t k1 = 0; k1 < n1; ++k1) {
        for (size_t k2 = 0; k2 < n2; ++k2) {
            FT_COPY(transposed[k2 * n1], out[n2 * k1 + k2]);

            for (size_t m = 1; m < n1; ++m) {
                ft_complex w;
                FT_POLAR(-2.0 * std::numbers::pi * static_cast<double>(m * k1) / static_cast<double>(n1), w);
                FT_RMUL(w, transposed[k2 * n1 + m]);
                FT_RADD(out[n2 * k1 + k2], w);
            }
        }
    }

    delete[] transposed;
}

void GoodThomas_I::forward(const size_t n, ft_complex *in, ft_complex *out) {
    const size_t n1 = find_optimal_coprime(n);
    fft(n, n1, n / n1, in, out);
}
