#pragma once
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>
#include <functional>

using ulock = std::unique_lock<std::mutex>;
class ThreadPool {
private:
    std::deque <std::function<void()>> jobs;
    std::mutex queue_lock;
    std::condition_variable new_job;
    std::condition_variable finished_jobs;
    std::vector <std::thread> threads;
    bool done;
    unsigned int busy;

    void thread_loop();

public:
    explicit ThreadPool();

    ~ThreadPool();

    void wait_for_jobs();

    void enqueue(std::function<void()> &job);
};
