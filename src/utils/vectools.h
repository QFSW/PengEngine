#pragma once

#include <vector>
#include <functional>
#include <algorithm>

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

    template <typename T>
    void remove(std::vector<T>& v, const T& item)
    {
        v.erase(std::remove(v.begin(), v.end(), item), v.end());
    }

    template <typename T>
    bool contains(const std::vector<T>& v, const T& item)
    {
        return std::find(v.begin(), v.end(), item) != v.end();
    }

    template <typename T>
    size_t buffer_size(const std::vector<T>& v)
    {
        return v.size() * sizeof(T);
    }
}