#include "threadpool.h"
#include <iostream>

constexpr int N = 100000;
static std::vector<int> mtrx(N);

void initMatrix() {
    for (auto i = 0; i < N; ++i)
            mtrx.at(i) = 4;
}
std::mutex lockl;
static unsigned long long sum = 0;
static unsigned long long sum_pool = 0;

void sumMatrix() {
    for(auto i = 0; i < mtrx.size(); ++i)
        sum += mtrx.at(i) + mtrx.at(i);
}

void sumMatrixPool() {
    unsigned long long s = 0;
    for(auto i = 0; i < mtrx.size(); ++i)
        s += mtrx.at(i) + mtrx.at(i);
    ulock l(lockl);
    sum_pool += s;
}

int main(void) {
    initMatrix();
    ThreadPool pool;

    std::cout << "Allocated: " << pool.getNumThreads() << " threads!" << std::endl;
    // Measure time for ThreadPool execution
    auto start = std::chrono::high_resolution_clock::now();
    std::function<void()> job = std::bind(&sumMatrixPool);
    for (int i = 0; i < N; ++i)
        pool.enqueue(job);

    pool.wait_for_jobs();
    auto end = std::chrono::high_resolution_clock::now();
    auto poolTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    //   std::cout << "Sequential execution time: " << sequentialTime << " seconds" << "\nsequential ans: " << sum <<std::endl;
    std::cout << "ThreadPool execution time: " << poolTime << " seconds" << "\npool ans: " << sum_pool <<std::endl;
    return 0;
}