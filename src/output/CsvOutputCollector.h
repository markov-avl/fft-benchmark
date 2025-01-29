#ifndef CSVOUTPUTCOLLECTOR_H
#define CSVOUTPUTCOLLECTOR_H
#include "IOutputCollector.h"


class CsvOutputCollector final : public IOutputCollector {
    std::string file_path;

public:
    explicit CsvOutputCollector(std::string file_path);

    void add(const std::string &value) override;

    void add(double value) override;

    void newline() override;
};


#endif
