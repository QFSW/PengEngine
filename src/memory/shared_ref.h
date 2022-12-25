#pragma once

#include <memory>
#include <stdexcept>

namespace peng
{
	template <typename T>
	class shared_ref
	{
	public:
		explicit shared_ref(const std::shared_ptr<T>& ptr)
			: _ptr(ptr)
		{
			if (_ptr == nullptr)
			{
				throw std::runtime_error("Cannot create a shared_ref from a nullptr");
			}
		}

		template <typename U>
		requires std::derived_from<U, T> || std::same_as<T, U>
		shared_ref(const shared_ref<U>& other)
			: _ptr(other.get_impl())
		{ }

		[[nodiscard]] T* get() const noexcept { return _ptr.get(); }
		[[nodiscard]] T* operator->() const noexcept { return get(); }
		[[nodiscard]] size_t use_count() const noexcept { return _ptr.use_count(); }

		[[nodiscard]] const std::shared_ptr<T>& get_impl() const noexcept
		{
			return _ptr;
		}

	private:
		std::shared_ptr<T> _ptr;
	};

	template <typename T, typename...Args>
	[[nodiscard]] shared_ref<T> make_shared(Args&&...args)
	{
		return shared_ref<T>(std::make_shared<T>(std::forward<Args>(args)...));
	}

	template <std::copyable T>
	[[nodiscard]] shared_ref<T> copy_shared(const shared_ref<T> ref)
	{
		return make_shared<T>(*ref.get());
	}

#pragma region Comparison Operators

	template <typename T, typename U>
	requires std::derived_from<T, U> || std::same_as<T, U>
	[[nodiscard]] bool operator==(const shared_ref<T>& a, const shared_ref<U>& b)
	{
		return a.get() == b.get();
	}

#pragma endregion
}