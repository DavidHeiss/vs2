#include <iostream>
#include <chrono>
#include <unistd.h>

#include "src/Verkehrssituation.h"
#include "src/VerkehrssituationUDP.h"
#include "src/VerkehrssituationMQTT.h"
#include "Libary/message.pb.h"

int main()
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
        Sensor::MQTT vs{};
        while (true)
        {
            auto message = vs.generateMessage();

            std::cout << message.DebugString() << std::endl;
            vs.sendMsg(message.SerializeAsString());
            std::chrono::milliseconds sleep_time{ms};
            usleep(std::chrono::duration_cast<std::chrono::microseconds>(sleep_time).count());
        }
    }
    else if (std::string(type) == "UDP")
    {
        Sensor::UDP vs{};
        while (true)
        {
            auto message = vs.generateMessage();

            std::cout << message.DebugString() << std::endl;
            vs.sendMsg(message);
            std::chrono::milliseconds sleep_time{ms};
            usleep(std::chrono::duration_cast<std::chrono::microseconds>(sleep_time).count());
        }
    }
    return 0;
}