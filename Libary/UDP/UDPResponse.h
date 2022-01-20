#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

namespace UDP
{
    struct Response
    {
        sockaddr_in address;
        std::string message;

        u_int16_t get_port();
        std::string get_addr();
    };

    u_int16_t Response::get_port()
    {
        return ntohs(address.sin_port);
    }

    std::string Response::get_addr()
    {
        return std::string{inet_ntoa(address.sin_addr)};
    }
} // namespace UDP