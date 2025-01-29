#ifndef STDOUTPUTCOLLECTOR_H
#define STDOUTPUTCOLLECTOR_H
#include "IOutputCollector.h"


class StdOutputCollector final : public IOutputCollector {
    std::size_t num_columns;
    int column_width;
    bool header = false;

public:
    StdOutputCollector(std::size_t num_columns, int column_width, bool header);

    void add(const std::string &value) override;

    void add(double value) override;

    void newline() override;
};


#endif
