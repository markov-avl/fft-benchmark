#include "CooleyTukey_I_BRP_R2.h"

#include "algorithm/utils/brp.h"
#include "algorithm/utils/operation.h"


static void fft(const size_t n, ft_complex *data) {
    for (size_t step = 2; step <= n; step *= 2) {
        const size_t half = step / 2;

        for (ft_complex *group = data; group < data + n; group += step) {
            ft_complex t;

            for (size_t k = 0; k < half; ++k) {
                ft_polar(-std::numbers::pi_v<double> * static_cast<double>(k) / static_cast<double>(half), t);
                ft_mul(t, group[half + k]);
                ft_sub(group[k], t, group[half + k]);
                ft_add(group[k], t);
            }
        }
    }
}


void CooleyTukey_I_BRP_R2::forward(const size_t n, ft_complex *in, ft_complex *out) {
    bit_reversal_permutation(n, in, out);
    fft(n, out);
}
