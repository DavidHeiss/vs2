#pragma once

#include <string>
#include <mqtt/client.h>

#include "Kilometerstand.h"

namespace Sensor
{
    class MQTT : public Kilometerstand
    {
    private:
        mqtt::client client;

    public:
        MQTT();
        void sendMsg(std::string message);
    };

    MQTT::MQTT() : client{"tcp://Mosquitto:1883", "sensor_kilometerstand"}
    {
        mqtt::connect_options options{};
        client.connect(options);
    }

    void MQTT::sendMsg(std::string message)
    {
        client.publish("kilometerstand", &message[0], message.size() + 1);
    }

} // namespace Sensor