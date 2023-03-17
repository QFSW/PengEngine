#pragma once

#include <memory/weak_ptr.h>
#include <libs/nlohmann/json.hpp>

class Entity;

namespace scene
{
	class SceneLoader
	{
	public:
		void load_from_file(const std::string& path);
		void load_from_json(const nlohmann::json& world_def);

	private:
		void load_entities(const nlohmann::json& world_def);
		void load_entity(const nlohmann::json& entity_def);
		void load_components(const nlohmann::json& entity_def, const peng::weak_ptr<Entity>& entity);
		void load_component(const nlohmann::json& component_def, const peng::weak_ptr<Entity>& entity);

		template <typename T>
		[[nodiscard]] T get_value(const nlohmann::json& obj, const std::string& key) const;

		template <typename T>
		[[nodiscard]] T get_value_or_default(const nlohmann::json& obj, const std::string& key, const T& value = {}) const;
	};

	template <typename T>
	T SceneLoader::get_value(const nlohmann::json& obj, const std::string& key) const
	{
		return obj.at(key).get<T>();
	}

	template <typename T>
	T SceneLoader::get_value_or_default(const nlohmann::json& obj, const std::string& key, const T& value) const
	{
		if (const auto it = obj.find(key); it != obj.end())
		{
			return it->get<T>();
		}

		return value;
	}
}
