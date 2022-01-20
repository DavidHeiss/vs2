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
    class Fuellstand
    {
    private:
        std::mt19937 rng;
        static double extract_distance(TCP::Response response);

    protected:
        sockaddr_in tcp_server_addr{};

        double capacity{};
        double current_capacity{};
        double level{};
        double ussage{};
        double will_refill_capacity{};
        double last_refueld{};
        double consumption_100km{};

    public:
        Fuellstand();
        double request_durchschnitsgeschwindigkeit();
        ::FuellstandToHeadquarter generate_message(double distance);
    };

    Fuellstand::Fuellstand()
    {
        rng = std::mt19937{};
        std::uniform_real_distribution<double> range_ca{50, 200};
        capacity = range_ca(rng);
        current_capacity = capacity;
        std::uniform_real_distribution<double> range_c{10, 30};
        consumption_100km = range_c(rng);

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

    double Fuellstand::request_durchschnitsgeschwindigkeit()
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

    double Fuellstand::extract_distance(TCP::Response response)
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
        throw std::runtime_error("Fuellstand : Error while extracting information about durchschnittsgeschwindigkeit! ");
    }

    ::FuellstandToHeadquarter Fuellstand::generate_message(double distance)
    {
        ::Fuellstand fuellstand{};

        fuellstand.set_capacity(capacity);

        fuellstand.set_last_refueld(last_refueld);

        fuellstand.set_consumption(consumption_100km);

        ussage = consumption_100km * (distance - last_refueld) / 1e5;
        fuellstand.set_ussage(ussage);

        current_capacity -= ussage;
        fuellstand.set_current_capacity(current_capacity);

        std::uniform_real_distribution<double> refill_range{0.0, 30.0};
        will_refill_capacity = refill_range(rng);
        fuellstand.set_will_refill(will_refill_capacity);

        if (current_capacity <= will_refill_capacity)
        {
            std::uniform_int_distribution chance{0, 1};
            auto roll = chance(rng);
            if (roll)
            {
                current_capacity = capacity;
            }
            else
            {
                std::uniform_real_distribution percentage{0.5, 1.0};
                auto value = percentage(rng);
                current_capacity = capacity * value;
            }
            last_refueld = distance;
        }

        level = current_capacity / capacity;
        fuellstand.set_level(level);

        fuellstand.set_type("Fuellstand");

        FuellstandToHeadquarter message{};

        message.mutable_data()->CopyFrom(fuellstand);

        message.set_send(std::chrono::system_clock::now().time_since_epoch().count());

        return message;
    }
} // namespace Sensor