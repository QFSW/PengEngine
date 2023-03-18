#pragma once

#include "thread_pool.h"

namespace threading
{
    class WorkerThread : public ThreadPool
    {
    public:
        explicit WorkerThread(std::string&& thread_name);

        virtual std::string get_thread_name() const noexcept override;

    private:
        const std::string _thread_name;
    };
}