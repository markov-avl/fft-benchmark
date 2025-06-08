#pragma once

#include "Radix2FFT.h"


class Stockham_R_R2 final : public Radix2FFT {
public:
    static constexpr auto NAME = "S-R-2";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
