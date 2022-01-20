#pragma once

#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "TCPClient.h"

namespace TCP
{
    class Server
    {
    private:
        int socket_fd{};
        sockaddr_in socket_addr{};

    public:
        void listen(u_int16_t port, int backlog = 5);
        Client accept();
        void close();
    };

    void Server::listen(u_int16_t port, int backlog)
    {
        socket_fd = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);
        if (socket_fd == -1)
        {
            throw std::runtime_error("TCPServer : Error while creating an endpoint for communication! ");
        }

        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        socket_addr.sin_port = htons(port);

        if (bind(socket_fd, (sockaddr *)&socket_addr, sizeof(socket_addr)) == -1)
        {
            throw std::runtime_error("TCPServer : Error while binding a name to a socket! ");
        }

        if (::listen(socket_fd, backlog) == -1)
        {
            throw std::runtime_error("TCPServer : Error while listening for connections on a socket! ");
        }
    }

    Client Server::accept()
    {
        sockaddr_in client_addr{};
        socklen_t client_addr_size = sizeof(client_addr);

        int client_fd = ::accept(socket_fd, (sockaddr *)&client_addr, &client_addr_size);
        if (client_fd == -1)
        {
            throw std::runtime_error("TCPServer : Error while accepting a connection on a socket! ");
        }

        return Client{client_fd, client_addr};
    }

    void Server::close()
    {
        if (::close(socket_fd) == -1)
        {
            throw std::runtime_error("TCPServer : Error while closing a file descriptor! ");
        }
    }

} // namespace TCP