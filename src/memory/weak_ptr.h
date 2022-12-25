#pragma once

#include "shared_ptr.h"

namespace peng
{
	template <typename T>
	class weak_ptr
	{
	public:
		weak_ptr()
			: _ptr()
		{ }

		weak_ptr(const shared_ref<T>& ref)
			: _ptr(ref.get_impl())
		{ }

		weak_ptr(const shared_ptr<T>& ptr)
			: _ptr(ptr.get_impl())
		{ }

		template <typename U>
		requires std::derived_from<U, T>
		weak_ptr(const weak_ptr<U>& other)
			: _ptr(std::static_pointer_cast<T>(other.lock().get_impl()))
		{ }

		[[nodiscard]] shared_ptr<T> lock() const noexcept
		{
			return shared_ptr<T>(_ptr.lock());
		}

		[[nodiscard]] T* operator->() noexcept
		{
			return lock().get();
		}

		[[nodiscard]] const T* operator->() const noexcept 
		{
			return lock().get();
		}

		[[nodiscard]] bool valid() const noexcept
		{
			return !_ptr.expired();
		}

		explicit operator bool() const noexcept
		{
			return valid();
		}

	private:
		std::weak_ptr<T> _ptr;
	};

#pragma region Comparison Operators

	template <typename T, typename U>
	requires std::derived_from<T, U> || std::same_as<T, U>
	[[nodiscard]] bool operator==(const weak_ptr<T>& a, const weak_ptr<U>& b)
	{
		return a.lock().get() == b.lock().get();
	}

	template <typename T, typename U>
	requires std::derived_from<T, U> || std::same_as<T, U>
	[[nodiscard]] bool operator==(const weak_ptr<T>& a, const shared_ptr<U>& b)
	{
		return a == weak_ptr<T>(b);
	}

	template <typename T, typename U>
	requires std::derived_from<T, U> || std::same_as<T, U>
	[[nodiscard]] bool operator==(const shared_ptr<T>& a, const weak_ptr<U>& b)
	{
		return weak_ptr<T>(a) == b;
	}

	template <typename T, typename U>
	requires std::derived_from<T, U> || std::same_as<T, U>
	[[nodiscard]] bool operator==(const weak_ptr<T>& a, const shared_ref<U>& b)
	{
		return a == weak_ptr<T>(b);
	}

	template <typename T, typename U>
	requires std::derived_from<T, U> || std::same_as<T, U>
	[[nodiscard]] bool operator==(const shared_ref<T>& a, const weak_ptr<U>& b)
	{
		return weak_ptr<T>(a) == b;
	}

#pragma endregion
}