#pragma once

#include <chrono>

namespace timing
{
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

    void sleep_for_precise(duration_ms duration)
    {
        static constexpr duration_ms min_sleep_duration(0);
        using clock = std::chrono::high_resolution_clock;

        clock::time_point start = clock::now();
        while (duration_ms(clock::now() - start) < duration)
        {
            std::this_thread::sleep_for(min_sleep_duration);
        }
    }
}