#include "CooleyTukey_R_R2.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, const ft_complex *in, ft_complex *out, const size_t T = 1) {
    if (n == 1) {
        FT_COPY(in[0], out[0]);
        return;
    }

    const size_t half = n / 2;
    auto *even_in = new ft_complex[half];
    auto *odd_in = new ft_complex[half];
    auto *even_out = new ft_complex[half];
    auto *odd_out = new ft_complex[half];

    for (size_t i = 0; i < half; ++i) {
        FT_COPY(in[2 * i], even_in[i]);
        FT_COPY(in[2 * i + 1], odd_in[i]);
    }

    if (T > 1) {
        std::thread t(fft, half, odd_in, odd_out, T / 2);
        fft(half, even_in, even_out, T / 2);
        t.join();
    } else {
        fft(half, even_in, even_out);
        fft(half, odd_in, odd_out);
    }

    for (size_t k = 0; k < half; ++k) {
        ft_complex t;
        FT_POLAR(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(half), t);
        FT_RMUL(t, odd_out[k]);
        FT_ADD(even_out[k], t, out[k]);
        FT_SUB(even_out[k], t, out[k + half]);
    }

    delete[] even_in;
    delete[] odd_in;
    delete[] even_out;
    delete[] odd_out;
}


void CooleyTukey_R_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, in, out, get_max_threads());
}
