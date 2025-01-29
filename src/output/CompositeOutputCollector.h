#ifndef COMPOSITEOUTPUTCOLLECTOR_H
#define COMPOSITEOUTPUTCOLLECTOR_H
#include <memory>
#include <vector>
#include "IOutputCollector.h"


class CompositeOutputCollector final : public IOutputCollector {
    std::vector<std::shared_ptr<IOutputCollector> > collectors;

public:
    explicit CompositeOutputCollector(std::vector<std::shared_ptr<IOutputCollector> > collectors);

    void add(const std::string &value) override;

    void add(double value) override;

    void newline() override;
};


#endif
