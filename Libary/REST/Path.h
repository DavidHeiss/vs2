#pragma once

#include <string>
#include <regex>
#include <map>
#include <vector>

#include "Segment.h"
#include "PathValue.h"

namespace REST
{
    class Path
    {
    private:
        std::string regex{};
        std::map<std::string, Segment> segments{};

        std::vector<Segment> split_segments(std::string path, std::string regex);
        std::map<std::string, PathValue> get_values(std::string path);

    public:
        explicit Path(std::string path);
        bool match(std::string path);
        template <class T>
        T get_value(std::string path, std::string key);
    };

    Path::Path(std::string path)
    {
        auto segment_split = split_segments(path, R"([^\/]+)");
        auto segment_regex = split_segments(path, R"(\{[^\/]+\=[^\/]+\})");

        std::map<std::string, Segment> segs{};

        for (auto &split : segment_split)
        {
            for (auto &regex : segment_regex)
            {
                if (split.start_pos == regex.start_pos && split.end_pos == regex.end_pos && split.length == regex.length)
                {
                    auto pos = split.string.find('=');
                    auto key = split.string.substr(1, pos - 1);
                    auto value = split.string.substr(pos + 1, split.string.size() - pos - 2);

                    split.string = value;
                    segs.emplace(key, split);
                }
            }
        }

        std::string path_regex_buffer{""};
        for (auto &split : segment_split)
        {
            char buffer[path_regex_buffer.size() + split.string.size() + 2]{};
            snprintf(buffer, sizeof(buffer) + 1, R"(%s\/%s)", &path_regex_buffer[0], &split.string[0]);
            path_regex_buffer = std::string{buffer};
        }

        regex = path_regex_buffer;
        segments = segs;
    }

    std::vector<Segment> Path::split_segments(std::string path, std::string regex)
    {
        std::vector<Segment> segments{};

        std::string search_string{path};
        std::regex split_regex{regex};
        std::smatch match;

        size_t pos{0};
        size_t segment_pos{0};

        while (std::regex_search(search_string, match, split_regex))
        {
            pos += match.position();

            Segment segment{};
            segment.start_pos = pos;
            segment.end_pos = pos + match.length();
            segment.length = match.length();
            segment.segment_pos = segment_pos;
            segment.string = match.str();

            segments.push_back(segment);

            pos += match.length();
            segment_pos++;
            search_string = match.suffix();
        }

        return segments;
    }

    bool Path::match(std::string path)
    {
        std::smatch match;
        std::regex path_regex{regex};
        if (std::regex_match(path, match, path_regex))
        {
            return true;
        }
        return false;
    }

    std::map<std::string, PathValue> Path::get_values(std::string path)
    {
        std::map<std::string, PathValue> values{};
        auto segs = split_segments(path, R"([^\/]+)");
        for (auto &segment : segments)
        {
            values.emplace(segment.first, segs.at(segment.second.segment_pos).string);
        }
        return values;
    }

    template <class T>
    T Path::get_value(std::string path, std::string key)
    {
        auto values = get_values(path);
        return values.at(key).get_value<T>();
    }
} // namespace REST