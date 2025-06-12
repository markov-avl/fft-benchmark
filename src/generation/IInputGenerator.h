#pragma once

#include <cstddef>

#include "types.h"


class IInputGenerator {
public:
    virtual ~IInputGenerator() = default;

    virtual void fill(ft_complex *in, std::size_t size) = 0;
};
