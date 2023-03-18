#pragma once

#include <cstdint>

namespace physics
{
	// Represents a single physics layer by an index [0, 63]
	// Can be converted into a bitwise mask
	struct Layer
	{
	public:
		Layer();
		explicit Layer(uint8_t index);

		[[nodiscard]] uint8_t index() const noexcept;
		[[nodiscard]] uint64_t mask() const noexcept;

		bool operator==(const Layer& other) const noexcept;
		bool operator!=(const Layer& other) const noexcept;

	private:
		uint8_t _index;
	};
}
