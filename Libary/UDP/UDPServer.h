#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <iostream>
#include <unistd.h>

#include "UDPResponse.h"

namespace UDP
{
    constexpr size_t message_size = 2048UL;

    class Server
    {
    private:
        int socket_fd{};

    public:
        explicit Server(u_int16_t port);
        Response receive();
        void close();
    };

    Server::Server(u_int16_t port)
    {
        socket_fd = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
        if (socket_fd == -1)
        {
            throw std::runtime_error("UDPServer : Error while creating an endpoint for communication! ");
        }

        sockaddr_in socket_addr{};
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        socket_addr.sin_port = htons(port);

        if (bind(socket_fd, (sockaddr *)&socket_addr, sizeof(socket_addr)) == -1)
        {
            throw std::runtime_error("UDPServer : Error while binding a name to a socket! ");
        }
    }

    Response Server::receive()
    {
        char buffer[message_size]{};

        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        if (recvfrom(socket_fd, &buffer, sizeof(buffer), 0, (sockaddr *)&client_addr, &client_addr_len) == -1)
        {
            throw std::runtime_error("UDPServer : Error while receiving a message from a socket! ");
        }

        Response response{};
        response.address = client_addr;

        auto size = message_size - 1;
        while (size > 0)
        {
            if (buffer[size] == '\000')
            {
                break;
            }
            size--;
        }
        response.message = std::string{buffer, size};

        return response;
    }

    void Server::close()
    {
        ::close(socket_fd);
    }
} // namespace UDP