#pragma once

#include "key_code.h"
#include "key_state.h"

#include <common/common.h>
#include <math/vector2.h>

struct GLFWwindow;

namespace input
{
	// TODO: Add "Action" for wrapping key bindings for actions 
	// TODO: Add "Axis" for wrapping things like "forwards"
	class InputManager
	{
	public:
		InputManager();

		// ----------- Engine API -----------
		void start(GLFWwindow* window);
		void tick();
		// ----------------------------------

		// ------------ User API ------------
		[[nodiscard]] math::Vector2i cursor_delta() const noexcept;
		[[nodiscard]] const math::Vector2i& cursor_pos() const noexcept;
		[[nodiscard]] const math::Vector2i& last_cursor_pos() const noexcept;
		[[nodiscard]] const KeyState& get_key(KeyCode code) const;
		[[nodiscard]] const KeyState& operator[](KeyCode code) const;
		// ----------------------------------

	private:
		static constexpr KeyCode from_opengl(int32_t opengl_key);
		
		GLFWwindow* _window;
		std::vector<int32_t> _opengl_keys;
		std::vector<int32_t> _opengl_mouse_buttons;

		math::Vector2i _cursor_pos;
		math::Vector2i _last_cursor_pos;
		common::unordered_map<KeyCode, KeyState> _key_states;
	};
}
