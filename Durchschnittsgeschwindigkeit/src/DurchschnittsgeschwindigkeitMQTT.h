#pragma once

#include <string>
#include <mqtt/client.h>

#include "Durchschnittsgeschwindigkeit.h"

namespace Sensor
{
    class MQTT : public Durchschnittsgeschwindigkeit
    {
    private:
        mqtt::client client;

    public:
        MQTT();
        void sendMsg(std::string message);
    };

    MQTT::MQTT() : client{"tcp://Mosquitto:1883", "sensor_durchschnittsgeschwindigkeit"}
    {
        mqtt::connect_options options{};
        client.connect(options);
    }

    void MQTT::sendMsg(std::string message)
    {
        client.publish("durchschnittsgeschwindigkeit", &message[0], message.size() + 1);
    }

} // namespace Sensor