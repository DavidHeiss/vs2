#pragma once

#include <string>
#include <deque>
#include <map>

namespace TCP
{
    constexpr size_t message_size = 2048UL;

    class Request
    {
    private:
        std::string raw_message;
        std::string method;
        std::string path;
        std::string version;
        std::map<std::string, std::string> headers;
        std::string body;
        std::deque<std::string> get_lines();

        static std::deque<std::string> split(char *buffer, const char *str);
        static std::deque<std::string> split(std::string buffer, const char *str);

    public:
        Request();
        explicit Request(char *buffer);

        std::string get_raw_message();
        std::string create_raw_message();
        std::string get_method() const;
        void set_method(std::string value);
        std::string get_path() const;
        void set_path(std::string value);
        std::string get_version() const;
        void set_version(std::string value);
        std::map<std::string, std::string> get_headers() const;
        void add_headers(std::string key, std::string value);
        std::string get_body() const;
        void set_body(std::string value);
    };

    Request::Request() {}

    Request::Request(char *buffer)
    {
        raw_message = std::string{buffer};

        auto lines = split(buffer, "\r\n");

        auto start_line = split(lines.front(), " ");
        lines.pop_front();
        if (start_line.size() != 3)
        {
            throw std::runtime_error("TCPRequest : Wrong start line size! ");
        }
        method = start_line.at(0);
        path = start_line.at(1);
        version = start_line.at(2);

        while (!lines.empty())
        {
            auto line = lines.front();
            lines.pop_front();
            if (line.size() == 0)
            {
                break;
            }

            auto header = split(line, ": ");
            if (header.size() != 2)
            {
                throw std::runtime_error("TCPRequest : Error while generating header! ");
            }

            headers.emplace(header.at(0), header.at(1));
        }

        body = lines.front();

        return;
    }

    std::deque<std::string> Request::get_lines()
    {
        std::deque<std::string> lines{};
        char status_line_buffer[method.size() + path.size() + version.size() + 2]{};
        snprintf(status_line_buffer, sizeof(status_line_buffer) + 1, "%s %s %s", &method[0], &path[0], &version[0]);
        lines.push_back(std::string{status_line_buffer});
        for (auto &header : headers)
        {
            char header_buffer[header.first.size() + header.second.size() + 2]{};
            snprintf(header_buffer, sizeof(header_buffer) + 1, "%s: %s", &header.first[0], &header.second[0]);
            lines.push_back(std::string{header_buffer});
        }
        lines.push_back(std::string{""});
        if (!body.empty())
        {
            lines.push_back(body);
        }
        return lines;
    }

    std::string Request::get_raw_message() 
    {
        if (raw_message.empty()) {
            return create_raw_message();
        }
        return raw_message;
    }

    std::string Request::create_raw_message()
    {
        auto lines = get_lines();

        // std::string start{""};
        // for (auto &line : lines)
        // {
        //     char new_line[start.size() + line.size() + 2]{};
        //     sprintf(new_line, "%s%s\r\n", &start[0], &line[0]);
        //     start = std::string{new_line};
        // }
        // return start;

        std::string start{""};
        for(auto & line : lines) {
            start += line + "\r\n";
        }

        return start;
    }

    std::string Request::get_method() const
    {
        return method;
    }

    void Request::set_method(std::string value)
    {
        method = value;
    }

    std::string Request::get_path() const
    {
        return path;
    }

    void Request::set_path(std::string value)
    {
        path = value;
    }

    std::string Request::get_version() const
    {
        return version;
    }

    void Request::set_version(std::string value)
    {
        version = value;
    }

    std::map<std::string, std::string> Request::get_headers() const
    {
        return headers;
    }

    void Request::add_headers(std::string key, std::string value)
    {
        headers.emplace(key, value);
    }

    std::string Request::get_body() const
    {
        return body;
    }

    void Request::set_body(std::string value)
    {
        body = value;
    }

    std::deque<std::string> Request::split(char *buffer, const char *str)
    {
        return split(std::string(buffer), str);
    }

    std::deque<std::string> Request::split(std::string buffer, const char *str)
    {
        std::deque<std::string> segments{};
        auto str_view = std::string(str);

        size_t end_pos{};
        while (true)
        {
            end_pos = buffer.find(str_view);
            auto sub_str = buffer.substr(0, end_pos);
            segments.push_back(sub_str);
            if (buffer.size() == 0 || end_pos == std::string ::npos)
            {
                break;
            }
            buffer = buffer.substr(end_pos + str_view.size(), buffer.size());
        }

        return segments;
    }
} // namespace TCP