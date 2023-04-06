#pragma once

#include <thread>
#include <string>

#include <common/common.h>

#include "job.h"

namespace threading
{
    class WorkerThread
    {
    public:
        explicit WorkerThread(std::string&& thread_name);
        WorkerThread(const WorkerThread&) = delete;
        WorkerThread(WorkerThread&&) = delete;
        ~WorkerThread();

        void schedule_job(Job&& job);
        void shutdown();

        [[nodiscard]] bool running() const noexcept { return _running; }
        [[nodiscard]] bool worker_busy() const noexcept { return _worker_busy; }
        [[nodiscard]] size_t num_pending_jobs() const noexcept { return _num_pending_jobs; }

    private:
        void flush_job_queue();
        void worker_routine();

        std::unique_ptr<std::thread> _worker;
        common::blocking_concurrent_queue<Job> _job_queue;
        std::atomic<bool> _running;
        std::atomic<bool> _worker_busy;
        std::atomic<size_t> _num_pending_jobs;
    };
}