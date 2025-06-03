#include "CompositeOutputCollector.h"

#include <utility>


CompositeOutputCollector::CompositeOutputCollector(const std::vector<std::shared_ptr<IOutputCollector> > &collectors)
    : collectors(collectors) {
}

void CompositeOutputCollector::add(const std::string &value) {
    for (const auto &collector: collectors) {
        collector->add(value);
    }
}

void CompositeOutputCollector::add(const double value, const int precision) {
    for (const auto &collector: collectors) {
        collector->add(value, precision);
    }
}

void CompositeOutputCollector::newline() {
    for (const auto &collector: collectors) {
        collector->newline();
    }
}
