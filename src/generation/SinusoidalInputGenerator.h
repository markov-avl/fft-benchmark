#pragma once

#include "IInputGenerator.h"


class SinusoidalInputGenerator final : public IInputGenerator {
public:
    static constexpr auto NAME = "sinusoid";

    void fill(ft_complex *in, size_t size) override;
};
