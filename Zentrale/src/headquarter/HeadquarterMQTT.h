#pragma once

#include <vector>
#include <thread>
#include <codecvt>
#include <chrono>
#include <nlohmann/json.hpp>

#include <mqtt/client.h>

namespace Headquarter
{
    class MQTT : public Headquarter
    {
    private:
        static void start_thread(History *history, const char *server, const char *id, const char *topic);
        std::vector<std::thread> threads{};

    public:
        MQTT();
        void join() override;
    };

    MQTT::MQTT()
    {
        auto server = "tcp://Mosquitto:1883";
        threads.emplace_back(start_thread, &verkehrssituation, server, "zentrale_verkehrssituation", "verkehrssituation");
        threads.emplace_back(start_thread, &fuellstand, server, "zentrale_fuellstand", "fuellstand");
        threads.emplace_back(start_thread, &durchschnittsgeschwindigkeit, server, "zentrale_durchschnittsgeschwindigkeit", "durchschnittsgeschwindigkeit");
        threads.emplace_back(start_thread, &kilometerstand, server, "zentrale_kilometerstand", "kilometerstand");
    }

    void MQTT::join()
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

    void MQTT::start_thread(History *history,
                            const char *server, const char *id,
                            const char *topic)
    {
        mqtt::client client(std::string{server}, std::string{id});
        mqtt::connect_options options{};
        auto connection = client.connect(options);

        client.subscribe(std::string{topic});

        while (true)
        {
            auto message = client.consume_message();

            if (message->get_topic() == "command" && message->to_string() == "exit")
            {
                break;
            }

            if (topic == "verkehrssituation")
            {
                VerkehrssituationToHeadquarter verkehrssituation{};
                verkehrssituation.ParseFromString(message->to_string());

                uint64_t received = get_time();
                auto send = verkehrssituation.send();

                verkehrssituation.set_received(received);
                verkehrssituation.set_latency(received - send);

                std::cout << verkehrssituation.DebugString() << std::endl;

                std::lock_guard<std::mutex>{history->mutex};
                history->data.push_back(verkehrssituation.SerializeAsString());
            }
            else if (topic == "fuellstand")
            {
                FuellstandToHeadquarter fuellstand{};
                fuellstand.ParseFromString(message->to_string());

                uint64_t received = get_time();
                auto send = fuellstand.send();

                fuellstand.set_received(received);
                fuellstand.set_latency(received - send);

                std::cout << fuellstand.DebugString() << std::endl;

                std::lock_guard<std::mutex>{history->mutex};
                history->data.push_back(fuellstand.SerializeAsString());
            }
            else if (topic == "durchschnittsgeschwindigkeit")
            {
                DurchschnittsgeschwindigkeitToHeadquarter durchschnittsgeschwindigkeit{};
                durchschnittsgeschwindigkeit.ParseFromString(message->to_string());

                uint64_t received = get_time();
                auto send = durchschnittsgeschwindigkeit.send();

                durchschnittsgeschwindigkeit.set_received(received);
                durchschnittsgeschwindigkeit.set_latency(received - send);

                std::cout << durchschnittsgeschwindigkeit.DebugString() << std::endl;

                std::lock_guard<std::mutex>{history->mutex};
                history->data.push_back(durchschnittsgeschwindigkeit.SerializeAsString());
            }
            else if (topic == "kilometerstand")
            {
                KilometerstandToHeadquarter kilometerstand{};
                kilometerstand.ParseFromString(message->to_string());

                uint64_t received = get_time();
                auto send = kilometerstand.send();

                kilometerstand.set_received(received);
                kilometerstand.set_latency(received - send);

                std::cout << kilometerstand.DebugString() << std::endl;

                std::lock_guard<std::mutex>{history->mutex};
                history->data.push_back(kilometerstand.SerializeAsString());
            }
        }
    }

} // namespace Headquarter