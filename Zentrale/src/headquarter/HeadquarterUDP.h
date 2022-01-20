#pragma once

#include <vector>
#include <thread>
#include <chrono>
#include <variant>
#include <variant>

#include "Headquarter.h"
#include "../../Libary/UDP/UDPServer.h"
#include "../../Libary/message.pb.h"

namespace Headquarter
{
    class UDP : public Headquarter
    {
    private:
        static void start_verkehrssituation(u_int16_t port, History *history);
        static void start_durchschnittsgeschwindigkeit(u_int16_t port, History *history);
        static void start_kilometerstand(u_int16_t port, History *history);
        static void start_fuellstand(u_int16_t port, History *history);
        std::vector<std::thread> threads{};

    public:
        UDP();
        void join() override;
    };

    UDP::UDP() : Headquarter{}
    {
        threads.emplace_back(start_verkehrssituation, 8002, &verkehrssituation);
        threads.emplace_back(start_durchschnittsgeschwindigkeit, 8003, &durchschnittsgeschwindigkeit);
        threads.emplace_back(start_kilometerstand, 8004, &kilometerstand);
        threads.emplace_back(start_fuellstand, 8005, &fuellstand);
    }

    void UDP::join()
    {
        Headquarter::join();
        for (auto &thread : threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    void UDP::start_verkehrssituation(u_int16_t port, History *history)
    {
        try
        {
            ::UDP::Server server{port};

            while (true)
            {
                auto response = server.receive();
                VerkehrssituationToHeadquarter message{};
                message.ParseFromString(response.message);

                uint64_t received = get_time();
                auto send = message.send();

                message.set_received(received);
                message.set_latency(received - send);
                message.set_from_address(response.get_addr());
                message.set_from_port(response.get_port());

                std::cout << message.DebugString() << std::endl;

                std::lock_guard<std::mutex>{history->mutex};
                history->data.push_back(message.SerializeAsString());
            }

            server.close();
        }
        catch (std::exception &e)
        {
            perror(e.what());
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    void UDP::start_durchschnittsgeschwindigkeit(u_int16_t port, History *history)
    {
        try
        {
            ::UDP::Server server{port};

            while (true)
            {
                auto response = server.receive();
                DurchschnittsgeschwindigkeitToHeadquarter message{};
                message.ParseFromString(response.message);

                uint64_t received = get_time();
                auto send = message.send();

                message.set_received(received);
                message.set_latency(received - send);
                message.set_from_address(response.get_addr());
                message.set_from_port(response.get_port());

                std::cout << message.DebugString() << std::endl;

                std::lock_guard<std::mutex>{history->mutex};
                history->data.push_back(message.SerializeAsString());
            }

            server.close();
        }
        catch (std::exception &e)
        {
            perror(e.what());
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    void UDP::start_kilometerstand(u_int16_t port, History *history)
    {
        try
        {
            ::UDP::Server server{port};

            while (true)
            {
                auto response = server.receive();
                KilometerstandToHeadquarter message{};
                message.ParseFromString(response.message);

                uint64_t received = get_time();
                auto send = message.send();

                message.set_received(received);
                message.set_latency(received - send);
                message.set_from_address(response.get_addr());
                message.set_from_port(response.get_port());

                std::cout << message.DebugString() << std::endl;

                std::lock_guard<std::mutex>{history->mutex};
                history->data.push_back(message.SerializeAsString());
            }

            server.close();
        }
        catch (std::exception &e)
        {
            perror(e.what());
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    void UDP::start_fuellstand(u_int16_t port, History *history)
    {
        try
        {
            ::UDP::Server server{port};

            while (true)
            {
                auto response = server.receive();
                FuellstandToHeadquarter message{};
                message.ParseFromString(response.message);

                uint64_t received = get_time();
                auto send = message.send();

                message.set_received(received);
                message.set_latency(received - send);
                message.set_from_address(response.get_addr());
                message.set_from_port(response.get_port());

                std::cout << message.DebugString() << std::endl;

                std::lock_guard<std::mutex>{history->mutex};
                history->data.push_back(message.SerializeAsString());
            }

            server.close();
        }
        catch (std::exception &e)
        {
            perror(e.what());
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
} // namespace Headquarter