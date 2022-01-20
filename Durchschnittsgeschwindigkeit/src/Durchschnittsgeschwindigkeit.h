#pragma once

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <netdb.h>
#include <nlohmann/json.hpp>

#include "../Libary/TCP/TCPClient.h"
#include "../Libary/message.pb.h"

namespace chrono = std::chrono;

namespace Sensor
{
    class Durchschnittsgeschwindigkeit
    {
    private:
        std::mt19937 random{};
        static std::string extract_verkehrssituation(TCP::Response response);

    protected:
        sockaddr_in tcp_server_addr{};

        int id{};
        double min_speed{};
        double max_speed{};
        double speed{};
        double average_speed{};
        double breaking_force;
        bool accelerate{false};
        int64_t start_time{-1};
        double distance{};
        int changepoint{};

    public:
        Durchschnittsgeschwindigkeit();
        std::string request_verkehrssituation();
        ::DurchschnittsgeschwindigkeitToHeadquarter generate_message(std::string verkehrssituation = "");
    };

    Durchschnittsgeschwindigkeit::Durchschnittsgeschwindigkeit()
    {
        std::uniform_real_distribution range{7.0, 260.0};
        speed = range(random);

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

    std::string Durchschnittsgeschwindigkeit::request_verkehrssituation()
    {
        auto client = TCP::Client(tcp_server_addr);

        TCP::Request request{};
        request.set_method("GET");
        request.set_path("/");
        request.set_version("HTTP/1.1");

        client.send(request);
        auto situation = extract_verkehrssituation(client.response());
        client.close();
        return situation;
    }

    ::DurchschnittsgeschwindigkeitToHeadquarter Durchschnittsgeschwindigkeit::generate_message(std::string verkehrssituation)
    {
        ::Durchschnittsgeschwindigkeit durchschnittsgeschwindigkeit{};

        //min max speed
        min_speed = 7.0;
        max_speed = 260;
        if (verkehrssituation == "Frei")
        {
            min_speed = 130.0;
            max_speed = 260.0;
        }
        else if (verkehrssituation == "maessiger Verkehr")
        {
            min_speed = 50.0;
            max_speed = 130.0;
        }
        else if (verkehrssituation == "starker Verkehr")
        {
            min_speed = 30.0;
            max_speed = 90.0;
        }
        else if (verkehrssituation == "Stau")
        {
            min_speed = 7.0;
            max_speed = 20.0;
        }
        durchschnittsgeschwindigkeit.set_min_speed(min_speed);
        durchschnittsgeschwindigkeit.set_max_speed(max_speed);

        //breaking force
        breaking_force = 1.0;
        if (speed > max_speed)
        {
            breaking_force = speed / max_speed;
        }
        durchschnittsgeschwindigkeit.set_breaking_force(breaking_force);

        //change point
        int changepoint = floor((speed - fmod(speed, 10)) / 10);

        //accelerate
        if (speed < min_speed)
        {
            accelerate = true;
        }
        else if (speed > max_speed)
        {
            accelerate = false;
        }
        if (Durchschnittsgeschwindigkeit::changepoint != changepoint)
        {
            Durchschnittsgeschwindigkeit::changepoint = changepoint;
            std::uniform_int_distribution range{0, 1};
            accelerate = range(random);
        }
        durchschnittsgeschwindigkeit.set_accelerate(accelerate);

        //acceleration
        std::uniform_real_distribution acceleration_range{(accelerate ? -1.0 : -2.5) * breaking_force, accelerate ? 2.5 : 1.0};
        auto acceleration = acceleration_range(random);
        durchschnittsgeschwindigkeit.set_acceleration(acceleration);

        //speed
        speed = speed + acceleration;
        durchschnittsgeschwindigkeit.set_speed(speed);

        //start time
        if (start_time == -1)
        {
            start_time = chrono::system_clock::now().time_since_epoch().count();
        }
        durchschnittsgeschwindigkeit.set_start_time(start_time);

        //distance
        static int64_t last_time = chrono::system_clock::now().time_since_epoch().count();
        int64_t time_nano = chrono::system_clock::now().time_since_epoch().count();
        distance += speed * (time_nano - last_time) / 3.6e12;
        durchschnittsgeschwindigkeit.set_distance(distance);
        last_time = time_nano;

        //average speed
        auto time_span = time_nano - start_time;
        average_speed = distance / time_span * 3.6e12;
        durchschnittsgeschwindigkeit.set_average_speed(average_speed);

        //type
        durchschnittsgeschwindigkeit.set_type("Durchschnittsgeschwindigkeit");

        ::DurchschnittsgeschwindigkeitToHeadquarter message{};

        message.mutable_data()->CopyFrom(durchschnittsgeschwindigkeit);

        //send time
        message.set_send(time_nano);

        return message;
    }

    std::string Durchschnittsgeschwindigkeit::extract_verkehrssituation(TCP::Response response)
    {
        auto json = nlohmann::json::parse(response.get_body());
        if (json.contains("verkehrssituation"))
        {
            auto data = json["verkehrssituation"];
            if (data.is_string())
            {
                return data;
            }
            else if (data.is_object())
            {
                if (data.contains("value"))
                {
                    return data["value"];
                }
            }
        }
        throw std::runtime_error("Durchschnittsgeschwindigkeit : Error while extracting information about verkehrssituation! ");
    }
} // namespace Sensor