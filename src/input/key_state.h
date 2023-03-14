#pragma once

namespace input
{
	struct KeyState
	{
	public:
		// TODO: this doesn't work properly if the frametime is long enough
		// @return true on the first frame the key was pressed, false otherwise
		[[nodiscard]] bool pressed() const noexcept;

		// @return true if the key is currently held down
		[[nodiscard]] bool is_down() const noexcept;

		// @return true of the key was held down on the previous frame
		[[nodiscard]] bool was_down() const noexcept;

		void update(bool is_down) noexcept;

	private:
		bool _is_down = false;
		bool _was_down = false;
	};
}
