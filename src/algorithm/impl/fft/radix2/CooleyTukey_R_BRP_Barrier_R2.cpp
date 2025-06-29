#include "CooleyTukey_R_BRP_Barrier_R2.h"

#include <barrier>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/brp.h"
#include "algorithm/utils/operation.h"


static void transform(const size_t half, ft_complex *data, const size_t from, const size_t to) {
    for (size_t k = from; k < to; ++k) {
        ft_complex t;
        FT_POLAR(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(half), t);
        FT_RMUL(t, data[half + k]);
        FT_SUB(data[k], t, data[half + k]);
        FT_RADD(data[k], t);
    }
}

static void fft(const size_t n, ft_complex *data, const size_t T = 1) {
    if (n == 1) {
        return;
    }

    const size_t half = n / 2;

    if (T > 1) {
        std::barrier barrier(2);
        const size_t batch_size = half / 2;
        auto task = [&](const size_t t) {
            fft(half, data + t * half, T / 2);
            barrier.arrive_and_wait();
            transform(half, data, t * batch_size, (t + 1) * batch_size);
        };

        std::thread t(task, 0);
        task(1);

        t.join();

        return;
    }

    fft(half, data);
    fft(half, data + half);
    transform(half, data, 0, half);
}


void CooleyTukey_R_BRP_Barrier_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    bit_reversal_permutation(n, in, out);
    fft(n, out, get_max_threads());
}
