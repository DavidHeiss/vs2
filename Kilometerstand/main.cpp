#include <iostream>

#include <string>
#include "src/KilometerstandUDP.h"
#include "src/KilometerstandMQTT.h"

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
            printf("%s\n", type);
            Sensor::MQTT mileage{};
            while (true)
            {
                auto distance = mileage.request_durchschnitsgeschwindigkeit();

                if (distance > 0)
                {
                    auto message = mileage.generate_message(distance);
                    std::cout << message.DebugString() << std::endl;
                    mileage.sendMsg(message.SerializeAsString());
                }

                std::chrono::milliseconds sleep_time{ms};
                usleep(std::chrono::duration_cast<std::chrono::microseconds>(sleep_time).count());
            }
        }

        if (std::string(type) == "UDP")
        {
            printf("%s\n", type);
            Sensor::UDP mileage{};
            while (true)
            {
                auto distance = mileage.request_durchschnitsgeschwindigkeit();

                if (distance > 0)
                {
                    auto message = mileage.generate_message(distance);
                    std::cout << message.DebugString() << std::endl;
                    mileage.sendMsg(message);
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