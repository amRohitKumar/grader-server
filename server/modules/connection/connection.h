#pragma once

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024
#define MAX_FILE_SIZE_BYTES 4

class Connection{
public:
    int sendData(int, const char*, size_t);
    int recvData(int, char*, size_t);
    int sendFile(int, const char*);
    void recvFile(int, const char*);
};