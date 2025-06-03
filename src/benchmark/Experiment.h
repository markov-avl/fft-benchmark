#pragma once

#include <memory>

#include "algorithm.h"
#include "types.h"


class Experiment final {
    std::shared_ptr<IFourierTransformAlgorithm> algorithm;
    std::shared_ptr<IMeasurer> measurer;
    size_t runs;
    size_t n;
    ft_complex *in;

public:
    Experiment(const std::shared_ptr<IFourierTransformAlgorithm> &algorithm,
               const std::shared_ptr<IMeasurer> &measurer,
               size_t runs,
               size_t n,
               ft_complex *in);

    ~Experiment() = default;

    [[nodiscard]] double run() const;
};
