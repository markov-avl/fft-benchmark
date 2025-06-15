#include "DFT.h"

#include <numbers>

#include "algorithm/utils/operation.h"


void precompute_twiddles(const size_t n, ft_complex *twiddles) {
    for (size_t t = 0; t < n; ++t) {
        FT_POLAR(-2.0 * std::numbers::pi * static_cast<double>(t) / static_cast<double>(n), twiddles[t]);
    }
}

void DFT::forward(const size_t n, ft_complex *in, ft_complex *out) {
    auto *twiddles = new ft_complex[n];
    precompute_twiddles(n, twiddles);

    ft_complex sum;
    for (size_t k = 0; k < n; ++k) {
        FT_ZERO(out[k]);

        for (size_t t = 0; t < n; ++t) {
            FT_MUL(twiddles[t * k % n], in[t], sum);
            FT_RADD(out[k], sum);
        }
    }

    delete[] twiddles;
}
