#include "IFourierTransformAlgorithm.h"

#include <stdexcept>


void IFourierTransformAlgorithm::run(const size_t n, ft_complex *in, ft_complex *out) {
    check_preconditions(n, in, out);
    initialize(n, in, out);
    forward(n, in, out);
    finalize(n, in, out);
}

void IFourierTransformAlgorithm::run(const size_t n, ft_complex *in, ft_complex *out, IMeasurer *measurer) {
    check_preconditions(n, in, out);
    initialize(n, in, out);

    measurer->start();
    forward(n, in, out);
    measurer->stop();

    finalize(n, in, out);
}

void IFourierTransformAlgorithm::check_preconditions(const size_t n, ft_complex *in, ft_complex *out) {
    if (n < 1) {
        throw std::invalid_argument("n must be greater than 0");
    }
}

void IFourierTransformAlgorithm::initialize(const size_t n, ft_complex *in, ft_complex *out) {
}

void IFourierTransformAlgorithm::finalize(const size_t n, ft_complex *in, ft_complex *out) {
}
