#include "worker_thread.h"

using namespace threading;

WorkerThread::WorkerThread(std::string&& thread_name)
    : _running(true)
    , _worker_busy(false)
    , _num_pending_jobs(0)
{
    _worker = std::make_unique<std::thread>([&] {
        worker_routine();
    });
}

WorkerThread::~WorkerThread()
{
    shutdown();
}

void WorkerThread::schedule_job(Job&& job)
{
    _num_pending_jobs++;
    _job_queue.enqueue(std::move(job));
}

void WorkerThread::shutdown()
{
    _running = false;
    flush_job_queue();

    _worker->join();
}

void WorkerThread::flush_job_queue()
{
    schedule_job(Job::empty());
}

void WorkerThread::worker_routine()
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

        _worker_busy = true;
        job.execute();
        _worker_busy = false;
    }
}
