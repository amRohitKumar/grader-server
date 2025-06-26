#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include "modules/grader/grader.h"
#include "modules/tcpServer/tcpServer.h"
#include "modules/threadPool/threadPool.h"

class Application
{
    ThreadPool pool;
    TCPServer tcpServer;
    Grader grader;

public:
    Application(int _port, int _poolSize)
    try : pool(_poolSize),
          tcpServer(_port, pool), grader(tcpServer)
    {
        tcpServer.listenClients(std::bind(&Grader::execute, &grader, std::placeholders::_1));
    } catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        throw std::runtime_error("ERROR :: INIT APP ERROR");
    }
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: <server port> <threadPoolSize>\n";
        return -1;
    }
    int serverPort = atoi(argv[1]), threadPoolSize = atoi(argv[1]);
    Application app(serverPort, threadPoolSize);
    return 0;
}