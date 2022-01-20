#pragma once

#include <string>

namespace REST
{
    struct Segment
    {
        size_t start_pos{};
        size_t end_pos{};
        size_t length{};
        size_t segment_pos{};
        std::string string{};
    };
} // namespace REST
