#include "strtools.h"

#include <algorithm>
#include <cctype>

namespace strtools
{
    std::vector<char>& detail::get_catf_buffer()
    {
        static thread_local std::vector<char> char_buf(1024);
        return char_buf;
    }

    std::vector<std::string> split(const std::string& str, const char delim)
    {
        std::vector<std::string> elems;
        split(str, delim, std::back_inserter(elems));
        return elems;
    }

    std::string remove_after(const std::string& str, const char delim)
    {
        std::istringstream iss(str);
        std::string item;
        std::getline(iss, item, delim);
        return item;
    }

    std::string remove_before(const std::string& str, const char delim)
    {
        size_t pos = str.find(delim);
        if (pos != std::string::npos)
            return str.substr(pos);

        return std::string();
    }

    std::string colorize(const std::string& str, const AnsiColor col)
    {
        return catf("\033[1;%dm%s\033[0m", col, str.c_str());
    }

    bool replace_substr(std::string& str, const std::string& to_find, const std::string& to_replace)
    {
        size_t pos = str.find(to_find);
        if (pos != std::string::npos)
        {
            str.replace(pos, to_find.size(), to_replace);
            return true;
        }

        return false;
    }

    void replace_substr_all(std::string& str, const std::string& to_find, const std::string& to_replace)
    {
        while (replace_substr(str, to_find, to_replace)) { }
    }

    bool str_contains(const std::string& str, const std::string& substr)
    {
        return str.find(substr) != std::string::npos;
    }

    bool isspace(const std::string& str)
    {
        for (const auto c : str)
        {
            if (!::isspace(c))
                return false;
        }

        return true;
    }
}