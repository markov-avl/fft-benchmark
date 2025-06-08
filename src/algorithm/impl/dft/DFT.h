#pragma once

#include "algorithm/IFourierTransformAlgorithm.h"
#include "algorithm/SupportedSequences.h"


class DFT final : public IFourierTransformAlgorithm {
public:
    static constexpr auto NAME = "DFT";

    [[nodiscard]] int supported_sequences() const override {
        return ANY;
    }

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
