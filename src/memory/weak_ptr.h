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
        requires std::convertible_to<U*, T*>
        weak_ptr(const weak_ptr<U>& other)
            : _ptr(std::static_pointer_cast<T>(other.lock().get_impl()))
        { }

        template <typename U>
        requires std::convertible_to<U*, T*>
        weak_ptr& operator=(const shared_ref<U>& other)
        {
            _ptr = other.get_impl();
            return *this;
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        weak_ptr& operator=(const shared_ptr<U>& other)
        {
            _ptr = other.get_impl();
            return *this;
        }

        template <typename U>
        requires std::convertible_to<U*, T*>
        weak_ptr& operator=(const weak_ptr<U>& other)
        {
            _ptr = std::static_pointer_cast<T>(other.lock().get_impl());
            return *this;
        }

        weak_ptr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        [[nodiscard]] shared_ptr<T> lock() const noexcept
        {
            return shared_ptr<T>(_ptr.lock());
        }

        [[nodiscard]] T* operator->() const 
        {
            check(valid());
            return _ptr.lock().get();
        }

        [[nodiscard]] bool valid() const noexcept
        {
            return !_ptr.expired();
        }

        weak_ptr& if_valid(const std::function<void(T&)>& func)
        {
            lock().if_valid(func);
            return *this;
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
    requires std::equality_comparable_with<T*, U*>
    [[nodiscard]] bool operator==(const weak_ptr<T>& a, const weak_ptr<U>& b)
    {
        return a.lock().get() == b.lock().get();
    }

    template <typename T, typename U>
    requires std::equality_comparable_with<T*, U*>
    [[nodiscard]] bool operator==(const weak_ptr<T>& a, const shared_ptr<U>& b)
    {
        return a == weak_ptr<T>(b);
    }

    template <typename T, typename U>
    requires std::equality_comparable_with<T*, U*>
    [[nodiscard]] bool operator==(const shared_ptr<T>& a, const weak_ptr<U>& b)
    {
        return weak_ptr<T>(a) == b;
    }

    template <typename T, typename U>
    requires std::equality_comparable_with<T*, U*>
    [[nodiscard]] bool operator==(const weak_ptr<T>& a, const shared_ref<U>& b)
    {
        return a == weak_ptr<T>(b);
    }

    template <typename T, typename U>
    requires std::equality_comparable_with<T*, U*>
    [[nodiscard]] bool operator==(const shared_ref<T>& a, const weak_ptr<U>& b)
    {
        return weak_ptr<T>(a) == b;
    }

#pragma endregion
}

template<typename T>
struct std::hash<peng::weak_ptr<T>>
{
    size_t operator()(const peng::weak_ptr<T>& ptr) const
    {
        return std::hash<T*>{}(ptr.lock().get());
    }
};