#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <queue>
// #include "queue.h"

class ThreadPool{
    int size;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
    std::vector<std::thread> workers;
public:
    explicit ThreadPool(int);

    template<class F>
    void enqueue(F&& f);

    ~ThreadPool();
};

template<class F>
void ThreadPool::enqueue(F&& f) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(std::forward<F>(f));
    }
    condition.notify_one();
}