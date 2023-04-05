#pragma once

#include <core/entity.h>
#include <memory/weak_ptr.h>

namespace entities::debug
{
	class Bootloader final : public Entity
	{
		DECLARE_ENTITY(Bootloader);

	public:
		Bootloader();

		static void initiate();

		void post_create() override;
		void tick(float delta_time) override;

	private:
		struct Entry
		{
			std::string path;
			peng::weak_ptr<Entity> entity;
		};

		std::string shorten_path(const std::string& scene_path) const;
		void update_entries_display();
		void load_entry(const Entry& entry);

		std::vector<Entry> _entries;
		int32_t _selected_entry = 0;
		std::string _scene_dir = "resources/scenes";
		std::string _scene_ext = ".json";

		static constexpr float font_size = 2.0f;
	};
}
