#pragma once

#include "algorithm/IFourierTransformAlgorithm.h"


class Radix2FFT : public IFourierTransformAlgorithm {
protected:
    void check_preconditions(size_t n, ft_complex *in, ft_complex *out) override;
};
