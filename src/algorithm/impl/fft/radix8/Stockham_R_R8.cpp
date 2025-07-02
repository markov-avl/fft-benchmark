#include "Stockham_R_R8.h"

#include <cmath>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n,
                const size_t s,
                const size_t q,
                const bool eo,
                ft_complex *x,
                ft_complex *y,
                const size_t T = 1) {
    if (n == 1) {
        if (eo) {
            FT_COPY(x[q], y[q]);
        }
        return;
    }

    constexpr ft_complex j = {0.0, 1.0};
    const size_t n1 = n / 4;
    const size_t n2 = n / 2;
    const size_t n3 = n1 + n2;
    const double theta = std::numbers::pi / static_cast<double>(n2);

    for (size_t p = 0; p < n1; ++p) {
        const double angle = static_cast<double>(p) * theta;
        const ft_complex w1 = {std::cos(angle), -std::sin(angle)};
        ft_complex w2, w3, a, b, c, d, apc, amc, bpd, jbmd;

        FT_MUL(w1, w1, w2);
        FT_MUL(w1, w2, w3);

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

    if (T > 3) {
        std::thread t1(fft, n1, 4 * s, q, !eo, y, x, T / 4);
        std::thread t2(fft, n1, 4 * s, q + s, !eo, y, x, T / 4);
        std::thread t3(fft, n1, 4 * s, q + 2 * s, !eo, y, x, T / 4);
        fft(n1, 4 * s, q + 3 * s, !eo, y, x, T / 4);

        t1.join();
        t2.join();
        t3.join();
    } else if (T > 1) {
        auto task = [&](const size_t t) {
            fft(n1, 4 * s, q + t * s, !eo, y, x);
            fft(n1, 4 * s, q + (t + 2) * s, !eo, y, x);
        };

        std::thread t(task, 0);
        task(1);

        t.join();
    } else {
        fft(n1, 4 * s, q, !eo, y, x);
        fft(n1, 4 * s, q + s, !eo, y, x);
        fft(n1, 4 * s, q + 2 * s, !eo, y, x);
        fft(n1, 4 * s, q + 3 * s, !eo, y, x);
    }
}

void Stockham_R_R8::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, 0, false, in, out, get_max_threads());
    FT_ARRAY_COPY(n, in, out);
}
