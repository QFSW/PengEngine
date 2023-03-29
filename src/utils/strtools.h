#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "traits.h"

namespace strtools
{
    namespace detail
    {
        std::vector<char>& get_catf_buffer();
    }

    // Creates a formatted string in a temporary buffer for immediate use
    // The result can safely be used until the next time catf/catf_temp are called
    template <typename ...Args>
    [[nodiscard]] const char* catf_temp(const char* format, Args...args)
    {
        static_assert(traits::for_none<std::is_class, Args...>(), "strtools::catf does not work with classes");
        std::vector<char>& char_buf = detail::get_catf_buffer();

        const int32_t msg_size = snprintf(char_buf.data(), char_buf.size(), format, args...);
        if (msg_size < static_cast<int32_t>(char_buf.size()))
        {
            return char_buf.data();
        }

        char_buf.resize(msg_size + 1);
        return catf_temp(format, args...);
    }

    template <typename ...Args>
    [[nodiscard]] std::string catf(const char* format, Args...args)
    {
        return catf_temp(format, args...);
    }

    template <typename T>
    std::string cat(T t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    template <typename T>
    std::string fixed_digits(T num, int digits, const char* delim = "0")
    {
        static_assert(std::is_integral<T>::value, "strtools::fixed_digits requires an integral value");

        constexpr size_t buf_size = 8;
        char fmt_buf[buf_size];

        snprintf(fmt_buf, buf_size, "%%%s%dd", delim, digits);
        return catf(fmt_buf, num);
    }

    template <typename Out>
    void split(const std::string& str, char delim, Out result)
    {
        std::istringstream iss(str);
        std::string item;
        while (std::getline(iss, item, delim)) {
            *result++ = item;
        }
    }

    std::vector<std::string> split(const std::string& str, char delim);
    std::string remove_after(const std::string& str, char delim);
    std::string remove_before(const std::string& str, char delim);

    enum class AnsiColor
    {
        Black = 30,
        Blue = 34,
        Green = 32,
        Cyan = 36,
        Red = 31,
        Purple = 35,
        Brown = 33,
        Gray = 37
    };

    std::string colorize(const std::string& str, AnsiColor col);

    bool replace_substr(std::string& str, const std::string& to_find, const std::string& to_replace);
    void replace_substr_all(std::string& str, const std::string& to_find, const std::string& to_replace);
    bool str_contains(const std::string& str, const std::string& substr);
    bool isspace(const std::string& str);
}

