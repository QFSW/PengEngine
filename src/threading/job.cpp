#include "job.h"

namespace threading
{
    Job::Job(std::function<void()> f)
        : _f(f)
    { }

    void Job::execute() const
    {
        _f();
    }

    Job Job::empty()
    {
        return Job([] {});
    }
}