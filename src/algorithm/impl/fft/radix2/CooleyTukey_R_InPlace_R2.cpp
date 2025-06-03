#include "CooleyTukey_R_InPlace_R2.h"

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

    const size_t half = n / 2;

    if (thread_count > 1) {
        std::thread t(fft, half, in + step, out + half, step * 2, thread_count / 2);
        fft(half, in, out, step * 2, thread_count / 2);
        t.join();
    } else {
        fft(half, in, out, step * 2);
        fft(half, in + step, out + half, step * 2);
    }

    ft_complex t;
    for (size_t k = 0; k < half; ++k) {
        ft_polar(-std::numbers::pi_v<double> * static_cast<double>(k) / static_cast<double>(half), t);
        ft_mul(t, out[half + k]);
        ft_sub(out[k], t, out[half + k]);
        ft_add(out[k], t);
    }
}


void CooleyTukey_R_InPlace_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, in, out, 1, get_max_threads());
}
