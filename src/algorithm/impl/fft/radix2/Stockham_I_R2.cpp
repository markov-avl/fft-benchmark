#include "Stockham_I_R2.h"

#include <cmath>

#include "algorithm/utils/operation.h"


static void fft(const size_t n, const size_t s, const bool eo, ft_complex *x, ft_complex *y) {
    if (n == 2) {
        ft_complex *z = eo ? y : x;

        for (size_t q = 0; q < s; ++q) {
            ft_complex a, b;
            ft_copy(x[q], a);
            ft_copy(x[q + s], b);
            ft_add(a, b, z[q]);
            ft_sub(a, b, z[q + s]);
        }
    } else if (n > 2) {
        const size_t half = n / 2;
        const double theta = std::numbers::pi / static_cast<double>(half);

        for (size_t p = 0; p < half; ++p) {
            const double angle = static_cast<double>(p) * theta;
            ft_complex w = {std::cos(angle), -std::sin(angle)};

            for (size_t q = 0; q < s; ++q) {
                ft_complex a, b;
                ft_copy(x[q + s * p], a);
                ft_copy(x[q + s * (p + half)], b);
                ft_add(a, b, y[q + s * (2 * p)]);
                ft_sub(a, b, y[q + s * (2 * p + 1)]);
                ft_mul(y[q + s * (2 * p + 1)], w);
            }
        }

        fft(half, 2 * s, !eo, y, x);
    }
}

void Stockham_I_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, false, in, out);
    for (size_t i = 0; i < n; ++i) {
        ft_copy(in[i], out[i]);
    }
}
