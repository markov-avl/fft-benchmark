#include "CooleyTukey_R_Stride_R2.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n,
                const ft_complex *in,
                ft_complex *out,
                const size_t step = 1,
                const size_t T = 1) {
    if (n == 1) {
        FT_COPY(in[0], out[0]);
        return;
    }

    const size_t half = n / 2;

    if (T > 1) {
        std::thread t(fft, half, in + step, out + half, step << 1, T / 2);
        fft(half, in, out, step << 1, T / 2);
        t.join();
    } else {
        fft(half, in, out, step << 1);
        fft(half, in + step, out + half, step << 1);
    }

    for (size_t k = 0; k < half; ++k) {
        ft_complex t;
        FT_POLAR(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(half), t);
        FT_RMUL(t, out[half + k]);
        FT_SUB(out[k], t, out[half + k]);
        FT_RADD(out[k], t);
    }
}


void CooleyTukey_R_Stride_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, in, out, 1, get_max_threads());
}
