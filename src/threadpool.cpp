#include "threadpool.h"

ThreadPool::ThreadPool(): done(false), busy(0), num_threads(std::thread::hardware_concurrency()) {
    for (auto i = 0; i < num_threads; ++i)
        threads.emplace_back(std::thread(&ThreadPool::thread_loop, this));
}

ThreadPool::~ThreadPool() {
    ulock lock(queue_lock);
    done = true;
    new_job.notify_all();
    lock.unlock();
    for (auto &task: threads)
        task.join();
}

void ThreadPool::thread_loop() {
    while (true) {
        ulock lock(queue_lock);
        new_job.wait(lock, [this] {
            return done || !jobs.empty();
        });

        if (!jobs.empty()) {
            ++busy;
            auto function = std::move(jobs.front());
            jobs.pop_front();

            lock.unlock();

            function();

            lock.lock();
            --busy;
            finished_jobs.notify_one();

        } else if (done) break;
    }
}

void ThreadPool::wait_for_jobs() {
    ulock lock(queue_lock);
    finished_jobs.wait(lock, [this] {
        return (busy == 0) && jobs.empty();
    });
}

void ThreadPool::enqueue(std::function<void()>& job) {
    ulock lock(queue_lock);
    jobs.emplace_back(job);
    new_job.notify_one();
}

const unsigned ThreadPool::getNumThreads() const noexcept {
    return num_threads;
}