#pragma once

#include <string>
#include "tcpClient.h"

#define BUFFER_SIZE 1024
#define MAX_FILE_SIZE_BYTES 4

class FileTransfer{
    char buffer[BUFFER_SIZE];
    char file_size_bytes[MAX_FILE_SIZE_BYTES];
    TCPClient &client;
public:
    FileTransfer(TCPClient &);
    int sendFile(const std::string&);
    int recvFile(const std::string&);
    void recvFileConsole();
};