#pragma once

#include "Radix4FFT.h"


class Stockham_I_Async_R4 final : public Radix4FFT {
public:
    static constexpr auto NAME = "S-I+A-4";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
