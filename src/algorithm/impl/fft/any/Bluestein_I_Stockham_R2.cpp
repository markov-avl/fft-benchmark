#include "Bluestein_I_Stockham_R2.h"

#include <cmath>
#include <thread>

#include "algorithm/utils/operation.h"


static void stockham_forward(const size_t n, const size_t s, const bool eo, ft_complex *x, ft_complex *y) {
    if (n == 2) {
        ft_complex *z = eo ? y : x;

        for (size_t q = 0; q < s; ++q) {
            ft_complex a, b;
            FT_COPY(x[q], a);
            FT_COPY(x[q + s], b);
            FT_ADD(a, b, z[q]);
            FT_SUB(a, b, z[q + s]);
        }
    } else if (n > 2) {
        const size_t half = n / 2;
        const double theta = std::numbers::pi / static_cast<double>(half);

        for (size_t p = 0; p < half; ++p) {
            const double angle = static_cast<double>(p) * theta;
            const ft_complex w = {std::cos(angle), -std::sin(angle)};

            for (size_t q = 0; q < s; ++q) {
                ft_complex a, b;
                FT_COPY(x[q + s * p], a);
                FT_COPY(x[q + s * (p + half)], b);
                FT_ADD(a, b, y[q + s * (2 * p)]);
                FT_SUB(a, b, y[q + s * (2 * p + 1)]);
                FT_RMUL(y[q + s * (2 * p + 1)], w);
            }
        }

        stockham_forward(half, 2 * s, !eo, y, x);
    }
}

static void stockham_inverse(const size_t n, ft_complex *x, ft_complex *y) {
    for (size_t i = 0; i < n; ++i) {
        FT_RCONJ(x[i]);
    }

    stockham_forward(n, 1, false, x, y);

    for (size_t i = 0; i < n; ++i) {
        x[i][0] = x[i][0] / static_cast<double>(n);
        x[i][1] = -x[i][1] / static_cast<double>(n);
    }
}

void Bluestein_I_Stockham_R2::initialize(const size_t n, ft_complex *in, ft_complex *out) {
    const size_t k = 2 * n - 1;
    for (l = 1; l < k; l <<= 1) {
    }

    temp = new ft_complex[l];
    u = new ft_complex[l];
    v = new ft_complex[l];
}

void Bluestein_I_Stockham_R2::finalize(const size_t n, ft_complex *in, ft_complex *out) {
    delete[] temp;
    delete[] u;
    delete[] v;
}

// TODO: Нет многопоточности
void Bluestein_I_Stockham_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    for (size_t i = 0; i < n; ++i) {
        FT_POLAR(-std::numbers::pi * i * (static_cast<double>(i) / n), out[i]);
        FT_MUL(in[i], out[i], u[i]);
        FT_CONJ(v[i], out[i]);
    }
    for (size_t i = n; i < l; ++i) {
        FT_ZERO(u[i]);
    }

    for (size_t i = 1; i < n; ++i) {
        FT_COPY(v[i], v[l - i]);
    }

    stockham_forward(l, 1, false, u, temp);
    stockham_forward(l, 1, false, v, temp);

    for (size_t i = 0; i < l; ++i) {
        FT_RMUL(u[i], v[i]);
    }

    stockham_inverse(l, u, temp);

    for (size_t i = 0; i < n; ++i) {
        FT_RMUL(out[i], u[i]);
    }
}
