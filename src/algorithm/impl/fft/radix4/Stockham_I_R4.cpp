#include "Stockham_I_R4.h"

#include <cmath>

#include "algorithm/utils/operation.h"


static const ft_complex j = {0.0, 1.0};

static void fft(const size_t n, const size_t s, const bool eo, ft_complex *x, ft_complex *y) {
    const size_t n1 = n / 4;
    const size_t n2 = n / 2;
    const size_t n3 = n1 + n2;
    const double theta = std::numbers::pi / static_cast<double>(n2);

    if (n == 1) {
        if (eo) {
            for (size_t q = 0; q < s; ++q) {
                ft_copy(x[q], y[q]);
            }
        }
    } else if (n == 2) {
        ft_complex a, b;

        for (size_t q = 0; q < s; ++q) {
            ft_copy(x[q + 0], a);
            ft_copy(x[q + s], b);
            ft_add(a, b, y[q]);
            ft_sub(a, b, y[q + s]);
        }

        fft(1, 2 * s, !eo, y, x);
    } else if (n > 2) {
        ft_complex w1, w2, w3, a, b, c, d, apc, amc, bpd, jbmd;

        for (size_t p = 0; p < n1; ++p) {
            const double angle = static_cast<double>(p) * theta;
            w1[0] = std::cos(angle), w1[1] = -std::sin(angle);
            ft_mul(w1, w1, w2);
            ft_mul(w1, w2, w3);

            for (size_t q = 0; q < s; ++q) {
                ft_copy(x[q + s * p], a);
                ft_copy(x[q + s * (p + n1)], b);
                ft_copy(x[q + s * (p + n2)], c);
                ft_copy(x[q + s * (p + n3)], d);

                ft_add(a, c, apc);
                ft_sub(a, c, amc);
                ft_add(b, d, bpd);
                ft_sub(b, d, jbmd);
                ft_mul(jbmd, j);

                ft_add(apc, bpd, y[q + s * (4 * p)]);
                ft_sub(amc, jbmd, y[q + s * (4 * p + 1)]);
                ft_mul(y[q + s * (4 * p + 1)], w1);
                ft_sub(apc, bpd, y[q + s * (4 * p + 2)]);
                ft_mul(y[q + s * (4 * p + 2)], w2);
                ft_add(amc, jbmd, y[q + s * (4 * p + 3)]);
                ft_mul(y[q + s * (4 * p + 3)], w3);
            }
        }

        fft(n1, 4 * s, !eo, y, x);
    }
}

void Stockham_I_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, false, in, out);
    for (size_t i = 0; i < n; ++i) {
        ft_copy(in[i], out[i]);
    }
}
