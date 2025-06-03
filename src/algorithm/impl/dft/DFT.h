#pragma once

#include "algorithm/IFourierTransformAlgorithm.h"


class DFT final : public IFourierTransformAlgorithm {
public:
    static constexpr auto NAME = "DFT";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
