#pragma once

#include <chrono>

namespace benchmark
{
    template <typename F>
    std::chrono::duration<double> measure(F&& f, const size_t count = 1)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < count; i++)
        {
            f();
        }
        const auto finish = std::chrono::high_resolution_clock::now();

        return (finish - start) / count;
    }

    template <typename F>
    std::chrono::duration<double> measure_auto(F&& f, const size_t batch_size, const double precision = 0.01, const size_t threshold = 10)
    {
        size_t runs = 0;
        size_t under_err = 0;
        double err = 0;
        std::chrono::duration<double> avg(0);

        while (under_err < threshold)
        {
            runs++;
            auto time = measure(f, batch_size);
            auto new_avg = (avg * (runs - 1) + time) / runs;
            err = (avg - new_avg) / new_avg;
            avg = new_avg;

            if (abs(err) <= precision)
                under_err++;
            else
                under_err = 0;
        }

        return avg;
    }
}