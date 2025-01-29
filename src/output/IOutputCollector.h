#ifndef IOUTPUTCOLLECTOR_H
#define IOUTPUTCOLLECTOR_H
#include <string>


class IOutputCollector {
public:
    virtual ~IOutputCollector() = default;

    virtual void add(const std::string &value) = 0;

    virtual void add(double value) = 0;

    virtual void newline() = 0;
};


#endif
