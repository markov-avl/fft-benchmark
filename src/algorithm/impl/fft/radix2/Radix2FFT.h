#pragma once

#include "algorithm/IFourierTransformAlgorithm.h"
#include "algorithm/SupportedSequences.h"


class Radix2FFT : public IFourierTransformAlgorithm {
public:
    [[nodiscard]] int supported_sequences() const override {
        return BASE_OF_2;
    }

protected:
    void check_preconditions(size_t n, ft_complex *in, ft_complex *out) override;
};
