#pragma once

#include "key_code.h"
#include "key_state.h"

#include <common/common.h>

struct GLFWwindow;

namespace input
{
	class InputManager
	{
	public:
		InputManager();

		// ----------- Engine API -----------
		void start(GLFWwindow* window);
		void tick();
		// ----------------------------------

		// ------------ User API ------------
		[[nodiscard]] const KeyState& get_key(KeyCode code) const;
		[[nodiscard]] const KeyState& operator[](KeyCode code) const;
		// ----------------------------------

	private:
		static constexpr KeyCode from_opengl(int32_t opengl_key);
		
		GLFWwindow* _window;
		std::vector<int32_t> _opengl_keys;
		common::unordered_map<KeyCode, KeyState> _key_states;
	};
}
