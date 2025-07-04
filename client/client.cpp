#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include "modules/tcpClient/tcpClient.h"


std::pair<std::string, int> parseServerAddress(const std::string& serverAddress){
    size_t colonPos = serverAddress.find(':');
    if(colonPos == std::string::npos){
        throw std::runtime_error("Invalid server address format. Usage <ip:port>\n");
    }
    
    std::string ipAddress = serverAddress.substr(0, colonPos);
    int serverPort = std::stoi(serverAddress.substr(colonPos+1));
    return {ipAddress, serverPort};
}

int main(int argc, char *argv[]){
    if(argc != 3){
        std::cerr<<"Usage: <server ip:port> <filePath>\n";
        return -1;
    }

    auto [serverIp, serverPort] = parseServerAddress(argv[1]);
    int timeOutTime = 2; // seconds

    struct timeval timeout;
    timeout.tv_sec = timeOutTime;
    timeout.tv_usec = 0;
    
    TCPClient client(serverPort, serverIp);

    try
    {
        client.sendFile(client.getSocketFD(), argv[2]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout<<"Code sent for grading, waiting for response\n";

    if (setsockopt(client.getSocketFD(), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed : ");
        close(client.getSocketFD());
        throw std::runtime_error("setsockopt failed");
    }
    std::cout<<"----- Server Response -------- \n";
    client.recvFileConsole();
    return 0;
}