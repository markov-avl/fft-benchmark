#pragma once

#include <string>
#include <vector>

#include "IMeasurer.h"


class IntelCPUPowerMeter final : public IMeasurer {
    std::vector<std::string> energy_paths;
    std::vector<double> start_energies;
    std::vector<double> stop_energies;

public:
    static constexpr auto NAME = "intel-cpu-power";

    IntelCPUPowerMeter();

    void start() override;

    void stop() override;

    [[nodiscard]] double result() override;
};
