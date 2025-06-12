#include "CooleyTukey_R_R2.h"

#include <thread>

#include "multiprocessing.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, const ft_complex *in, ft_complex *out, const size_t thread_count = 1) {
    if (n == 1) {
        ft_copy(in[0], out[0]);
        return;
    }

    const size_t half = n / 2;
    auto *even_in = new ft_complex[half];
    auto *odd_in = new ft_complex[half];
    auto *even_out = new ft_complex[half];
    auto *odd_out = new ft_complex[half];

    for (size_t i = 0; i < half; ++i) {
        ft_copy(in[2 * i], even_in[i]);
        ft_copy(in[2 * i + 1], odd_in[i]);
    }

    if (thread_count > 1) {
        std::thread t(fft, half, odd_in, odd_out, thread_count / 2);
        fft(half, even_in, even_out, thread_count / 2);
        t.join();
    } else {
        fft(half, even_in, even_out);
        fft(half, odd_in, odd_out);
    }

    ft_complex t;
    for (size_t k = 0; k < half; ++k) {
        ft_polar(-std::numbers::pi * static_cast<double>(k) / static_cast<double>(half), t);
        ft_mul(t, odd_out[k]);
        ft_add(even_out[k], t, out[k]);
        ft_sub(even_out[k], t, out[k + half]);
    }

    delete[] even_in;
    delete[] odd_in;
    delete[] even_out;
    delete[] odd_out;
}


void CooleyTukey_R_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    fft(n, in, out, get_max_threads());
}
