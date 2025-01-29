#ifndef RADIX2FFT_H
#define RADIX2FFT_H
#include "algorithm/IFourierTransformAlgorithm.h"


class Radix2FFT : public IFourierTransformAlgorithm {
public:
    explicit Radix2FFT(const std::string &name) : IFourierTransformAlgorithm(name) {
    }

protected:
    static void check_preconditions(size_t n, ft_complex *in, ft_complex *out);
};


#endif
