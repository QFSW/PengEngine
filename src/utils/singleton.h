#pragma once

namespace utils
{
    template <typename T>
    class Singleton
    {
    public:
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(Singleton&&) = delete;

        static T& get()
        {
            static T instance;
            exists_mutable() = true;

            return instance;
        }

        static bool exists()
        {
            return exists_mutable();
        }

    protected:
        Singleton() = default;

        static bool& exists_mutable()
        {
            static bool e = false;
            return e;
        }
    };
}
