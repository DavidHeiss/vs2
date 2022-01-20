#pragma once

#include <string>

namespace REST
{
    class PathValue
    {
    private:
        std::string value{};

    public:
        PathValue(std::string value);
        template <class T>
        T get_value();
    };

    PathValue::PathValue(std::string v)
    {
        value = v;
    }

    template <class T>
    T PathValue::get_value()
    {
        T value;
        std::stringstream ss{PathValue::value};
        ss >> value;
        return value;
    }
} // namespace REST
