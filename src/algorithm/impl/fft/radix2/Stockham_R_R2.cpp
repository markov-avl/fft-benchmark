#include "Stockham_R_R2.h"

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
                const size_t thread_count = 1) {
    if (n == 2) {
        ft_complex *z = eo ? y : x;

        ft_complex a, b;
        FT_COPY(x[q], a);
        FT_COPY(x[q + s], b);
        FT_ADD(a, b, z[q]);
        FT_SUB(a, b, z[q + s]);
    } else if (n > 2) {
        const size_t half = n / 2;
        const double theta = std::numbers::pi / static_cast<double>(half);

        for (size_t p = 0; p < half; ++p) {
            const double angle = static_cast<double>(p) * theta;
            const ft_complex w = {std::cos(angle), -std::sin(angle)};

            ft_complex a, b;
            FT_COPY(x[q + s * p], a);
            FT_COPY(x[q + s * (p + half)], b);
            FT_ADD(a, b, y[q + s * (2 * p)]);
            FT_SUB(a, b, y[q + s * (2 * p + 1)]);
            FT_RMUL(y[q + s * (2 * p + 1)], w);
        }

        if (thread_count > 1) {
            std::thread t1(fft, half, 2 * s, q, !eo, y, x, thread_count / 2);
            fft(half, 2 * s, q + s, !eo, y, x, thread_count / 2);

            t1.join();
        } else {
            fft(half, 2 * s, q, !eo, y, x);
            fft(half, 2 * s, q + s, !eo, y, x);
        }
    }
}

void Stockham_R_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, 0, false, in, out, get_max_threads());
    FT_ARRAY_COPY(n, in, out);
}
