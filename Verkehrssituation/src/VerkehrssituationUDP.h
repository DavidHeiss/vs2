#pragma once

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "Verkehrssituation.h"
#include "../Libary/message.pb.h"

constexpr size_t message_size = 2048;
constexpr auto server_name = "Zentrale";
constexpr u_int16_t server_port = 8002;

namespace Sensor
{
    class UDP : public Sensor::Verkehrssitation
    {
    private:
        int sock_fd;
        hostent host;

    public:
        UDP();
        ~UDP();
        void sendMsg(VerkehrssituationToHeadquarter message);
    };

    UDP::UDP()
    {
        int domain = AF_INET;     // IPv4 Internet protocols
        int type = SOCK_DGRAM;    // Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
        int protocol = PF_UNSPEC; // Unspecified

        sock_fd = socket(domain, type, protocol);
        if (sock_fd == -1)
        {
            throw std::runtime_error("Error while creating an endpoint for communication!");
        }

        auto host_pt = gethostbyname(server_name);
        if (host_pt == nullptr)
        {
            throw std::runtime_error(hstrerror(h_errno));
        }
        host = hostent(*host_pt);
    }

    UDP::~UDP()
    {
        close(sock_fd);
    }

    void UDP::sendMsg(VerkehrssituationToHeadquarter message)
    {

        sockaddr_in server_addr{};
        server_addr.sin_family = host.h_addrtype;
        memset(&server_addr.sin_addr.s_addr, 0, sizeof(server_addr.sin_addr.s_addr));
        memcpy(&server_addr.sin_addr.s_addr, host.h_addr, sizeof(host.h_addr));
        server_addr.sin_port = htons(server_port);

        message.set_to_address(inet_ntoa(server_addr.sin_addr));
        message.set_to_port(ntohs(server_addr.sin_port));

        auto message_s = message.SerializeAsString();

        char buffer[message_size]{};
        memset(&buffer, 0, sizeof(buffer));
        memcpy(&buffer, message_s.c_str(), message_s.size());

        if (sendto(sock_fd, &buffer, sizeof(buffer), 0, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            throw std::runtime_error("Error while sending a message on a socket!");
        }
    }
} // namespace Sensor