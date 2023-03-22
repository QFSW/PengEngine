#pragma once

#include <unordered_map>

#include <core/subsystem.h>
#include <math/vector2.h>

#include "key_code.h"
#include "key_state.h"
#include "axis.h"

struct GLFWwindow;

namespace input
{
	// TODO: Add "Action" for wrapping key bindings for actions 
	class InputSubsystem : public Subsystem
	{
        DECLARE_SUBSYSTEM(InputSubsystem)

	public:
		InputSubsystem();

		// ----------- Engine API -----------
		void start() override;
		void shutdown() override;
		void tick(float delta_time) override;
		// ----------------------------------

		// ------------ User API ------------
		[[nodiscard]] math::Vector2i cursor_delta() const noexcept;
		[[nodiscard]] const math::Vector2i& cursor_pos() const noexcept;
		[[nodiscard]] const math::Vector2i& last_cursor_pos() const noexcept;
		[[nodiscard]] const KeyState& get_key(KeyCode code) const;
		[[nodiscard]] const KeyState& operator[](KeyCode code) const;
		[[nodiscard]] float axis_value(const Axis& axis) const;
		// ----------------------------------

	private:
		static constexpr KeyCode from_opengl(int32_t opengl_key);
		
		GLFWwindow* _window;
		std::vector<int32_t> _opengl_keys;
		std::vector<int32_t> _opengl_mouse_buttons;

		bool _has_ticked = false;
		math::Vector2i _cursor_pos;
		math::Vector2i _last_cursor_pos;
		std::unordered_map<KeyCode, KeyState> _key_states;
	};
}
