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
        ft_copy(in[0], out[0]);
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
        ft_complex h1, h2, h3;
        ft_polar(-0.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), h1);
        ft_polar(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), h2);
        ft_polar(-1.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), h3);

        ft_complex m, m1w, m2w, m3w;
        ft_copy(out[k], m);
        ft_mul(out[quarter + k], h1, m1w);
        ft_mul(out[2 * quarter + k], h2, m2w);
        ft_mul(out[3 * quarter + k], h3, m3w);

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
