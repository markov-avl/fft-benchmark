#include "Stockham_I_R4.h"

#include <cmath>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n,
                const size_t s,
                const bool eo,
                ft_complex *x,
                ft_complex *y,
                const size_t thread_count = 1) {
    if (n == 1) {
        if (eo) {
            for (size_t q = 0; q < s; ++q) {
                FT_COPY(x[q], y[q]);
            }
        }
        return;
    }

    constexpr ft_complex j = {0.0, 1.0};
    const size_t n1 = n / 4;
    const size_t n2 = n / 2;
    const size_t n3 = n1 + n2;
    const double theta = std::numbers::pi / static_cast<double>(n2);

    auto task = [&](const size_t t) {
        const size_t p_start = t * n1 / thread_count;
        const size_t p_end = t == thread_count - 1 ? n1 : (t + 1) * n1 / thread_count;

        for (size_t p = p_start; p < p_end; ++p) {
            const double angle = static_cast<double>(p) * theta;
            const ft_complex w1 = {std::cos(angle), -std::sin(angle)};
            ft_complex w2, w3;
            FT_MUL(w1, w1, w2);
            FT_MUL(w1, w2, w3);

            for (size_t q = 0; q < s; ++q) {
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
    };

    std::vector<std::thread> threads;

    for (size_t t = 1; t < thread_count; ++t) {
        threads.emplace_back(task, t);
    }
    task(0);

    for (auto &t: threads) {
        t.join();
    }

    fft(n1, 4 * s, !eo, y, x);
}

void Stockham_I_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, false, in, out, get_max_threads());
    for (size_t i = 0; i < n; ++i) {
        FT_COPY(in[i], out[i]);
    }
}
