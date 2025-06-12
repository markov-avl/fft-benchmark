#include "Radix4FFT.h"

#include <stdexcept>


void Radix4FFT::check_preconditions(const size_t n, ft_complex *in, ft_complex *out) {
    IFourierTransformAlgorithm::check_preconditions(n, in, out);
    if (n & n - 1 || !(n & 0x5555555555555555)) {
        throw std::invalid_argument("n must be a power of 4");
    }
}
