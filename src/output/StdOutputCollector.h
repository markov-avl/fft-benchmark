#pragma once

#include <iomanip>

#include "IOutputCollector.h"


class StdOutputCollector final : public IOutputCollector {
    int current_column = 0;
    int column_width = 15;
    bool header = false;

    void print_delimiter() const;

    void apply_format() const;

public:
    explicit StdOutputCollector(int column_width = 15, bool header = false);

    void add(const std::string &value) override;

    void add(double value, int precision) override;

    void newline() override;
};
