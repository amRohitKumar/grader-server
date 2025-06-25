#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <pthread.h>
#include "threadPool.h"

ThreadPool::ThreadPool(int _size = std::thread::hardware_concurrency()): size(_size), stop(false){
    for(int i = 0; i < size; i++){
        workers.emplace_back([this]{
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock, [this]{ return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }

}

ThreadPool::~ThreadPool(){
    stop = true;
    condition.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}