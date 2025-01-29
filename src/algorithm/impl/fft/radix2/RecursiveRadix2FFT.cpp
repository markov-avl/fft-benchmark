#include "algorithm.h"
#include "RecursiveRadix2FFT.h"


static void fft(const size_t n, ft_complex *data) {
    if (n == 8) {
        fft_of_8(data);
        return;
    }
    if (n == 4) {
        fft_of_4(data);
        return;
    }
    if (n == 2) {
        fft_of_2(data);
        return;
    }
    if (n == 1) {
        return;
    }

    const size_t half = n / 2;
    fft(half, data);
    fft(half, data + half);

    transform(half, data);
}

void RecursiveRadix2FFT::forward(const size_t n, ft_complex *in, ft_complex *out) {
    bit_shuffle(n, in, out);
    fft(n, out);
}
