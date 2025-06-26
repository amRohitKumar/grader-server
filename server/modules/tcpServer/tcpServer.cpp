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


TCPServer::TCPServer(int _serverPort, ThreadPool &_pool): ITCP(_serverPort), pool(_pool){
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof serverAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_serverPort);

    if (bind(socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        throw std::runtime_error("ERROR :: Socket Binding Failed");
    }

    std::cout<<"Server is Live on Port :: "<<_serverPort<<'\n';

    if (listen(socketFD, MAX_CLIENTS) < 0){
        throw std::runtime_error("ERROR :: Socket Listening Failed");
    }
}