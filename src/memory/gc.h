#pragma once

#include <utils/singleton.h>

#include "shared_ptr.h"

namespace memory
{
    // TODO: add more control over GC policies
    class GC : public utils::Singleton<GC>
    {
        using Singleton::Singleton;

    public:
        template <typename T, typename...Args>
        requires std::constructible_from<T, Args...>
        [[nodiscard]] static peng::shared_ref<T> alloc(Args&&...args);

        void tick();

    private:
        struct Tracker
        {
            peng::shared_ref<void> object;
            int32_t dead_frames = 0;

            // We consider a tracked object dead if there are no longer any strong
            // references left to it. Dead objects can still be revived via weak_ptrs however
            [[nodiscard]] bool dead() const noexcept;
        };

        void update_trackers();
        void free_garbage();
        
        std::vector<Tracker> _tracked_objects;
        std::vector<Tracker> _garbage;
    };

    template <typename T, typename ... Args> requires std::constructible_from<T, Args...>
    peng::shared_ref<T> GC::alloc(Args&&... args)
    {
        // TODO: make it thread safe to alloc
        peng::shared_ref<T> obj = peng::make_shared<T>(std::forward<Args>(args)...);
        get()._tracked_objects.push_back(Tracker{
            .object = obj,
            .dead_frames = 0
        });

        return obj;
    }
}
