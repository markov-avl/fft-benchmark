#pragma once

#include "IInputGenerator.h"


class ImpulseInputGenerator final : public IInputGenerator {
public:
    static constexpr auto NAME = "impulse";

    void fill(ft_complex *in, size_t size) override;
};
