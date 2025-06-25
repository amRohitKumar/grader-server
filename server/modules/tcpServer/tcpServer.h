#pragma once

#include <netinet/in.h>
#include <pthread.h>
#include "../threadPool/threadPool.h"
#include "../grader/grader.h"
#include "../connection/connection.h"
// #include "queue.h"

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define MAX_FILE_SIZE_BYTES 4

class TCPServer: public Connection{
    int serverPort, socketFD;
    struct sockaddr_in serverAddr, clientAddr;
    ThreadPool &pool;
    Grader &grader;
public:
    TCPServer(int, ThreadPool &, Grader &);
    void listenClients();
};