#include "FFTWEstimate.h"


void FFTWEstimate::initialize(const size_t n, ft_complex *in, ft_complex *out) {
    plan = fftw_plan_dft_1d(static_cast<int>(n), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
}

void FFTWEstimate::forward(size_t n, ft_complex *in, ft_complex *out) {
    fftw_execute(plan);
}

void FFTWEstimate::finalize(size_t n, ft_complex *in, ft_complex *out) {
    fftw_destroy_plan(plan);
}
