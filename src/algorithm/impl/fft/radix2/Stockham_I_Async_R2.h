#pragma once

#include "Radix2FFT.h"


class Stockham_I_Async_R2 final : public Radix2FFT {
public:
    static constexpr auto NAME = "S-I+A-2";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
