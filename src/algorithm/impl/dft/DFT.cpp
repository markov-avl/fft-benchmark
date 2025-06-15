#include "DFT.h"

#include <numbers>

#include "algorithm/utils/operation.h"


void precompute_twiddles(const size_t n, ft_complex *twiddles) {
    for (size_t t = 0; t < n; ++t) {
        ft_polar(-2.0 * std::numbers::pi * static_cast<double>(t) / static_cast<double>(n), twiddles[t]);
    }
}

void DFT::forward(const size_t n, ft_complex *in, ft_complex *out) {
    auto *twiddles = new ft_complex[n];
    precompute_twiddles(n, twiddles);

    ft_complex sum;
    for (size_t k = 0; k < n; ++k) {
        out[k][0] = 0.0, out[k][1] = 0.0;

        for (size_t t = 0; t < n; ++t) {
            ft_mul(twiddles[t * k % n], in[t], sum);
            ft_add(out[k], sum);
        }
    }

    delete[] twiddles;
}
