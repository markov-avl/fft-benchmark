#include "CooleyTukey_R_DRP_Barrier_R4.h"

#include <barrier>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/drp.h"
#include "algorithm/utils/operation.h"


static void transform(const size_t quarter, ft_complex *data, const size_t from, const size_t to) {
    for (size_t k = from; k < to; ++k) {
        ft_complex w1, w2, w3, m, m1w, m2w, m3w;
        FT_POLAR(-0.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w1);
        FT_POLAR(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w2);
        FT_POLAR(-1.5 * std::numbers::pi * static_cast<double>(k) / static_cast<double>(quarter), w3);

        FT_COPY(data[k], m);
        FT_MUL(data[quarter + k], w1, m1w);
        FT_MUL(data[2 * quarter + k], w2, m2w);
        FT_MUL(data[3 * quarter + k], w3, m3w);

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

static void fft(const size_t n, ft_complex *data, const size_t T = 1) {
    if (n == 1) {
        return;
    }

    const size_t quarter = n / 4;

    if (T > 3) {
        std::barrier barrier(4);
        const size_t batch_size = quarter / 4;
        auto task = [&](const size_t t) {
            fft(quarter, data + t * quarter, T / 4);
            barrier.arrive_and_wait();
            transform(quarter, data, t * batch_size, (t + 1) * batch_size);
        };

        std::thread t1(task, 0);
        std::thread t2(task, 1);
        std::thread t3(task, 2);
        task(3);

        t1.join();
        t2.join();
        t3.join();

        return;
    }
    if (T > 1) {
        std::barrier barrier(2);
        const size_t batch_size = quarter / 2;
        auto task = [&](const size_t t) {
            fft(quarter, data + t * quarter);
            fft(quarter, data + (t + 2) * quarter);
            barrier.arrive_and_wait();
            transform(quarter, data, t * batch_size, (t + 1) * batch_size);
        };

        std::thread t(task, 0);
        task(1);

        t.join();

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
