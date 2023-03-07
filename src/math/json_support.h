#pragma once

#include <libs/nlohmann/json.hpp>

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "transform.h"

// A collection of JSON parsers/serializers for the types in the math namespace

namespace math
{
    void to_json(nlohmann::json& j, const Transform& t);

    void from_json(const nlohmann::json& j, Transform& t);

    template <typename T>
    void json_try_get_to(const nlohmann::json& j, const std::string& key, T& to)
    {
        if (const auto it = j.find(key); it != j.end())
        {
            it->get_to(to);
        }
    }

    template <number T>
    void to_json(nlohmann::json& j, const Vector4<T>& v)
	{
        j = nlohmann::json({
    		{"x", v.x},
    		{"y", v.y},
    		{"z", v.z},
            {"w", v.w}
    	});
    }

    template <number T>
    void to_json(nlohmann::json& j, const Vector3<T>& v)
    {
        j = nlohmann::json({
            {"x", v.x},
            {"y", v.y},
            {"z", v.z}
        });
    }

    template <number T>
    void to_json(nlohmann::json& j, const Vector2<T>& v)
    {
        j = nlohmann::json({
            {"x", v.x},
            {"y", v.y}
        });
    }

    template <number T>
    void from_json(const nlohmann::json& j, Vector4<T>& v)
	{
        json_try_get_to(j, "x", v.x);
        json_try_get_to(j, "y", v.y);
        json_try_get_to(j, "z", v.z);
        json_try_get_to(j, "w", v.w);
    }

    template <number T>
    void from_json(const nlohmann::json& j, Vector3<T>& v)
    {
        json_try_get_to(j, "x", v.x);
        json_try_get_to(j, "y", v.y);
        json_try_get_to(j, "z", v.z);
    }

    template <number T>
    void from_json(const nlohmann::json& j, Vector2<T>& v)
    {
	    json_try_get_to(j, "x", v.x);
	    json_try_get_to(j, "y", v.y);
    }
}
