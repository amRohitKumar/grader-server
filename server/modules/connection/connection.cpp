#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include "connection.h"

int Connection::sendData(int clientFD, const char* buffer, size_t bufferSize){
    printf("Data to send: %s\n", buffer);
    int dataSent = send(clientFD, buffer, bufferSize, MSG_NOSIGNAL);
    if(dataSent < 0){
        throw std::runtime_error("Error sending data");
    }
    return dataSent;
}
int Connection::recvData(int clientFD, char* buffer, size_t bufferSize){
    int dataRead = recv(clientFD, buffer, bufferSize, 0);
    if(dataRead < 0){
        throw std::runtime_error("Error receiving data");
    }
    return dataRead;
}

int Connection::sendFile(int clientFD, const char* filePath){
    char buffer[BUFFER_SIZE]; //buffer to read  from  file
    bzero(buffer, BUFFER_SIZE); //initialize buffer to all NULLs
    FILE *file = fopen(filePath, "rb"); //open the file for reading, get file descriptor 
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }
		
		//for finding file size in bytes
    fseek(file, 0L, SEEK_END); 
    int file_size = ftell(file);
    // printf("File size is: %d\n", file_size);
    
    //Reset file descriptor to beginning of file
    fseek(file, 0L, SEEK_SET);
		
		//buffer to send file size to server
    char file_size_bytes[MAX_FILE_SIZE_BYTES];
    //copy the bytes of the file size integer into the char buffer
    memcpy(file_size_bytes, &file_size, sizeof(file_size));
    
    //send file size to server, return -1 if error
    if (sendData(clientFD, file_size_bytes, MAX_FILE_SIZE_BYTES) == -1)
    {
        std::cerr<<"Error sending file size\n";
        fclose(file);
        return -1;
    }

	//now send the source code file 
    while (!feof(file))  //while not reached end of file
    {
    
    		//read buffer from file
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE -1, file);
        
     		//send to server
        if (sendData(clientFD, buffer, BUFFER_SIZE) == -1)
        {
            std::cerr<<"Error sending file data";
            fclose(file);
            return -1;
        }
        
        //clean out buffer before reading into it again
        memset(buffer, 0, BUFFER_SIZE);
    }
    //close file
    fclose(file);
    return 0;
}
void Connection::recvFile(int clientFD, const char* filePath){
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    FILE *file = fopen(filePath, "wb");
    if (!file){
        throw std::runtime_error("ERROR :: FILE OPEN ERROR");
    }
    char file_size_bytes[MAX_FILE_SIZE_BYTES];
    if (recvData(clientFD, file_size_bytes, MAX_FILE_SIZE_BYTES) == -1)
    {
        fclose(file);
        throw std::runtime_error("ERROR :: FILE RECV ERROR");
    }

    int file_size;
    memcpy(&file_size, file_size_bytes, sizeof(file_size_bytes));

    printf("File size is: %d, File path is: %s\n", file_size, filePath);

    size_t bytes_read = 0, total_bytes_read = 0;
    while (true)
    {
        bytes_read = recvData(clientFD, buffer, BUFFER_SIZE);
        total_bytes_read += bytes_read;

        if (bytes_read <= 0)
        {
            fclose(file);
            throw std::runtime_error("ERROR :: FILE RECV ERROR");
        }
        fwrite(buffer, 1, bytes_read, file);
        memset(buffer, 0, BUFFER_SIZE);
        if (total_bytes_read >= file_size)
            break;
    }
    fclose(file);
}
