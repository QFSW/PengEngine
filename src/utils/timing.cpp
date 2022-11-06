#include "timing.h"

namespace timing
{
    void sleep_for_precise(duration_ms duration)
    {
        static constexpr duration_ms min_sleep_duration(0);
        clock::time_point start = clock::now();

        while (duration_ms(clock::now() - start) < duration)
        {
            std::this_thread::sleep_for(min_sleep_duration);
        }
    }

    void sleep_until_precise(clock::time_point time_point)
    {
        static constexpr duration_ms min_sleep_duration(0);

        while (clock::now() < time_point)
        {
            std::this_thread::sleep_for(min_sleep_duration);
        }
    }
}