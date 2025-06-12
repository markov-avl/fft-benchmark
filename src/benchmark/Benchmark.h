#pragma once

#include <memory>
#include <string>
#include <vector>

#include "algorithm.h"
#include "generation.h"


class Benchmark final {
    std::vector<std::string> algorithms;
    std::vector<size_t> sizes;
    std::shared_ptr<IMeasurer> measurer;
    std::shared_ptr<IInputGenerator> generator;
    size_t runs;

public:
    Benchmark(const std::vector<std::string> &algorithms,
              const std::vector<size_t> &sizes,
              const std::string &measurer,
              const std::string &generator,
              size_t runs);

    ~Benchmark() = default;

    void run(const std::string &output_file, size_t threads) const;
};
