#include "Bluestein_I_Stockham_R4.h"

#include <cmath>
#include <thread>

#include "algorithm/utils/operation.h"


static void stockham_forward(const size_t n, const size_t s, const bool eo, ft_complex *x, ft_complex *y) {
    if (n == 1) {
        if (eo) {
            FT_ARRAY_COPY(s, x, y);
        }
        return;
    }

    const size_t n1 = n / 4;
    const size_t n2 = n / 2;
    const size_t n3 = n1 + n2;
    const double theta = std::numbers::pi / static_cast<double>(n2);

    for (size_t p = 0; p < n1; ++p) {
        const double angle = static_cast<double>(p) * theta;
        const ft_complex w1 = {std::cos(angle), -std::sin(angle)};
        ft_complex w2, w3;
        FT_MUL(w1, w1, w2);
        FT_MUL(w1, w2, w3);

        for (size_t q = 0; q < s; ++q) {
            constexpr ft_complex j = {0.0, 1.0};
            ft_complex a, b, c, d, apc, amc, bpd, jbmd;

            FT_COPY(x[q + s * p], a);
            FT_COPY(x[q + s * (p + n1)], b);
            FT_COPY(x[q + s * (p + n2)], c);
            FT_COPY(x[q + s * (p + n3)], d);

            FT_ADD(a, c, apc);
            FT_SUB(a, c, amc);
            FT_ADD(b, d, bpd);
            FT_SUB(b, d, jbmd);
            FT_RMUL(jbmd, j);

            FT_ADD(apc, bpd, y[q + s * (4 * p)]);
            FT_SUB(amc, jbmd, y[q + s * (4 * p + 1)]);
            FT_RMUL(y[q + s * (4 * p + 1)], w1);
            FT_SUB(apc, bpd, y[q + s * (4 * p + 2)]);
            FT_RMUL(y[q + s * (4 * p + 2)], w2);
            FT_ADD(amc, jbmd, y[q + s * (4 * p + 3)]);
            FT_RMUL(y[q + s * (4 * p + 3)], w3);
        }
    }

    stockham_forward(n1, 4 * s, !eo, y, x);
}

static void stockham_inverse(const size_t n, ft_complex *x, ft_complex *y) {
    for (size_t i = 0; i < n; ++i) {
        FT_RCONJ(x[i]);
    }

    stockham_forward(n, 1, false, x, y);

    for (size_t i = 0; i < n; ++i) {
        x[i][0] = x[i][0] / n;
        x[i][1] = -x[i][1] / n;
    }
}

void Bluestein_I_Stockham_R4::initialize(const size_t n, ft_complex *in, ft_complex *out) {
    const size_t k = 4 * n - 1;
    for (l = 1; l < k; l <<= 2) {
    }

    temp = new ft_complex[l];
    u = new ft_complex[l];
    v = new ft_complex[l];
}

void Bluestein_I_Stockham_R4::finalize(const size_t n, ft_complex *in, ft_complex *out) {
    delete[] temp;
    delete[] u;
    delete[] v;
}

// TODO: Нет многопоточности
void Bluestein_I_Stockham_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
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
