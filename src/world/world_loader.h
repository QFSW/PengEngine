#pragma once

#include <libs/nlohmann/json.hpp>

namespace world
{
	class WorldLoader
	{
	public:
		void load_from_file(const std::string& path);
		void load_from_json(const nlohmann::json& world_def);

	private:
		void load_entities(const nlohmann::json& world_def);
		void load_entity(const nlohmann::json& entity_def);

		template <typename T>
		[[nodiscard]] T get_value(const nlohmann::json& obj, const std::string& key) const;

		template <typename T>
		[[nodiscard]] T get_value_or_default(const nlohmann::json& obj, const std::string& key, const T& value = {}) const;
	};

	template <typename T>
	T WorldLoader::get_value(const nlohmann::json& obj, const std::string& key) const
	{
		return obj.at(key).get<T>();
	}

	template <typename T>
	T WorldLoader::get_value_or_default(const nlohmann::json& obj, const std::string& key, const T& value) const
	{
		if (const auto it = obj.find(key); it != obj.end())
		{
			return it->get<T>();
		}

		return value;
	}
}
