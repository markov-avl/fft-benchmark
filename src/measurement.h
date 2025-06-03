#pragma once

#include <map>
#include <memory>
#include <ranges>

#include "measurement/IMeasurer.h"
#include "measurement/IntelCPUPowerMeter.h"
#include "measurement/Timer.h"


static const std::map<std::string, std::shared_ptr<IMeasurer> > MEASURERS = {
    {IntelCPUPowerMeter::NAME, std::make_shared<IntelCPUPowerMeter>()},
    {Timer::NAME, std::make_shared<Timer>()}
};

inline std::vector<std::string> supported_measurers() {
    std::vector<std::string> measurer_names;
    for (const auto &name: MEASURERS | std::views::keys) {
        measurer_names.emplace_back(name);
    }
    return measurer_names;
}
