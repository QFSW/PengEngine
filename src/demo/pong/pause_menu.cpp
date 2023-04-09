#include "pause_menu.h"

#include <components/sprite_renderer.h>
#include <components/text_renderer.h>
#include <input/input_subsystem.h>

using namespace demo::pong;
using namespace components;
using namespace input;
using namespace math;

PauseMenu::PauseMenu()
    : Entity("PauseMenu")
{ }

void PauseMenu::post_create()
{
    Entity::post_create();

    _local_transform.position = Vector3f(0, 0, -5);

	peng::weak_ptr<Entity> background = create_child<Entity>("Background");
	background->local_transform().position = Vector3f(0, 0, 1);
	background->local_transform().scale = Vector3f::one() * 100;
	background->add_component<SpriteRenderer>()->color() = Vector4f(0, 0, 0, 0.75f);

	peng::weak_ptr<Entity> text = create_child<Entity>("Text");
	text->local_transform().scale = Vector3f::one() * pause_size;
	text->add_component<TextRenderer>()->set_text("paused");

    _button_events = std::vector({
        &_on_resume,
        &_on_restart,
        &_on_quit
    });

    const std::vector<std::string> button_labels({
        "Resume",
        "Restart",
        "Quit"
    });

    for (int32_t i = 0; i < button_labels.size(); i++)
    {
        peng::weak_ptr<Entity> button = create_child<Entity>(button_labels[i]);
        button->local_transform().position = Vector3f(0, -5.25f - i * 1.75f * btn_size, 0);
        button->local_transform().scale = Vector3f::one() * btn_size;
        button->add_component<TextRenderer>();

        _buttons.push_back(button);
    }

    update_buttons();
}

void PauseMenu::tick(float delta_time)
{
    Entity::tick(delta_time);

    const int32_t num_entries = static_cast<int32_t>(_buttons.size());
    if (num_entries == 0)
    {
        return;
    }

    int32_t new_selection = _selected_entry;

    if (InputSubsystem::get()[KeyCode::down].pressed())
    {
        new_selection = ++new_selection % num_entries;
    }

    if (InputSubsystem::get()[KeyCode::up].pressed())
    {
        new_selection = (--new_selection + num_entries) % num_entries;
    }

    if (new_selection != _selected_entry)
    {
        _selected_entry = new_selection;
        update_buttons();
        _on_selection_change();
    }

    if (InputSubsystem::get()[KeyCode::enter].pressed())
    {
        _on_selection_click();
        _button_events[_selected_entry]->invoke();
    }
}

void PauseMenu::show()
{
    _selected_entry = 0;

    set_active(true);
    update_buttons();
}

void PauseMenu::hide()
{
    set_active(false);
}

void PauseMenu::update_buttons()
{
    for (int32_t i = 0; i < _buttons.size(); i++)
    {
        const std::string label =
            i == _selected_entry
            ? strtools::catf(">%s<", _buttons[i]->name().c_str())
            : _buttons[i]->name();

        _buttons[i]->get_component<TextRenderer>()->set_text(label);
    }
}
