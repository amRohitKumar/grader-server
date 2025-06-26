#pragma once

#include <netinet/in.h>
#include <pthread.h>
#include "../threadPool/threadPool.h"
#include "../../../modules/tcp/tcp.h"

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define MAX_FILE_SIZE_BYTES 4

class TCPServer: public ITCP{
    ThreadPool &pool;
public:
    TCPServer(int, ThreadPool &);

    template<class F>
    void listenClients(F&& f);
};

template<class F>
void TCPServer::listenClients(F&& f){
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    while(1){
        int clientSocketFD = accept(socketFD, (struct sockaddr *)&clientAddr, &clientAddrLen);

        if(clientSocketFD < 0){
            std::cerr<<"ERROR :: Client Socket Accept Failed\n";
            continue;
        }

        printf("Client connected successfully from :: %d:%d\n", ntohs(clientAddr.sin_port), clientSocketFD);

        std::function<void()> workerFn = std::bind(std::forward<F>(f), clientSocketFD);

        pool.enqueue(workerFn);
    }
}