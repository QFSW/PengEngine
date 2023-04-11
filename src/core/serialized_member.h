#pragma once

#include <string_view>

#include "archive.h"

namespace detail
{
    using Str = const char*;

    inline std::string get_member_name(const char* raw_name, const char* provided_name)
    {
        if (provided_name && !std::string_view(provided_name).empty())
        {
            return provided_name;
        }

        if (raw_name[0] == '_')
        {
            return &raw_name[1];
        }

        return raw_name;
    }
}

// Adds serialization for the provided member
// If a name is not provided, the member name with any leading _ removed will be used
// e.g: _member will become "member"
#define SERIALIZED_MEMBER(member, ...)                                                              \
    do                                                                                              \
    {                                                                                               \
        const std::string __PE_name = detail::get_member_name(#member, detail::Str(__VA_ARGS__));   \
        add_serializer([this, name = __PE_name](Archive& archive)                                   \
        {                                                                                           \
            archive.write(name.c_str(), member);                                                    \
        });                                                                                         \
                                                                                                    \
        add_deserializer([this, name = __PE_name](const Archive& archive)                           \
        {                                                                                           \
            archive.try_read(name.c_str(), member);                                                 \
        });                                                                                         \
    } while(0)