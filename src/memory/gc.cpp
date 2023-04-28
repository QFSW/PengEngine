#include "gc.h"

#include "profiling/scoped_event.h"

using namespace memory;

void GC::tick()
{
    SCOPED_EVENT("GC - tick");
    update_trackers();
    free_garbage();
}

bool GC::Tracker::dead() const noexcept
{
    return object.use_count() == 1;
}

void GC::update_trackers()
{
    SCOPED_EVENT("GC - update trackers");

    for (int32_t i = static_cast<int32_t>(_tracked_objects.size()) - 1; i >= 0; i--)
    {
        Tracker& tracker = _tracked_objects[i];

        // Reset the dead frames counter if object is alive, otherwise increment it
        tracker.dead_frames =
            tracker.dead()
            ? tracker.dead_frames + 1
            : 0;

        // If an tracker has been dead sufficiently long according to it's policy, enqueue for destruction
        if (tracker.dead_frames >= 2)
        {
            _garbage.push_back(std::move(tracker));
            _tracked_objects.erase(_tracked_objects.begin() + i);
        }
    }
}

void GC::free_garbage()
{
    // Currently just free all garbage at once
    SCOPED_EVENT("GC - free garbage");
    _garbage.clear();
}
