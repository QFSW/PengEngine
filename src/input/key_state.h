#pragma once

namespace input
{
	struct KeyState
	{
	public:
		[[nodiscard]] bool pressed() const noexcept;
		[[nodiscard]] bool is_down() const noexcept;
		[[nodiscard]] bool was_down() const noexcept;

		void update(bool is_down) noexcept;

	private:
		bool _is_down = false;
		bool _was_down = false;
	};
}
