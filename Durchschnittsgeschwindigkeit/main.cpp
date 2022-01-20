#include <iostream>
#include <unistd.h>

#include "src/DurchschnittsgeschwindigkeitUDP.h"
#include "src/DurchschnittsgeschwindigkeitMQTT.h"

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
            Sensor::MQTT durchschnittsgeschwindigkeit{};
            while (true)
            {
                auto value = durchschnittsgeschwindigkeit.request_verkehrssituation();
                if (value != "empty")
                {
                    auto message = durchschnittsgeschwindigkeit.generate_message(value);
                    std::cout << message.DebugString() << std::endl;
                    durchschnittsgeschwindigkeit.sendMsg(message.SerializeAsString());
                }
                std::chrono::milliseconds sleep_time{ms};
                usleep(std::chrono::duration_cast<std::chrono::microseconds>(sleep_time).count());
            }
        }
        else if (std::string(type) == "UDP")
        {
            Sensor::UDP durchschnittsgeschwindigkeit{};
            while (true)
            {
                auto value = durchschnittsgeschwindigkeit.request_verkehrssituation();
                if (value != "empty")
                {
                    auto message = durchschnittsgeschwindigkeit.generate_message(value);
                    std::cout << message.DebugString() << std::endl;
                    durchschnittsgeschwindigkeit.sendMsg(message);
                }
                std::chrono::milliseconds sleep_time{ms};
                usleep(std::chrono::duration_cast<std::chrono::microseconds>(sleep_time).count());
            }
        }
    }
    catch (const std::exception &e)
    {
        perror(e.what());
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}