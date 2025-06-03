#pragma once

#include "IOutputCollector.h"


class CsvOutputCollector final : public IOutputCollector {
    int current_column = 0;
    std::string file_path;

    void append_delimiter(std::ofstream &file) const;

public:
    explicit CsvOutputCollector(const std::string &file_path);

    void add(const std::string &value) override;

    void add(double value, int precision) override;

    void newline() override;
};
