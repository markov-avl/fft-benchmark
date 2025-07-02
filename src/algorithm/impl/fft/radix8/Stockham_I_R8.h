#pragma once

#include "Radix8FFT.h"


class Stockham_I_R8 final : public Radix8FFT {
public:
    static constexpr auto NAME = "S-I-8";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
