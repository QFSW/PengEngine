#pragma once

#include <vector>
#include <fstream>

#include "io.h"
#include "strtools.h"

namespace csv
{
    template <typename T>
    class CsvParser
    {
    public:
        T parse_item(const std::string& item) const = delete;
    };

    template <typename T>
    std::vector<T> read_file(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file " + filepath);
        }

        std::vector<T> results;
        std::string line;

        while (std::getline(file, line))
        {
            results.push_back(CsvParser<T>().parse_item(line));
        }

        return results;
    }

#pragma region Tuples

    template <typename...Args>
    class CsvParser<std::tuple<Args...>>
    {
    public:
        std::tuple<Args...> parse_item(const std::string& item) const;

    private:
        template <typename A>
        std::tuple<A> parse(const std::vector<std::string>& items, int32_t i) const;

        template <typename A1, typename A2, typename...As>
        std::tuple<A1, A2, As...> parse(const std::vector<std::string>& items, int32_t i) const;
    };

    template <typename ... Args>
    std::tuple<Args...> CsvParser<std::tuple<Args...>>::parse_item(const std::string& item) const
    {
	    constexpr size_t pack_size = sizeof...(Args);
	    std::vector<std::string> sub_items = strtools::split(item, ',');
        sub_items.resize(std::min(sub_items.size(), pack_size));

	    return parse<Args...>(sub_items, 0);
    }

    template <typename ... Args>
    template <typename A>
    std::tuple<A> CsvParser<std::tuple<Args...>>::parse(const std::vector<std::string>& items, int32_t i) const
    {
        return std::tuple<A>(CsvParser<A>().parse_item(items[i]));
    }

    template <typename ... Args>
    template <typename A1, typename A2, typename ... As>
    std::tuple<A1, A2, As...> CsvParser<std::tuple<Args...>>::parse(const std::vector<std::string>& items, int32_t i) const
    {
        std::tuple<A1> t = parse<A1>(items, i);
        std::tuple<A2, As...> ts = parse<A2, As...>(items, i + 1);
        return std::tuple_cat(t, ts);
    }

#pragma endregion

#pragma region TypeParsers

    template<>
    class CsvParser<std::string>
    {
    public:
        std::string parse_item(const std::string& item) const;
    };

#pragma endregion
}