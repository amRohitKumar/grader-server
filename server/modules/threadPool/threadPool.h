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

    template<class F, class... Args>
    void enqueue(F&& f, Args&&... args);

    ~ThreadPool();
};

template<class F, class... Args>
void ThreadPool::enqueue(F&& f, Args&&... args) {
    std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(std::move(task));
    }
    condition.notify_one();
}