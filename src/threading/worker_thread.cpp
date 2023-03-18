#include "worker_thread.h"

namespace threading
{
    WorkerThread::WorkerThread(std::string&& thread_name)
        : ThreadPool(1)
        , _thread_name(thread_name)
    { }

    std::string WorkerThread::get_thread_name() const noexcept
    {
        return _thread_name;
    }
}
