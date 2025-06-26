#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"

ITCP::ITCP(int _serverPort): serverPort(_serverPort){
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD < 0){
        throw std::runtime_error("ERROR :: SOCKET OPEN FAIL");
    }
}

int ITCP::getPort(){
    return serverPort;
}

int ITCP::getSocketFD(){
    return socketFD;
}

int ITCP::recvData(int _fd, char *buffer, size_t bufferSize){
    int dataRead = recv(_fd, buffer, bufferSize, 0);
    if(dataRead < 0){
        throw std::runtime_error("Error receiving data");
    }
    return dataRead;
}

int ITCP::sendData(int _fd, char *buffer, size_t bufferSize){
    int dataSent = send(_fd, buffer, bufferSize, MSG_NOSIGNAL);
    if(dataSent < 0){
        throw std::runtime_error("Error sending data");
    }
    return dataSent;
}

void ITCP::recvFile(int _fd, char *filePath){
    char buffer[BUFFER_SIZE];
    char file_size_bytes[MAX_FILE_SIZE_BYTES];
    memset(buffer, 0, BUFFER_SIZE);
    FILE *file = fopen(filePath, "wb");
    if (!file){
        throw std::runtime_error("ERROR :: FILE OPEN ERROR");
    }

    try
    {
        recvData(_fd, file_size_bytes, MAX_FILE_SIZE_BYTES);
    }
    catch(const std::exception& e)
    {
        fclose(file);
        std::cerr << e.what() << '\n';
        throw std::runtime_error("ERROR :: FILE SIZE RECV ERROR");
    }

    size_t fileSize;
    memcpy(&fileSize, file_size_bytes, MAX_FILE_SIZE_BYTES);

    int bytesRead = 0, totalBytesRead = 0;
    while (totalBytesRead < fileSize)
    {
        try
        {
            bytesRead = recvData(_fd, buffer, BUFFER_SIZE);
        } catch(const std::runtime_error& e){
            fclose(file);
            std::cerr << e.what() << '\n';
            throw std::runtime_error("ERROR :: FILE RECV ERROR 1");
        }
        
        if (bytesRead <= 0)
        {
            fclose(file);
            throw std::runtime_error("ERROR :: FILE RECV ERROR 2");
        }

        fwrite(buffer, 1, bytesRead, file);
        memset(buffer, 0, BUFFER_SIZE);
        totalBytesRead += bytesRead;
    }
    fclose(file);
}

void ITCP::sendFile(int _fd, char *filePath){
    char buffer[BUFFER_SIZE];
    char file_size_bytes[MAX_FILE_SIZE_BYTES];
    memset(buffer, 0, BUFFER_SIZE);
    FILE *file = fopen(filePath, "rb");
    if (!file)
    {
        throw std::runtime_error("ERROR :: FILE OPEN ERROR");
    }
		
    fseek(file, 0L, SEEK_END); 
    int file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    memcpy(file_size_bytes, &file_size, sizeof(file_size));
    
    try
    {
        sendData(_fd, file_size_bytes, MAX_FILE_SIZE_BYTES);
    }
    catch(const std::runtime_error& e)
    {
        fclose(file);
        std::cerr << e.what() << '\n';
        throw std::runtime_error("ERROR :: FILE SIZE SEND ERROR");
    }

    while (!feof(file))
    {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
        
        try
        {
            sendData(_fd, buffer, BUFFER_SIZE);
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what() << '\n';
            fclose(file);
            throw std::runtime_error("ERROR :: FILE SEND ERROR");
        }
        memset(buffer, 0, BUFFER_SIZE);
    }
    fclose(file);
}

void ITCP::recvFileConsole(){
    char buffer[BUFFER_SIZE];
    char file_size_bytes[MAX_FILE_SIZE_BYTES];
    int rcv_bytes;
    memset(buffer,0,BUFFER_SIZE);
    while (true){
        try{
            rcv_bytes = recvData(socketFD, buffer, BUFFER_SIZE);
        } catch(const std::runtime_error& e){
            std::cerr << e.what() << '\n'; 
            break;
        }
        if (rcv_bytes <= 0) break;
        printf("%s\n", buffer);
        memset(buffer,0,BUFFER_SIZE);
    }
    if (rcv_bytes < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            perror("Time out : No response from server : ");
        }
        else {
            perror("No response from server : ");
        }    
    }
    else {
        printf("----response successfully recieved-----\n");
    }
}

ITCP::~ITCP(){
    close(socketFD);
}