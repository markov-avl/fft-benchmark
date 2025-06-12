#pragma once

#include <memory>
#include <vector>

#include "IOutputCollector.h"


class CompositeOutputCollector final : public IOutputCollector {
    std::vector<std::shared_ptr<IOutputCollector> > collectors;

public:
    explicit CompositeOutputCollector(const std::vector<std::shared_ptr<IOutputCollector> > &collectors);

    void add(const std::string &value) override;

    void add(double value, int precision) override;

    void newline() override;
};
