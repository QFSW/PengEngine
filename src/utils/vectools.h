#pragma once

#include <vector>
#include <functional>

namespace vectools
{
    template <typename T>
    void remove_all(std::vector<T>& v, const std::function<bool(const T&)>& condition)
    {
        v.erase(
            std::remove_if(v.begin(), v.end(), condition),
            v.end()
        );
    }
}