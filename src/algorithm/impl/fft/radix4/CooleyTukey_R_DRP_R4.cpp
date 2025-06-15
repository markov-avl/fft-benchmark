#include "CooleyTukey_R_DRP_R4.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/drp.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, ft_complex *data, const size_t thread_count = 1) {
    if (n == 1) {
        return;
    }

    const size_t quarter = n / 4;

    if (thread_count > 3) {
        std::thread t1(fft, quarter, data + quarter, thread_count / 4);
        std::thread t2(fft, quarter, data + 2 * quarter, thread_count / 4);
        std::thread t3(fft, quarter, data + 3 * quarter, thread_count / 4);
        fft(quarter, data);

        t1.join();
        t2.join();
        t3.join();
    } else {
        fft(quarter, data, thread_count / 4);
        fft(quarter, data + quarter, thread_count / 4);
        fft(quarter, data + 2 * quarter, thread_count / 4);
        fft(quarter, data + 3 * quarter, thread_count / 4);
    }

    for (size_t k = 0; k < quarter; ++k) {
        ft_complex h1, h2, h3;
        ft_polar(-0.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), h1);
        ft_polar(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), h2);
        ft_polar(-1.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), h3);

        ft_complex m, m1w, m2w, m3w;
        ft_copy(data[k], m);
        ft_mul(data[quarter + k], h1, m1w);
        ft_mul(data[2 * quarter + k], h2, m2w);
        ft_mul(data[3 * quarter + k], h3, m3w);

        data[k][0] += m1w[0] + m2w[0] + m3w[0];
        data[k][1] += m1w[1] + m2w[1] + m3w[1];
        data[quarter + k][0] = m[0] + m1w[1] - m2w[0] - m3w[1];
        data[quarter + k][1] = m[1] - m1w[0] - m2w[1] + m3w[0];
        data[2 * quarter + k][0] = m[0] - m1w[0] + m2w[0] - m3w[0];
        data[2 * quarter + k][1] = m[1] - m1w[1] + m2w[1] - m3w[1];
        data[3 * quarter + k][0] = m[0] - m1w[1] - m2w[0] + m3w[1];
        data[3 * quarter + k][1] = m[1] + m1w[0] - m2w[1] - m3w[0];
    }
}


void CooleyTukey_R_DRP_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    digit_reversal_permutation(4, n, in, out);
    fft(n, out, get_max_threads());
}
