#pragma once

#include <string>

class TCPClient{
    int serverPort;
    int clientSocket;
    const std::string serverIp;
public:
    TCPClient(int, const std::string&);
    int getSocketFD();
    int sendData(const char*, size_t);
    int recvData(const char*, size_t);
    ~TCPClient();
};