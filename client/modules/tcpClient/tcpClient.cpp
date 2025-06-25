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

TCPClient::TCPClient(int _serverPort, const std::string &_serverIp) : serverPort(_serverPort), serverIp(_serverIp)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    server_addr.sin_port = htons(serverPort);

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Error creating socket\n";
        close(clientSocket);
        throw std::runtime_error("Error creating socket");
    }

    if (connect(clientSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Server not responding\n";
        close(clientSocket);
        throw std::runtime_error("Server not responding");
    }
}

int TCPClient::getSocketFD(){
    return clientSocket;
}

int TCPClient::sendData(const char *buffer, size_t bufferSize){
    printf("Data to send: %s\n", buffer);
    int dataSent = send(clientSocket, buffer, bufferSize, MSG_NOSIGNAL);
    if(dataSent < 0){
        throw std::runtime_error("Error sending data");
    }
    return dataSent;
}

int TCPClient::recvData(const char *buffer, size_t bufferSize){
    int dataRead = recv(clientSocket, (void *)buffer, bufferSize, 0);
    if(dataRead < 0){
        throw std::runtime_error("Error receiving data");
    }
    return dataRead;
}

TCPClient::~TCPClient(){
    std::cout<<"Closing client socket\n";
    close(clientSocket);
}