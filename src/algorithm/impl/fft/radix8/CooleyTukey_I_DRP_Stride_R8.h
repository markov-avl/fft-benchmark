#pragma once

#include "Radix8FFT.h"


class CooleyTukey_I_DRP_Stride_R8 final : public Radix8FFT {
public:
    static constexpr auto NAME = "CT-I+DRP+S-8";

protected:
    void forward(size_t n, ft_complex *in, ft_complex *out) override;
};
