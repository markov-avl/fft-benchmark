#pragma once

#include <fftw3.h>

#include "algorithm/IFourierTransformAlgorithm.h"


class FFTWEstimate final : public IFourierTransformAlgorithm {
    fftw_plan plan{};

public:
    static constexpr auto NAME = "FFTW-E";

protected:
    void initialize(size_t n, ft_complex *in, ft_complex *out) override;

    void finalize(size_t n, ft_complex *in, ft_complex *out) override;

    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
