#include "algorithm.h"
#include "RecursiveSteppedRadix2FFT.h"


static void fft(const size_t n, const ft_complex *in, ft_complex *out, const size_t step = 1) {
    if (n == 1) {
        ft_copy(in[0], out[0]);
        return;
    }

    const size_t half = n / 2;
    fft(half, in, out, step * 2);
    fft(half, in + step, out + half, step * 2);

    transform(half, out);
}

void RecursiveSteppedRadix2FFT::forward(size_t n, ft_complex *in, ft_complex *out) {
    fft(n, in, out, 1);
}
