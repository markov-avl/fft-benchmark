#include "algorithm.h"
#include "IterativeRadix2FFT.h"


static void fft(const size_t n, ft_complex *data) {
    for (size_t step = 2; step <= n; step *= 2) {
        for (size_t group = 0; group < n; group += step) {
            transform(step / 2, &data[group]);
        }
    }
}

void IterativeRadix2FFT::forward(const size_t n, ft_complex *in, ft_complex *out) {
    bit_shuffle(n, in, out);
    fft(n, out);
}
