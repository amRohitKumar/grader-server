#pragma once

#include <string>
#include "../../../modules/tcp/tcp.h"

class TCPClient: public ITCP{
    const std::string serverIp;
public:
    TCPClient(int, const std::string&);
};