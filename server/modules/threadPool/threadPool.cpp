#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <pthread.h>
#include "threadPool.h"

ThreadPool::ThreadPool(int _size = 4): size(_size), stop(false){
    for(int i = 0; i < size; i++){
        workers.emplace_back([this]{
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock, [this]{ return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) return;
                    printf("Starting worker\n");
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }

}



// void *ThreadPool::worker(void *arg){
//     ThreadPool *self = static_cast<ThreadPool*>(arg);
//     self->workerClient();
//     return nullptr;
// }

// void ThreadPool::workerClient(){
//     pthread_mutex_lock(&requestQueue.queueLock);
//     while(requestQueue.isEmpty()){
//         pthread_cond_wait(&requestQueue.queueCond, &requestQueue.queueLock);
//     }
//     int clientSockFD = requestQueue.dequeue();
//     pthread_mutex_unlock(&requestQueue.queueLock);

    

//     // if (grader(clientSockFD) == 0)
//     //     printf("SUCCESS :: Client File Graded for Client Socket with FD = %d\n", clientSockFD);
//     // else
//     //     printf("ERROR :: Client File Cannot Be Graded for Client Socket with FD = %d\n", clientSockFD);
//     close(clientSockFD);
// }