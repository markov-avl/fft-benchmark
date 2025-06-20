#pragma once

#include "algorithm/IFourierTransformAlgorithm.h"
#include "algorithm/SupportedSequences.h"


class Bluestein_I_Stockham_R4 final : public IFourierTransformAlgorithm {
    size_t l = 0;
    ft_complex *temp = nullptr;
    ft_complex *u = nullptr;
    ft_complex *v = nullptr;

public:
    static constexpr auto NAME = "B-I+S-4";

    [[nodiscard]] int supported_sequences() const override {
        return ANY;
    }

protected:
    void initialize(size_t n, ft_complex *in, ft_complex *out) override;

    void finalize(size_t n, ft_complex *in, ft_complex *out) override;

    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
