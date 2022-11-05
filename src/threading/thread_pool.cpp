#include "thread_pool.h"

#include <utils/strtools.h>

namespace threading
{
    ThreadPool::ThreadPool(const size_t worker_count)
        : _max_workers(worker_count)
        , _running(true)
        , _num_busy_workers(0)
        , _num_pending_jobs(0)
    {
        _workers.reserve(_max_workers);
    }

    ThreadPool::~ThreadPool()
    {
        shutdown();
    }

    void ThreadPool::schedule_job(Job&& job)
    {
        _num_pending_jobs++;
        _job_queue.enqueue(std::move(job));

        if (_running
            && _num_busy_workers == _workers.size()
            && _workers.size() < _max_workers)
        {
            create_worker();
        }
    }

    void ThreadPool::shutdown()
    {
        _running = false;
        flush_job_queue();

        for (auto& worker : _workers)
        {
            worker.join();
        }

        _workers.clear();
    }

    std::string ThreadPool::get_thread_name() const noexcept
    {
        static int32_t num_worker_threads = 0;
        return strtools::catf("WorkerThread_%d", num_worker_threads++);
    }

    void ThreadPool::flush_job_queue()
    {
        for (size_t i = 0; i < _workers.size(); i++)
        {
            schedule_job(Job::empty());
        }
    }

    void ThreadPool::create_worker()
    {
        _workers.emplace_back([&] {
            worker_routine();
        });
    }

    void ThreadPool::worker_routine()
    {
        Job job = Job::empty();
        decltype(_job_queue)::consumer_token_t dequeue_token(_job_queue);

        while (_running)
        {
            _job_queue.wait_dequeue(dequeue_token, job);
            _num_pending_jobs--;

            if (!_running)
            {
                return;
            }

            ++_num_busy_workers;
            job.execute();
            --_num_busy_workers;
        }
    }

    size_t ThreadPool::get_auto_thread_count()
    {
        const uint32_t threads = std::thread::hardware_concurrency();
        return threads > 0
            ? threads
            : 8;
    }
}
