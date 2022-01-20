#pragma once

#include <mutex>
#include <deque>
#include <string>

namespace Headquarter
{
    struct History
    {
        std::mutex mutex{};
        std::deque<std::string> data{};
        int synced{};
    };

} // namespace Headquarter
