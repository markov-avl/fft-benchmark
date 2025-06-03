#pragma once

#include "IInputGenerator.h"


class ConstantInputGenerator final : public IInputGenerator {
public:
    static constexpr auto NAME = "constant";

    void fill(ft_complex *in, size_t size) override;
};
