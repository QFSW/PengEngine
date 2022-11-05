#pragma once

#include <utility>

namespace utils::detail
{
    template <class F>
    class final_act
    {
    public:
        explicit final_act(F f) noexcept
            : _f(std::move(f))
            , _invoke(true)
        { }

        final_act(final_act&& other) noexcept
            : _f(std::move(other._f))
            , _invoke(other._invoke)
        {
            other._invoke = false;
        }

        final_act(const final_act&) = delete;
        final_act& operator=(const final_act&) = delete;
        final_act& operator=(final_act&&) = delete;

        ~final_act() noexcept
        {
            if (_invoke) _f();
        }

    private:
        const F _f;
        bool _invoke;
    };
}