#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include "modules/grader.h"
#include "modules/tcpServer.h"
#include "modules/threadPool.h"

class Application{
    Grader grader;
    ThreadPool pool;
    TCPServer tcpServer;
public:
    Application(int _port, int _poolSize): grader(), pool(_poolSize), tcpServer(_port, pool, grader){}
};

int main(int argc, char *argv[]){
    if(argc != 3){
        std::cerr<<"Usage: <server port> <threadPoolSize>\n";
        return -1;
    }
    int serverPort = atoi(argv[1]), threadPoolSize = atoi(argv[1]);
    Application app(serverPort, threadPoolSize);
    return 0;
}