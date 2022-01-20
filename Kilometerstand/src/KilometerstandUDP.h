#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <unistd.h>

#include "Kilometerstand.h"
#include "../Libary/message.pb.h"

namespace Sensor
{
    class UDP : public Kilometerstand
    {
    private:
        int udp_socket_fd;

    public:
        UDP();
        void close();
        void sendMsg(KilometerstandToHeadquarter msg);
    };

    UDP::UDP() : Kilometerstand{}
    {
        udp_socket_fd = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC);
        if (udp_socket_fd == -1)
        {
            throw std::runtime_error("KilometerstandUDP : Error while creating an endpoint for communication! ");
        }
    }

    void UDP::sendMsg(KilometerstandToHeadquarter msg)
    {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = tcp_server_addr.sin_addr.s_addr;
        addr.sin_port = htons(8004);

        msg.set_to_address(inet_ntoa(addr.sin_addr));
        msg.set_to_port(ntohs(addr.sin_port));

        auto message = msg.SerializeAsString();

        if (sendto(udp_socket_fd, &message[0], message.size(), 0, (sockaddr *)&addr, sizeof(addr)) == -1)
        {
            throw std::runtime_error("KilometerstandUDP : Error while sending a message on a socket! ");
        }
    }

    void UDP::close()
    {
        if (::close(udp_socket_fd) == -1)
        {
            throw std::runtime_error("KilometerstandUDP : Error while closing a file descriptor! ");
        }
    }
}; // namespace Sensor