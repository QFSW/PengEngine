#include "bootloader.h"

#include <filesystem>

#include <components/text_renderer.h>
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
            entry.entity = create_child<Entity>(x.path().stem().string());
            entry.entity->add_component<TextRenderer>()->set_text(shorten_path(entry.path));
            entry.entity->local_transform().position = math::Vector3f(0, -1.25f * font_size, 0) * _entries.size();
            entry.entity->local_transform().scale = math::Vector3f::one() * font_size;

            _entries.push_back(entry);
        }
    }
}

void Bootloader::tick(float delta_time)
{
    Entity::tick(delta_time);

    if (_entries.empty())
    {
        return;
    }

    if (InputSubsystem::get()[KeyCode::down].pressed())
    {
        _selected_entry = (_selected_entry + 1) % _entries.size();
    }

    if (InputSubsystem::get()[KeyCode::up].pressed())
    {
        _selected_entry = (_selected_entry - 1) % _entries.size();
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

void Bootloader::load_entry(const Entry& entry)
{
    scene::SceneLoader loader;
    loader.load_from_file(entry.path);

    destroy();
}
