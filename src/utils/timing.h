#pragma once

#include <chrono>

namespace timing
{
    using clock = std::chrono::high_resolution_clock;
    using duration_ms = std::chrono::duration<double, std::milli>;

    template <typename F>
    std::chrono::duration<double> measure(F&& f)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        f();
        const auto finish = std::chrono::high_resolution_clock::now();

        return finish - start;
    }

    template <typename F>
    double measure_ms(F&& f)
    {
        const auto duration = measure(std::move(f));
        return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(duration).count();
    }

    void sleep_for_precise(duration_ms duration);
    void sleep_until_precise(clock::time_point time_point);
}