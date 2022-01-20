#pragma once

#include <nlohmann/json.hpp>
#include "message.pb.h"

namespace JSON
{
    nlohmann::json Durchschnittsgeschwindigkeit(const ::Durchschnittsgeschwindigkeit &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_accelerate())
        {
            json["accelerate"] = buffer.accelerate();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'accelerate'! ");
        }

        if (buffer.has_acceleration())
        {
            json["acceleration"] = buffer.acceleration();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'acceleration'! ");
        }

        if (buffer.has_average_speed())
        {
            json["average speed"] = buffer.average_speed();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'average speed'! ");
        }

        if (buffer.has_breaking_force())
        {
            json["breaking force"] = buffer.breaking_force();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'breaking force'! ");
        }

        if (buffer.has_distance())
        {
            json["distance"] = buffer.distance();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'distance'! ");
        }

        if (buffer.has_min_speed())
        {
            json["min speed"] = buffer.min_speed();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : duchschnittsgeschwindigkeit do not have 'min speed'! ");
        }

        if (buffer.has_max_speed())
        {
            json["max speed"] = buffer.max_speed();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'max speed'! ");
        }

        if (buffer.has_speed())
        {
            json["speed"] = buffer.speed();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'speed'! ");
        }

        if (buffer.has_start_time())
        {
            json["start time"] = buffer.start_time();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'start time'! ");
        }

        if (buffer.has_type())
        {
            json["type"] = buffer.type();
        }
        else
        {
            throw std::runtime_error("Durchschnittsgeschwindigkeit : buffer do not have 'type'! ");
        }

        return json;
    }

    nlohmann::json DurchschnittsgeschwindigkeitToHeadquarter(const ::DurchschnittsgeschwindigkeitToHeadquarter &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = Durchschnittsgeschwindigkeit(buffer.data());
        }
        else
        {
            throw std::runtime_error("DurchschnittsgeschwindigkeitToHeadquarter : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("DurchschnittsgeschwindigkeitToHeadquarter : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json DurchschnittsgeschwindigkeitToServer(const ::DurchschnittsgeschwindigkeitToServer &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = DurchschnittsgeschwindigkeitToHeadquarter(buffer.data());
        }
        else
        {
            throw std::runtime_error("DurchschnittsgeschwindigkeitToServer : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("DurchschnittsgeschwindigkeitToServer : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json DurchschnittsgeschwindigkeitFromServer(const ::DurchschnittsgeschwindigkeitFromServer &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = DurchschnittsgeschwindigkeitToServer(buffer.data());
        }
        else
        {
            throw std::runtime_error("DurchschnittsgeschwindigkeitFromServer : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("DurchschnittsgeschwindigkeitFromServer : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json Fuellstand(::Fuellstand buffer)
    {
        nlohmann::json json{};

        if (buffer.has_capacity())
        {
            json["capacity"] = buffer.capacity();
        }
        else
        {
            throw std::runtime_error("Fuellstand : buffer do not have 'capacity'! ");
        }

        if (buffer.has_consumption())
        {
            json["consumption"] = buffer.consumption();
        }
        else
        {
            throw std::runtime_error("Fuellstand : buffer do not have 'consumption'! ");
        }

        if (buffer.has_current_capacity())
        {
            json["current capacity"] = buffer.current_capacity();
        }
        else
        {
            throw std::runtime_error("Fuellstand : buffer do not have 'consumption'! ");
        }

        if (buffer.has_last_refueld())
        {
            json["last refueld"] = buffer.last_refueld();
        }
        else
        {
            throw std::runtime_error("Fuellstand : buffer do not have 'last refueld'! ");
        }

        if (buffer.has_level())
        {
            json["level"] = buffer.level();
        }
        else
        {
            throw std::runtime_error("Fuellstand : buffer do not have 'last refueld'! ");
        }

        if (buffer.has_ussage())
        {
            json["ussage"] = buffer.ussage();
        }
        else
        {
            throw std::runtime_error("Fuellstand : buffer do not have 'last refueld'! ");
        }

        if (buffer.has_will_refill())
        {
            json["will refill"] = buffer.will_refill();
        }
        else
        {
            throw std::runtime_error("Fuellstand : buffer do not have 'will refill'! ");
        }

        if (buffer.has_type())
        {
            json["type"] = buffer.type();
        }
        else
        {
            throw std::runtime_error("Fuellstand : buffer do not have 'type'! ");
        }

        return json;
    }

    nlohmann::json FuellstandToHeadquarter(const ::FuellstandToHeadquarter &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = Fuellstand(buffer.data());
        }
        else
        {
            throw std::runtime_error("FuellstandToHeadquarter : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("FuellstandToHeadquarter : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json FuellstandToServer(const ::FuellstandToServer &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = FuellstandToHeadquarter(buffer.data());
        }
        else
        {
            throw std::runtime_error("FuellstandToServer : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("FuellstandToServer : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json FuellstandFromServer(const ::FuellstandFromServer &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = FuellstandToServer(buffer.data());
        }
        else
        {
            throw std::runtime_error("FuellstandFromServer : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("FuellstandFromServer : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json Kilometerstand(const ::Kilometerstand buffer)
    {
        nlohmann::json json{};

        if (buffer.has_mileage())
        {
            json["mileage"] = buffer.mileage();
        }
        else
        {
            throw std::runtime_error("Kilometerstand : buffer do not have 'mileage'! ");
        }

        if (buffer.has_type())
        {
            json["type"] = buffer.type();
        }
        else
        {
            throw std::runtime_error("Kilometerstand : buffer do not have 'type'! ");
        }

        return json;
    }

    nlohmann::json KilometerstandToHeadquarter(const ::KilometerstandToHeadquarter &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = Kilometerstand(buffer.data());
        }
        else
        {
            throw std::runtime_error("KilometerstandToHeadquarter : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("KilometerstandToHeadquarter : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json KilometerstandToServer(const ::KilometerstandToServer &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = KilometerstandToHeadquarter(buffer.data());
        }
        else
        {
            throw std::runtime_error("KilometerstandToServer : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("KilometerstandToServer : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json KilometerstandFromServer(const ::KilometerstandFromServer &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = KilometerstandToServer(buffer.data());
        }
        else
        {
            throw std::runtime_error("KilometerstandFromServer : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("KilometerstandFromServer : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json Verkehrssituation(const ::Verkehrssituation &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_numeric_value())
        {
            json["numeric value"] = buffer.numeric_value();
        }
        else
        {
            throw std::runtime_error("Verkehrssituation : buffer do not have 'numberic value'! ");
        }

        if (buffer.has_rising())
        {
            json["rising"] = buffer.rising();
        }
        else
        {
            throw std::runtime_error("Verkehrssituation : buffer do not have 'rising'! ");
        }

        if (buffer.has_value())
        {
            json["value"] = buffer.value();
        }
        else
        {
            throw std::runtime_error("Verkehrssituation : buffer do not have 'value'! ");
        }

        if (buffer.has_type())
        {
            json["type"] = buffer.type();
        }
        else
        {
            throw std::runtime_error("Verkehrssituation : buffer do not have 'type'! ");
        }

        return json;
    }

    nlohmann::json VerkehrssituationToHeadquarter(const ::VerkehrssituationToHeadquarter &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = Verkehrssituation(buffer.data());
        }
        else
        {
            throw std::runtime_error("VerkehrssituationToHeadquarter : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("VerkehrssituationToHeadquarter : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json VerkehrssituationToServer(const ::VerkehrssituationToServer &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = VerkehrssituationToHeadquarter(buffer.data());
        }
        else
        {
            throw std::runtime_error("VerkehrssituationToServer : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("VerkehrssituationToServer : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }

    nlohmann::json VerkehrssituationFromServer(const ::VerkehrssituationFromServer &buffer)
    {
        nlohmann::json json{};

        if (buffer.has_data())
        {
            json["data"] = VerkehrssituationToServer(buffer.data());
        }
        else
        {
            throw std::runtime_error("VerkehrssituationFromServer : buffer do not have 'data'! ");
        }

        if (buffer.has_from_address())
        {
            json["from address"] = buffer.from_address();
        }

        if (buffer.has_from_port())
        {
            json["from port"] = buffer.from_port();
        }

        if (buffer.has_to_address())
        {
            json["to address"] = buffer.to_address();
        }

        if (buffer.has_to_port())
        {
            json["to port"] = buffer.to_port();
        }

        if (buffer.has_send())
        {
            json["send"] = buffer.send();
        }
        else
        {
            throw std::runtime_error("VerkehrssituationFromServer : buffer do not have 'send'! ");
        }

        if (buffer.has_received())
        {
            json["received"] = buffer.received();
        }

        if (buffer.has_latency())
        {
            json["latency"] = buffer.latency();
        }

        if (buffer.has_round_trip())
        {
            json["round trip"] = buffer.round_trip();
        }

        return json;
    }
} // namespace JSON