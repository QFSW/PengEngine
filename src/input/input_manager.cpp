#include "input_manager.h"

#include <GLFW/glfw3.h>

#include <utils/strtools.h>

using namespace input;

InputManager::InputManager()
	: _window(nullptr)
	, _opengl_keys
	{
		GLFW_KEY_0,
		GLFW_KEY_1,
		GLFW_KEY_2,
		GLFW_KEY_3,
		GLFW_KEY_4,
		GLFW_KEY_5,
		GLFW_KEY_6,
		GLFW_KEY_7,
		GLFW_KEY_8,
		GLFW_KEY_9,
		GLFW_KEY_A,
		GLFW_KEY_B,
		GLFW_KEY_C,
		GLFW_KEY_D,
		GLFW_KEY_E,
		GLFW_KEY_F,
		GLFW_KEY_G,
		GLFW_KEY_H,
		GLFW_KEY_I,
		GLFW_KEY_J,
		GLFW_KEY_K,
		GLFW_KEY_L,
		GLFW_KEY_M,
		GLFW_KEY_N,
		GLFW_KEY_O,
		GLFW_KEY_P,
		GLFW_KEY_Q,
		GLFW_KEY_R,
		GLFW_KEY_S,
		GLFW_KEY_T,
		GLFW_KEY_U,
		GLFW_KEY_V,
		GLFW_KEY_W,
		GLFW_KEY_X, 
		GLFW_KEY_Y,
		GLFW_KEY_Z,
		GLFW_KEY_F1,
		GLFW_KEY_F2,
		GLFW_KEY_F3,
		GLFW_KEY_F4,
		GLFW_KEY_F5,
		GLFW_KEY_F6,
		GLFW_KEY_F7,
		GLFW_KEY_F8,
		GLFW_KEY_F9,
		GLFW_KEY_F10,
		GLFW_KEY_F11,
		GLFW_KEY_F12,
		GLFW_KEY_ESCAPE,
		GLFW_KEY_ENTER,
		GLFW_KEY_TAB,
		GLFW_KEY_BACKSPACE,
		GLFW_KEY_INSERT,
		GLFW_KEY_DELETE,
		GLFW_KEY_RIGHT,
		GLFW_KEY_LEFT,
		GLFW_KEY_DOWN,
		GLFW_KEY_UP,
		GLFW_KEY_PAGE_UP,
		GLFW_KEY_PAGE_DOWN,
		GLFW_KEY_HOME,
		GLFW_KEY_END,
		GLFW_KEY_CAPS_LOCK,
		GLFW_KEY_SCROLL_LOCK,
		GLFW_KEY_NUM_LOCK,
		GLFW_KEY_PRINT_SCREEN,
		GLFW_KEY_PAUSE,
	}
{
	for (const int32_t opengl_key : _opengl_keys)
	{
		const KeyCode code = from_opengl(opengl_key);
		_key_states[code] = KeyState();
	}
}

void InputManager::start(GLFWwindow* window)
{
	_window = window;
}

void InputManager::tick()
{
	for (const int32_t opengl_key : _opengl_keys)
	{
		const KeyCode code = from_opengl(opengl_key);
		const bool key_down = glfwGetKey(_window, opengl_key) == GLFW_PRESS;
		_key_states[code].update(key_down);
	}
}

const KeyState& InputManager::get_key(KeyCode code) const
{
	if (const auto it = _key_states.find(code); it != _key_states.end())
	{
		return it->second;
	}

	throw std::logic_error(strtools::catf("KeyCode %d is invalid", static_cast<int32_t>(code)));
;}

const KeyState& InputManager::operator[](KeyCode code) const
{
	return get_key(code);
}

constexpr KeyCode InputManager::from_opengl(int32_t opengl_key)
{
	if (opengl_key >= GLFW_KEY_A && opengl_key <= GLFW_KEY_Z)
	{
		const int32_t letter_offset = opengl_key - GLFW_KEY_A;
		return static_cast<KeyCode>(static_cast<int32_t>(KeyCode::a) + letter_offset);
	}

	if (opengl_key >= GLFW_KEY_0 && opengl_key <= GLFW_KEY_9)
	{
		const int32_t num_row_offset = opengl_key - GLFW_KEY_0;
		return static_cast<KeyCode>(static_cast<int32_t>(KeyCode::num_row_0) + num_row_offset);
	}

	if (opengl_key >= GLFW_KEY_F1 && opengl_key <= GLFW_KEY_F12)
	{
		const int32_t function_key_offset = opengl_key - GLFW_KEY_F1;
		return static_cast<KeyCode>(static_cast<int32_t>(KeyCode::f1) + function_key_offset);
	}

	if (opengl_key >= GLFW_KEY_ESCAPE && opengl_key <= GLFW_KEY_PAUSE)
	{
		const int32_t key_offset = opengl_key - GLFW_KEY_ESCAPE;
		return static_cast<KeyCode>(static_cast<int32_t>(KeyCode::escape) + key_offset);
	}

	throw std::logic_error(strtools::catf("Cannot convert OpenGL key %d", opengl_key));
}
