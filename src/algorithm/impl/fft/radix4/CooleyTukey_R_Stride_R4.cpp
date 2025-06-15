#include "CooleyTukey_R_Stride_R4.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n,
                const ft_complex *in,
                ft_complex *out,
                const size_t step = 1,
                const size_t thread_count = 1) {
    if (n == 1) {
        FT_COPY(in[0], out[0]);
        return;
    }

    const size_t quarter = n / 4;

    if (thread_count > 3) {
        std::thread t1(fft, quarter, in + step, out + quarter, step << 2, thread_count / 4);
        std::thread t2(fft, quarter, in + 2 * step, out + 2 * quarter, step << 2, thread_count / 4);
        std::thread t3(fft, quarter, in + 3 * step, out + 3 * quarter, step << 2, thread_count / 4);
        fft(quarter, in, out, step << 2, thread_count / 4);

        t1.join();
        t2.join();
        t3.join();
    } else {
        fft(quarter, in, out, step << 2);
        fft(quarter, in + step, out + quarter, step << 2);
        fft(quarter, in + 2 * step, out + 2 * quarter, step << 2);
        fft(quarter, in + 3 * step, out + 3 * quarter, step << 2);
    }

    for (size_t k = 0; k < quarter; ++k) {
        ft_complex w1, w2, w3, m, m1w, m2w, m3w;
        FT_POLAR(-0.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w1);
        FT_POLAR(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w2);
        FT_POLAR(-1.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w3);

        FT_COPY(out[k], m);
        FT_MUL(out[quarter + k], w1, m1w);
        FT_MUL(out[2 * quarter + k], w2, m2w);
        FT_MUL(out[3 * quarter + k], w3, m3w);

        out[k][0] += m1w[0] + m2w[0] + m3w[0];
        out[k][1] += m1w[1] + m2w[1] + m3w[1];
        out[quarter + k][0] = m[0] + m1w[1] - m2w[0] - m3w[1];
        out[quarter + k][1] = m[1] - m1w[0] - m2w[1] + m3w[0];
        out[2 * quarter + k][0] = m[0] - m1w[0] + m2w[0] - m3w[0];
        out[2 * quarter + k][1] = m[1] - m1w[1] + m2w[1] - m3w[1];
        out[3 * quarter + k][0] = m[0] - m1w[1] - m2w[0] + m3w[1];
        out[3 * quarter + k][1] = m[1] + m1w[0] - m2w[1] - m3w[0];
    }
}


void CooleyTukey_R_Stride_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, in, out, 1, get_max_threads());
}
