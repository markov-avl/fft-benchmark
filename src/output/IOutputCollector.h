#pragma once

#include <string>


class IOutputCollector {
public:
    virtual ~IOutputCollector() = default;

    virtual void add(const std::string &value) = 0;

    virtual void add(double value, int precision = 10) = 0;

    virtual void newline() = 0;
};
