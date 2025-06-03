#pragma once


class IMeasurer {
public:
    virtual ~IMeasurer() = default;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual double result() = 0;
};
