#pragma once

#include <cstdint>
#include <mutex>
#include <utility>

#include "detail/final_act.h"

namespace utils
{
    template <typename T>
    [[nodiscard]] T copy(const T& t)
    {
        return T(t);
    }

    template <class F>
    [[nodiscard]] detail::final_act<F> finally(const F& f) noexcept
    {
        return detail::final_act(f);
    }

    template <class F>
    [[nodiscard]] detail::final_act<F> finally(F&& f) noexcept
    {
        return detail::final_act(std::move(f));
    }

    template <typename T>
    void swap(T& t1, T& t2) noexcept
    {
        T tmp = std::move(t1);
        t1 = std::move(t2);
        t2 = std::move(tmp);
    }

    template <typename F>
    void potentially_lock(std::mutex& m, const bool should_lock, F&& f)
    {
        if (should_lock)
        {
            std::lock_guard<std::mutex> lock(m);
            f();
        }
        else
        {
            f();
        }
    }

    template <typename T, typename F>
    T potentially_lock_return(std::mutex& m, const bool should_lock, F&& f)
    {
        if (should_lock)
        {
            std::lock_guard<std::mutex> lock(m);
            return f();
        }

        return f();
    }
}