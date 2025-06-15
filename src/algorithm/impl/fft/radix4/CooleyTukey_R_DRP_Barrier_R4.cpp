#include "CooleyTukey_R_DRP_Barrier_R4.h"

#include <barrier>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/drp.h"
#include "algorithm/utils/operation.h"


static void transform(const size_t quarter, ft_complex *data, const size_t from, const size_t to) {
    for (size_t k = from; k < to; ++k) {
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

static void fft(const size_t n, ft_complex *data, const size_t thread_count = 1) {
    if (n == 1) {
        return;
    }

    const size_t quarter = n / 4;

    if (thread_count > 3) {
        std::barrier barrier(4);
        std::thread t1([&] {
            fft(quarter, data, thread_count / 4);
            barrier.arrive_and_wait();
            transform(quarter, data, 0, quarter / 4);
        });
        std::thread t2([&] {
            fft(quarter, data + quarter, thread_count / 4);
            barrier.arrive_and_wait();
            transform(quarter, data, quarter / 4, quarter / 2);
        });
        std::thread t3([&] {
            fft(quarter, data + 2 * quarter, thread_count / 4);
            barrier.arrive_and_wait();
            transform(quarter, data, quarter / 2, 3 * quarter / 4);
        });

        fft(quarter, data + 3 * quarter, thread_count / 4);
        barrier.arrive_and_wait();
        transform(quarter, data, 3 * quarter / 4, quarter);

        t1.join();
        t2.join();
        t3.join();

        return;
    }

    fft(quarter, data);
    fft(quarter, data + quarter);
    fft(quarter, data + 2 * quarter);
    fft(quarter, data + 3 * quarter);
    transform(quarter, data, 0, quarter);
}


void CooleyTukey_R_DRP_Barrier_R4::forward(const size_t n, ft_complex *in, ft_complex *out) {
    digit_reversal_permutation(4, n, in, out);
    fft(n, out, get_max_threads());
}
