#pragma once

#include <memory>
#include <stdexcept>

#include <utils/check.h>

namespace peng
{
    template <typename T>
    class shared_ref
    {
    public:
        explicit shared_ref(const std::shared_ptr<T>& ptr)
            : _ptr(ptr)
        {
            check(_ptr);
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        shared_ref(const shared_ref<U>& other)
            : _ptr(other.get_impl())
        { }

        template <typename U>
        requires std::convertible_to<U*, T*>
        shared_ref& operator=(const shared_ref<U>& other)
        {
            _ptr = other.get_impl();
            return *this;
        }

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
    requires std::constructible_from<T, Args...>
    [[nodiscard]] shared_ref<T> make_shared(Args&&...args)
    {
        return shared_ref<T>(std::make_shared<T>(std::forward<Args>(args)...));
    }

    template <std::copy_constructible T>
    [[nodiscard]] shared_ref<std::remove_const_t<T>> copy_shared(const shared_ref<T> ref)
    {
        return make_shared<std::remove_const_t<T>>(*ref.get());
    }

#pragma region Comparison Operators

    template <typename T, typename U>
    requires std::equality_comparable_with<T*, U*>
    [[nodiscard]] bool operator==(const shared_ref<T>& a, const shared_ref<U>& b)
    {
        return a.get() == b.get();
    }

#pragma endregion
}

template<typename T>
struct std::hash<peng::shared_ref<T>>
{
    size_t operator()(const peng::shared_ref<T>& ref) const
    {
        return std::hash<T*>{}(ref.get());
    }
};