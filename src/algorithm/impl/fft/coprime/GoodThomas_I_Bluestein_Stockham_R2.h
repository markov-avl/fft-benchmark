#pragma once

#include "algorithm/IFourierTransformAlgorithm.h"
#include "algorithm/SupportedSequences.h"


class GoodThomas_I_Bluestein_Stockham_R2 final : public IFourierTransformAlgorithm {
    size_t n1 = 1;
    size_t n2 = 1;

public:
    static constexpr auto NAME = "GT-I+B+S-2";

    [[nodiscard]] int supported_sequences() const override {
        return COPRIME;
    }

protected:
    void check_preconditions(size_t n, ft_complex *in, ft_complex *out) override;

    void initialize(size_t n, ft_complex *in, ft_complex *out) override;

    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
