#pragma once

#include <vector>
#include <thread>
#include <string>

#include <common/common.h>

#include "job.h"

namespace threading
{
    class ThreadPool
    {
    public:
        explicit ThreadPool(size_t worker_count = get_auto_thread_count());
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        virtual ~ThreadPool();

        void schedule_job(Job&& job);
        void shutdown();

        // TODO: make this get used
        virtual std::string get_thread_name() const noexcept;

        [[nodiscard]] bool running() const noexcept { return _running; }
        [[nodiscard]] size_t num_pending_jobs() const noexcept { return _num_pending_jobs; }
        [[nodiscard]] size_t num_executing_jobs() const noexcept { return _num_busy_workers; }

    private:
        void flush_job_queue();
        void create_worker();
        void worker_routine();

        static size_t get_auto_thread_count();

        const size_t _max_workers;
        std::vector<std::thread> _workers;
        common::blocking_concurrent_queue<Job> _job_queue;
        std::atomic<bool> _running;
        std::atomic<size_t> _num_busy_workers;
        std::atomic<size_t> _num_pending_jobs;
    };
}