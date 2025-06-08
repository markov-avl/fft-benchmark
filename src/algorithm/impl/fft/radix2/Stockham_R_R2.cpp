#include "Stockham_R_R2.h"

#include <cmath>

#include "algorithm/utils/brp.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, const size_t s, const size_t q, const bool eo, ft_complex *x, ft_complex *y) {
    if (n == 1) {
        if (eo) {
            ft_copy(y[q], x[q]);
        }
        return;
    }

    const size_t half = n / 2;
    const double theta = std::numbers::pi / static_cast<double>(half);

    ft_complex w, a, b;
    for (size_t p = 0; p < half; ++p) {
        const double angle = static_cast<double>(p) * theta;
        w[0] = std::cos(angle);
        w[1] = -std::sin(angle);

        ft_copy(x[q + s * p], a);
        ft_copy(x[q + s * (p + half)], b);
        ft_add(a, b, y[q + s * (2 * p)]);
        ft_sub(a, b, y[q + s * (2 * p + 1)]);
        ft_mul(y[q + s * (2 * p + 1)], w);
    }

    fft(half, 2 * s, q, !eo, y, x);
    fft(half, 2 * s, q + s, !eo, y, x);
}

void Stockham_R_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, 0, false, in, out);
    for (size_t i = 0; i < n; ++i) {
        ft_copy(in[i], out[i]);
    }
}
