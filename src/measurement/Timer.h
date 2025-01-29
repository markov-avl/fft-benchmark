#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include "IMeasurer.h"


class Timer final : public IMeasurer {
    std::chrono::time_point<std::chrono::high_resolution_clock> t0;
    std::chrono::time_point<std::chrono::high_resolution_clock> t1;

public:
    void start() override;

    void stop() override;

    [[nodiscard]] double result() override;
};

#endif
