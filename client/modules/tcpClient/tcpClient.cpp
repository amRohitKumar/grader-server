#include <string>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcpClient.h"  

TCPClient::TCPClient(int _serverPort, const std::string &_serverIp) : ITCP(_serverPort), serverIp(_serverIp)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    server_addr.sin_port = htons(getPort());

    if (connect(socketFD, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        close(socketFD);
        throw std::runtime_error("ERROR :: SERVER NOT RESPONDING");
    }
}