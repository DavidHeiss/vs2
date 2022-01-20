#pragma once

#include <string>
#include <random>
#include <random>
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../Libary/message.pb.h"

using json = nlohmann::json;
namespace chrono = std::chrono;

namespace Sensor
{
    enum Types
    {
        frei = 1,
        maessiger_verkehr,
        starker_verkehr,
        stau
    };

    class Verkehrssitation
    {
    private:
        size_t id;
        std::mt19937 rng;
        double value;
        bool rising;
        Types situation;
        void changeRising();
        double getTimeDiff();

    public:
        Verkehrssitation();
        ::VerkehrssituationToHeadquarter generateMessage();
    };

    Verkehrssitation::Verkehrssitation()
    {
        rng = std::mt19937{};
        situation = Types::frei;
        value = situation * 1.0;
        rising = true;
        id = 0;
    }

    ::VerkehrssituationToHeadquarter Verkehrssitation::generateMessage()
    {
        ::Verkehrssituation verkehrssituation{};

        if (value < Types::maessiger_verkehr)
        {
            if (value <= 0)
            {
                rising = true;
                value = 0;
            }
            else if (situation != Types::frei)
            {
                changeRising();
            }
            situation = Types::frei;
        }
        else if (value < Types::starker_verkehr)
        {
            if (situation != Types::maessiger_verkehr)
            {
                changeRising();
            }
            situation = Types::maessiger_verkehr;
        }
        else if (value < Types::stau)
        {
            if (situation != Types::starker_verkehr)
            {
                changeRising();
            }
            situation = Types::starker_verkehr;
        }
        else
        {
            if (value >= 5)
            {
                rising = false;
                value = 5;
            }
            else if (situation != Types::stau)
            {
                changeRising();
            }
            situation = Types::stau;
        }

        switch (situation)
        {
        case Types::frei:
            verkehrssituation.set_value("Frei");
            break;
        case Types::maessiger_verkehr:
            verkehrssituation.set_value("maessiger Verkehr");
            break;
        case Types::starker_verkehr:
            verkehrssituation.set_value("starker Verkehr");
            break;
        case Types::stau:
            verkehrssituation.set_value("Stau");
            break;
        default:
            throw std::runtime_error("Error unkown value for Verkerhssitatuion!");
        }

        verkehrssituation.set_rising(rising);
        verkehrssituation.set_numeric_value(value);

        std::uniform_real_distribution range{
            rising ? -0.8 : -1.0,
            rising ? 1.0 : 0.8};

        value += range(rng);

        verkehrssituation.set_type("Verkehrssituation");

        ::VerkehrssituationToHeadquarter message{};

        message.mutable_data()->CopyFrom(verkehrssituation);

        chrono::nanoseconds time{chrono::system_clock::now().time_since_epoch()};
        message.set_send(time.count());

        return message;
    }

    void Verkehrssitation::changeRising()
    {
        std::uniform_int_distribution range{0, 1};
        rising = range(rng);
    }
} // namespace Sensor