#include "Radix2FFT.h"

#include <stdexcept>


void Radix2FFT::check_preconditions(const size_t n, ft_complex *in, ft_complex *out) {
    IFourierTransformAlgorithm::check_preconditions(n, in, out);
    if (n & n - 1) {
        throw std::invalid_argument("n must be a power of 2");
    }
}
