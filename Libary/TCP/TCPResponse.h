#pragma once

#include <string>
#include <map>
#include <deque>
#include <vector>

namespace TCP
{
    class Response
    {
    private:
        std::string version{};
        std::string status_code{};
        std::string status_message{};
        std::map<std::string, std::string> headers{};
        std::string body{};
        static std::deque<std::string> split(std::string buffer, const char *str);

    public:
        Response();
        explicit Response(std::string message);

        std::deque<std::string> get_lines();

        std::string get_raw_message();

        void set_version(std::string version);
        std::string get_version() const;

        void set_status_code(std::string status_code);
        std::string get_status_code() const;

        void set_status_message(std::string status_message);
        std::string get_status_message() const;

        void add_header(std::string key, std::string value);
        std::map<std::string, std::string> get_headers() const;

        void set_body(std::string body);
        std::string get_body() const;
    };

    std::deque<std::string> Response::split(std::string buffer, const char *str)
    {
        std::deque<std::string> segments{};
        auto str_view = std::string(str);

        size_t end_pos{};
        while (buffer.size() != 0 && end_pos != std::string ::npos)
        {
            end_pos = buffer.find(str_view);
            auto sub_str = buffer.substr(0, end_pos);
            segments.push_back(sub_str);

            buffer = buffer.substr(end_pos + str_view.size(), buffer.size());
        }

        return segments;
    }

    Response::Response() {}

    Response::Response(std::string message)
    {
        auto lines = split(message, "\r\n");

        auto status_line = split(lines.front(), " ");
        lines.pop_front();
        if (status_line.size() < 3)
        {
            std::cout << message << std::endl;
            throw std::runtime_error("TCPResponse : Wrong status line size! ");
        }
        version = status_line.at(0);
        status_code = status_line.at(1);
        if (status_line.size() > 3)
        {
            std::string status_message_buffer{};
            for (size_t i = 2; i < status_line.size(); i++)
            {
                auto status_message_part = status_line.at(i);

                char buffer[status_message_buffer.size() + status_message_part.size() + 1]{};
                if (i + 1 == status_line.size())
                {
                    snprintf(buffer, sizeof(buffer) + 1, "%s%s", &status_message_buffer[0], &status_message_part[0]);
                }
                else
                {
                    snprintf(buffer, sizeof(buffer) + 1, "%s%s ", &status_message_buffer[0], &status_message_part[0]);
                }
                status_message_buffer = std::string{buffer};
            }

            status_message = status_message_buffer;
        }
        else
        {
            status_message = status_line.at(2);
        }

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
                throw std::runtime_error("TCPResponse : Error headers has wrong format! ");
            }

            headers.emplace(header.at(0), header.at(1));
        }

        body = lines.back();
        return;
    }

    std::deque<std::string> Response::get_lines()
    {
        std::deque<std::string> lines{};
        char status_line_buffer[version.size() + status_code.size() + status_message.size() + 2]{};
        snprintf(status_line_buffer, sizeof(status_line_buffer) + 1, "%s %s %s", &version[0], &status_code[0], &status_message[0]);
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

    std::string Response::get_raw_message()
    {
        auto lines = get_lines();

        std::string start{""};
        for (auto &line : lines)
        {
            char new_line[start.size() + line.size() + 2]{};
            sprintf(new_line, "%s%s\r\n", &start[0], &line[0]);
            start = std::string{new_line};
        }

        return start;
    }

    void Response::set_version(std::string value)
    {
        version = value;
    }

    std::string Response::get_version() const
    {
        return version;
    }

    void Response::set_status_code(std::string value)
    {
        status_code = value;
    }

    std::string Response::get_status_code() const
    {
        return status_code;
    }

    void Response::set_status_message(std::string value)
    {
        status_message = value;
    }

    std::string Response::get_status_message() const
    {
        return status_message;
    }

    void Response::add_header(std::string key, std::string value)
    {
        headers.emplace(key, value);
    }

    std::map<std::string, std::string> Response::get_headers() const
    {
        return headers;
    }

    void Response::set_body(std::string value)
    {
        body = value;
    }

    std::string Response::get_body() const
    {
        return body;
    }

} // namespace TCP
