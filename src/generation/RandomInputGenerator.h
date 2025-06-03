#pragma once

#include "IInputGenerator.h"


class RandomInputGenerator final : public IInputGenerator {
public:
    static constexpr auto NAME = "random";

    void fill(ft_complex *in, size_t size) override;
};
