#pragma once

#include "../radix2/Radix2FFT.h"


class GoodThomas_I final : public IFourierTransformAlgorithm {
public:
    static constexpr auto NAME = "GT-I";

    [[nodiscard]] int supported_sequences() const override {
        return COPRIME;
    }

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
