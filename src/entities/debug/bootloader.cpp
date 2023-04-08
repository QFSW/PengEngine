#include "bootloader.h"

#include <filesystem>

#include <components/text_renderer.h>
#include <entities/camera.h>
#include <input/input_subsystem.h>
#include <scene/scene_loader.h>

IMPLEMENT_ENTITY(entities::debug::Bootloader);

using namespace entities::debug;
using namespace components;
using namespace input;

Bootloader::Bootloader()
    : Entity("Bootloader")
{ }

void Bootloader::initiate()
{
    for (const peng::weak_ptr<Entity>& entity : EntitySubsystem::get().all_entities())
    {
        entity->destroy();
    }

    EntitySubsystem::get().create_entity<Bootloader>();
}

void Bootloader::post_create()
{
    Entity::post_create();

    for (const auto& x : std::filesystem::recursive_directory_iterator(_scene_dir))
    {
        if (x.path().extension() == _scene_ext)
        {
            Entry entry;
            entry.path = x.path().string();
            entry.entity = create_child<Entity>(shorten_path(entry.path));
            entry.entity->add_component<TextRenderer>();
            entry.entity->local_transform().position = math::Vector3f(0, -1.25f * font_size, 0) * _entries.size();
            entry.entity->local_transform().scale = math::Vector3f::one() * font_size;

            _entries.push_back(entry);
        }
    }

    update_entries_display();

    // TODO: should be a better camera system so that UI elements can be rendered
    //       as an overlay without needing to create a camera like this
    create_child<Camera>()->make_orthographic(20, -10, 10);
}

void Bootloader::tick(float delta_time)
{
    Entity::tick(delta_time);

    const int32_t num_entries = static_cast<int32_t>(_entries.size());
    if (num_entries == 0)
    {
        return;
    }

    // TODO: Refactor out this button selection system into something more re-usable
    if (InputSubsystem::get()[KeyCode::down].pressed())
    {
        _selected_entry = ++_selected_entry % num_entries;
        update_entries_display();
    }

    if (InputSubsystem::get()[KeyCode::up].pressed())
    {
        _selected_entry = (--_selected_entry + num_entries) % num_entries;
        update_entries_display();
    }

    if (InputSubsystem::get()[KeyCode::enter].pressed())
    {
        load_entry(_entries[_selected_entry]);
    }
}

std::string Bootloader::shorten_path(const std::string& scene_path) const
{
    return scene_path.substr(
        _scene_dir.size() + 1,
        scene_path.size() - _scene_dir.size() - _scene_ext.size() - 1
    );
}

void Bootloader::update_entries_display()
{
    for (size_t i = 0; i < _entries.size(); i++)
    {
        const bool selected = i == _selected_entry;
        const std::string label = strtools::catf(
            "%s%s", selected ? ">" : " ", _entries[i].entity->name().c_str()
        );

        _entries[i].entity->get_component<TextRenderer>()->set_text(label);
    }
}

void Bootloader::load_entry(const Entry& entry)
{
    scene::SceneLoader loader;
    loader.load_from_file(entry.path);

    destroy();
}
