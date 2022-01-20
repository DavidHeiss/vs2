#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

#include "TCPRequest.h"
#include "TCPResponse.h"

namespace TCP
{
    class Client
    {
    private:
        int socket_fd;
        sockaddr_in socket_addr;

    public:
        Client(int fd, sockaddr_in addr);
        Client(sockaddr_in addr);
        Request receive() const;
        Response response() const;
        void send(Response response);
        void send(Request request);
        void send_raw(std::string message);
        std::string receive_raw();
        std::string get_addr();
        u_int16_t get_port();
        void close();
    };

    Client::Client(int fd, sockaddr_in addr)
    {
        socket_fd = fd;
        socket_addr = addr;
    }

    Client::Client(sockaddr_in addr)
    {
        socket_addr = addr;

        socket_fd = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);
        if (socket_fd == -1)
        {
            throw std::runtime_error("TCP Client : Error while creating an endpoint for communication! ");
        }

        if (connect(socket_fd, (sockaddr *)&socket_addr, sizeof(socket_addr)) == -1)
        {
            throw std::runtime_error("TCP Client : Error while initiating a connection on a socket! ");
        }
    }

    Request Client::receive() const
    {
        char buffer[TCP::message_size]{};

        if (recv(socket_fd, &buffer, sizeof(buffer), 0) == -1)
        {
            throw std::runtime_error("TCP Client : Error while receiving a message from a socket! ");
        }

        return Request(buffer);
    }

    Response Client::response() const
    {
        char buffer[TCP::message_size]{};

        if (recv(socket_fd, &buffer, sizeof(buffer), 0) == -1)
        {
            throw std::runtime_error("TCP Client : Error while receiving a message from a socket! ");
        }

        return Response(buffer);
    }

    void Client::send(Response response)
    {
        std::string response_s = response.get_raw_message();
        if (::send(socket_fd, &response_s[0], response_s.size(), 0) == -1)
        {
            throw std::runtime_error("TCP Client : Error while sending a message on a socket! ");
        }
    }

    void Client::send(Request request)
    {
        std::string request_s = request.create_raw_message();
        send_raw(request_s);
    }

    void Client::send_raw(std::string message)
    {
        if (::send(socket_fd, &message[0], message.size(), 0) == -1)
        {
            throw std::runtime_error("TCP Client : Error while sending a message on a socket! ");
        }
    }

    std::string Client::receive_raw()
    {
        char buffer[TCP::message_size]{};

        if (recv(socket_fd, &buffer, sizeof(buffer), 0) == -1)
        {
            throw std::runtime_error("TCP Client : Error while receiving a message from a socket! ");
        }

        auto size = sizeof(buffer) - 1;
        while (size > 0)
        {
            auto c = buffer[size];
            if (c != '\000')
            {
                break;
            }
            --size;
        }
        std::string s{buffer, size + 1};
        return s;
    }

    std::string Client::get_addr()
    {
        return std::string{inet_ntoa(socket_addr.sin_addr)};
    }

    u_int16_t Client::get_port()
    {
        return ntohs(socket_addr.sin_port);
    }

    void Client::close()
    {
        if (::close(socket_fd) == -1)
        {
            throw std::runtime_error("TCP Client : Error while closeing a file descriptor! ");
        }
    }
} // namespace TCP