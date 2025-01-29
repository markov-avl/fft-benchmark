#include <utility>
#include "CompositeOutputCollector.h"


CompositeOutputCollector::CompositeOutputCollector(std::vector<std::shared_ptr<IOutputCollector> > collectors)
    : collectors(std::move(collectors)) {
}

void CompositeOutputCollector::add(const std::string &value) {
    for (const auto &collector: collectors) {
        collector->add(value);
    }
}

void CompositeOutputCollector::add(const double value) {
    for (const auto &collector: collectors) {
        collector->add(value);
    }
}

void CompositeOutputCollector::newline() {
    for (const auto &collector: collectors) {
        collector->newline();
    }
}
