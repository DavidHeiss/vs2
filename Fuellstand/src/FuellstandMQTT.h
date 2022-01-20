#pragma once

#include <string>
#include <mqtt/client.h>

#include "Fuellstand.h"

namespace Sensor
{
    class MQTT : public Fuellstand
    {
    private:
        mqtt::client client;

    public:
        MQTT();
        void sendMsg(std::string message);
    };

    MQTT::MQTT() : client{"tcp://Mosquitto:1883", "sensor_fuellstand"}
    {
        mqtt::connect_options options{};
        client.connect(options);

    }

    void MQTT::sendMsg(std::string message)
    {
        client.publish("fuellstand", &message[0], message.size() + 1);
    }

} // namespace Sensor