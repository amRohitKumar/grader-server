#include <iostream>
#include <errno.h>
#include <string>
#include "cstring"
#include "fileTransfer.h"

FileTransfer::FileTransfer(TCPClient& _client): client(_client){}

int FileTransfer::sendFile(const std::string& filePath){
    memset(buffer, 0, sizeof buffer);
    FILE *file = fopen(filePath.c_str(), "rb");
    if(!file){
        std::cerr<<"Error opening file\n";
        return -1;
    }
    fseek(file, 0L, SEEK_END);
    int fileSize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    memcpy(file_size_bytes, &fileSize, MAX_FILE_SIZE_BYTES);

    try{
        printf("File size: %d\n", fileSize);
        client.sendData(file_size_bytes, MAX_FILE_SIZE_BYTES);
    } catch(const std::runtime_error& e){
        std::cerr << e.what() << '\n';
        fclose(file);
        return -1;
    }
    memset(buffer, 0, BUFFER_SIZE);
    while(!feof(file)){
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
        if(bytes_read > 0){
            try{
                client.sendData(buffer, BUFFER_SIZE);
            } catch(const std::runtime_error& e){
                std::cerr << e.what() << '\n';
                fclose(file);
                return -1;
            }
            memset(buffer, 0, BUFFER_SIZE);
        }
    }

    fclose(file);
    return 0;
}

int FileTransfer::recvFile(const std::string& filePath){
    memset(buffer, 0, BUFFER_SIZE);
    FILE *file = fopen(filePath.c_str(), "wb");

    if(!file){
        std::cerr<<"Error writing file\n";
        return -1;
    }

    try{
        client.recvData(file_size_bytes, BUFFER_SIZE);
    } catch(const std::runtime_error& e){
        std::cerr << e.what() << '\n';
        fclose(file);
        return -1;
    }

    int fileSize;
    memcpy(&fileSize, file_size_bytes, sizeof(file_size_bytes));

    std::cout<<"File size is: "<<fileSize<<'\n';

    size_t bytesRead = 0, totalBytesRead = 0;
    while (true)
    {
        try{
            bytesRead = client.recvData(buffer, BUFFER_SIZE);
        } catch(const std::runtime_error& e){
            std::cerr << e.what() << '\n';
            fclose(file);
            return -1;
        }
        totalBytesRead += bytesRead;

        fwrite(buffer, 1, bytesRead, file);
        memset(buffer, 0, BUFFER_SIZE);
        if (totalBytesRead >= fileSize)
            break;
    }
    fclose(file);
    return 0;
}

void FileTransfer::recvFileConsole(){
    int rcv_bytes;
    memset(buffer,0,BUFFER_SIZE);
    while (true){
        try{
            rcv_bytes = client.recvData(buffer, BUFFER_SIZE);
        } catch(const std::runtime_error& e){
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