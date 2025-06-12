#pragma once

#include "generation/ConstantInputGenerator.h"
#include "generation/IInputGenerator.h"
#include "generation/ImpulseInputGenerator.h"
#include "generation/RandomInputGenerator.h"
#include "generation/SinusoidalInputGenerator.h"


static const std::map<std::string, std::shared_ptr<IInputGenerator> > GENERATORS = {
    {ConstantInputGenerator::NAME, std::make_shared<ConstantInputGenerator>()},
    {ImpulseInputGenerator::NAME, std::make_shared<ImpulseInputGenerator>()},
    {RandomInputGenerator::NAME, std::make_shared<RandomInputGenerator>()},
    {SinusoidalInputGenerator::NAME, std::make_shared<SinusoidalInputGenerator>()},
};

inline std::vector<std::string> supported_generators() {
    std::vector<std::string> generator_names;
    for (const auto &name: GENERATORS | std::views::keys) {
        generator_names.emplace_back(name);
    }
    return generator_names;
}