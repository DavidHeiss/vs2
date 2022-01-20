#include <iostream>

#include "src/FuellstandMQTT.h"
#include "src/FuellstandUDP.h"

int main()
{
    try
    {
        auto type = getenv("type");
        if (type == nullptr)
        {
            throw std::runtime_error("Error while loading environment variable type! ");
        }

        auto interval = getenv("interval");
        if (interval == nullptr)
        {
            throw std::runtime_error("Error while loading environment variable interval! ");
        }
        auto ms = std::stoi(std::string(interval));

        if (std::string(type) == "MQTT")
        {
            Sensor::MQTT fuellstand{};
            while (true)
            {
                auto distance = fuellstand.request_durchschnitsgeschwindigkeit();

                if (distance > 0)
                {
                    auto message = fuellstand.generate_message(distance);
                    std::cout << message.DebugString() << std::endl;
                    fuellstand.sendMsg(message.SerializeAsString());
                }

                std::chrono::milliseconds sleep_time{ms};
                usleep(std::chrono::duration_cast<std::chrono::microseconds>(sleep_time).count());
            }
        }
        else if (std::string(type) == "UDP")
        {
            Sensor::UDP fuellstand{};
            while (true)
            {
                auto distance = fuellstand.request_durchschnitsgeschwindigkeit();

                if (distance > 0)
                {
                    auto message = fuellstand.generate_message(distance);
                    std::cout << message.DebugString() << std::endl;
                    fuellstand.sendMsg(message);
                }

                std::chrono::milliseconds sleep_time{ms};
                usleep(std::chrono::duration_cast<std::chrono::microseconds>(sleep_time).count());
            }
        }
    }
    catch (const std::exception &e)
    {
        perror(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}