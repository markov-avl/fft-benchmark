#include "Stockham_I_Async_R4.h"

#include <cmath>
#include <future>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n,
                const size_t s,
                const bool eo,
                ft_complex *x,
                ft_complex *y,
                const size_t task_count = 1) {
    if (n == 1) {
        if (eo) {
            FT_ARRAY_COPY(s, x, y);
        }
        return;
    }

    constexpr ft_complex j = {0.0, 1.0};
    const size_t n1 = n / 4;
    const size_t n2 = n / 2;
    const size_t n3 = n1 + n2;
    const size_t tasks = std::min(task_count, n1);
    const double theta = std::numbers::pi / static_cast<double>(n2);
    std::vector<std::future<void> > futures;

    for (size_t t = 0; t < tasks; ++t) {
        futures.push_back(std::async(std::launch::async, [&, t] {
            const size_t p_start = t * n1 / tasks;
            const size_t p_end = t == tasks - 1 ? n1 : (t + 1) * n1 / tasks;

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
        }));
    }

    auto task = std::async(std::launch::async, [&] {
        fft(n1, 4 * s, !eo, y, x, std::max(1ul, task_count / 4));
    });

    for (auto &f: futures) {
        f.get();
    }
    task.get();
}

void Stockham_I_Async_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, 1, false, in, out, get_max_threads());
    FT_ARRAY_COPY(n, in, out);
}
