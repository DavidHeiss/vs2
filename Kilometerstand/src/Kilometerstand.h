#pragma once

#include <random>
#include <chrono>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <netdb.h>
#include <nlohmann/json.hpp>

#include "../Libary/TCP/TCPClient.h"
#include "../Libary/message.pb.h"

namespace Sensor
{
    class Kilometerstand
    {
    private:
        size_t id{};
        double start_distance{};
        double mileage{};
        int64_t send{};

        static double extract_distance(TCP::Response response);

    protected:
        sockaddr_in tcp_server_addr{};

    public:
        double request_durchschnitsgeschwindigkeit();
        ::KilometerstandToHeadquarter generate_message(double distance);

        Kilometerstand();
    };

    Kilometerstand::Kilometerstand()
    {
        std::mt19937 rng{};
        std::uniform_real_distribution<double> range{1000.0, 25000.0};
        start_distance = range(rng);

        auto hostent_server_p = gethostbyname("Zentrale");
        if (hostent_server_p == nullptr)
        {
            throw std::runtime_error(hstrerror(h_errno));
        }

        tcp_server_addr.sin_family = hostent_server_p->h_addrtype;
        memset(&tcp_server_addr.sin_addr.s_addr, 0, sizeof(tcp_server_addr.sin_addr.s_addr));
        memcpy(&tcp_server_addr.sin_addr.s_addr, hostent_server_p->h_addr, sizeof(hostent_server_p->h_addr));
        tcp_server_addr.sin_port = htons(8081);
    }

    double Kilometerstand::request_durchschnitsgeschwindigkeit()
    {
        auto client = TCP::Client(tcp_server_addr);

        TCP::Request request{};
        request.set_method("GET");
        request.set_path("/");
        request.set_version("HTTP/1.1");

        client.send(request);
        auto distance = extract_distance(client.response());
        client.close();
        return distance;
    }

    double Kilometerstand::extract_distance(TCP::Response response)
    {
        auto json = nlohmann::json::parse(response.get_body());
        if (json.contains("durchschnittsgeschwindigkeit"))
        {
            auto data = json["durchschnittsgeschwindigkeit"];
            if (data.is_string())
            {
                return -1.0;
            }
            else if (data.is_object())
            {
                if (data.contains("distance"))
                {
                    return data["distance"].get<double>();
                }
            }
        }
        throw std::runtime_error("Kilometerstand : Error while extracting information about durchschnittsgeschwindigkeit! ");
    }

    ::KilometerstandToHeadquarter Kilometerstand::generate_message(double distance)
    {
        ::Kilometerstand kilometerstand{};

        mileage = start_distance + distance;
        kilometerstand.set_mileage(mileage);

        kilometerstand.set_type("Kilometerstand");

        ::KilometerstandToHeadquarter message{};
        message.mutable_data()->CopyFrom(kilometerstand);

        message.set_send(std::chrono::system_clock::now().time_since_epoch().count());

        return message;
    }
} // namespace Sensor