#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include "tcpServer.h"


TCPServer::TCPServer(int _serverPort, ThreadPool &_pool, Grader &_grader): serverPort(_serverPort), pool(_pool), grader(_grader){
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD < 0){
        throw std::runtime_error("ERROR :: Socket Opening Failed");
    }
    memset(&serverAddr, 0, sizeof serverAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    if (bind(socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        throw std::runtime_error("ERROR :: Socket Binding Failed");
    }

    std::cout<<"Server is Live on Port :: "<<serverPort<<'\n';

    if (listen(socketFD, MAX_CLIENTS) < 0){
        throw std::runtime_error("ERROR :: Socket Listening Failed");
    }

    listenClients();
}

void TCPServer::listenClients(){
    unsigned int clientAddrLen = sizeof(clientAddr);
    while(1){
        int clientSocketFD = accept(socketFD, (struct sockaddr *)&clientAddr, &clientAddrLen);

        if(clientSocketFD < 0){
            std::cerr<<"ERROR :: Client Socket Accept Failed\n";
            continue;
        }

        printf("Client connected successfully from :: %d:%d\n", ntohs(clientAddr.sin_port), clientSocketFD);

        pool.enqueue([this, clientSocketFD]{
            grader.execute(clientSocketFD);
        });
        // pthread_mutex_lock(&requestQueue.queueLock);
        // requestQueue.enqueue(clientSocketFD);
        // pthread_mutex_unlock(&requestQueue.queueLock);
        // pthread_cond_signal(&requestQueue.queueCond);
    }
}

