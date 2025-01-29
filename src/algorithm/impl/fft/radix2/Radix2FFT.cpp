#include <stdexcept>
#include "Radix2FFT.h"


void Radix2FFT::check_preconditions(const size_t n, ft_complex *in, ft_complex *out) {
    if ((n & n - 1) == 0) {
        throw std::invalid_argument("n must be a power of 2");
    }
}
