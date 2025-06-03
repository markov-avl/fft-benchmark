#include "CooleyTukey_R_BRP_Barrier_R2.h"

#include <barrier>
#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/brp.h"
#include "algorithm/utils/operation.h"


static void transform(const size_t half, ft_complex *data, const size_t from, const size_t to) {
    ft_complex t;
    for (size_t k = from; k < to; ++k) {
        ft_polar(-std::numbers::pi_v<double> * static_cast<double>(k) / static_cast<double>(half), t);
        ft_mul(t, data[half + k]);
        ft_sub(data[k], t, data[half + k]);
        ft_add(data[k], t);
    }
}

static void fft(const size_t n, ft_complex *data, const size_t thread_count = 1) {
    if (n == 1) {
        return;
    }

    const size_t half = n / 2;

    if (thread_count > 1) {
        std::barrier barrier(2);
        std::thread t([&] {
            fft(half, data, thread_count / 2);
            barrier.arrive_and_wait();
            transform(half, data, 0, half / 2);
        });

        fft(half, data + half, thread_count / 2);
        barrier.arrive_and_wait();
        transform(half, data, half / 2, half);
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
