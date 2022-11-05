#pragma once
#include <string>
#include <sstream>
#include <vector>

#include "traits.h"

namespace strtools
{
	template <size_t BufSize = 1024, typename ...Args>
	std::string catf(const char* format, Args...args)
	{
		static_assert(traits::for_none<std::is_class, Args...>(), "strtools::catf does not work with classes");

		char buf[BufSize];
		snprintf(buf, BufSize, format, args...);
		return std::string(buf);
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
	std::string to_lower(const std::string& str);

	bool replace_substr(std::string& str, const std::string& to_find, const std::string& to_replace);
	void replace_substr_all(std::string& str, const std::string& to_find, const std::string& to_replace);
	bool str_contains(const std::string& str, const std::string& substr);
	bool isspace(const std::string& str);
}

