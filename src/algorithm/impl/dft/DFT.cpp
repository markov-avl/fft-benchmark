#include <cmath>
#include "DFT.h"


void precompute_twiddles(const size_t n, ft_complex *twiddles) {
    for (size_t k = 0; k < n; ++k) {
        const double angle = -2.0 * std::numbers::pi_v<double> * static_cast<double>(k) / static_cast<double>(n);
        twiddles[k][0] = std::cos(angle);
        twiddles[k][1] = std::sin(angle);
    }
}

void DFT::forward(const size_t n, ft_complex *in, ft_complex *out) {
    auto *twiddles = new ft_complex[n];
    precompute_twiddles(n, twiddles);

    for (size_t k = 0; k < n; ++k) {
        double sum_r = 0.0;
        double sum_i = 0.0;

        for (size_t t = 0; t < n; ++t) {
            const size_t twiddle_index = t * k % n;
            const double W_r = twiddles[twiddle_index][0];
            const double W_i = twiddles[twiddle_index][1];

            const double in_r = in[t][0];
            const double in_i = in[t][1];

            sum_r += in_r * W_r - in_i * W_i;
            sum_i += in_r * W_i + in_i * W_r;
        }

        out[k][0] = sum_r;
        out[k][1] = sum_i;
    }

    delete[] twiddles;
}
