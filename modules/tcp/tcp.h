#pragma once

#include <sys/types.h>

#define BUFFER_SIZE 1024
#define MAX_FILE_SIZE_BYTES 4

class ITCP{
    int serverPort;
protected:
    int socketFD;
public:
    ITCP(int);
    int getPort();
    int getSocketFD();
    int sendData(int, char*, size_t);
    int recvData(int, char*, size_t);
    void sendFile(int, char*);
    void recvFile(int, char*);
    void recvFileConsole();
    ~ITCP();
};