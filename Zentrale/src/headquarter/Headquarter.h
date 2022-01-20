#pragma once

#include <vector>
#include <mutex>
#include <thread>
#include <netdb.h>
#include <fcntl.h>

#include "../../Libary/History.h"
#include "../../Libary/TCP/TCPServer.h"
#include "../../Libary/REST/Path.h"
#include "../../Libary/serialize.h"

namespace chrono = std::chrono;

namespace Headquarter
{
    class Headquarter
    {
    private:
        std::vector<TCP::Client> clients{};
        std::vector<std::thread> threads{};
        static void start_tcp_server(Headquarter *headquarter);
        static void handle_client(Headquarter *headquarter, TCP::Client client);
        static void start_sync_with_server(u_int16_t port, History *history, std::function<std::string(std::string &, sockaddr_in &)> serializer);
        static std::string sync_verkehrssituation(std::string &value, sockaddr_in &addr);
        static std::string sync_durchschnittsgeschwindigkeit(std::string &value, sockaddr_in &addr);
        static std::string sync_kilometerstand(std::string &value, sockaddr_in &addr);
        static std::string sync_fuellstand(std::string &value, sockaddr_in &addr);

    protected:
        History verkehrssituation{};
        History durchschnittsgeschwindigkeit{};
        History kilometerstand{};
        History fuellstand{};
        static int64_t get_time();
        virtual void join();

    public:
        Headquarter();
    };

    Headquarter::Headquarter()
    {
        threads.emplace_back(start_tcp_server, this);
        threads.emplace_back(start_sync_with_server, 9000, &verkehrssituation, sync_verkehrssituation);
        threads.emplace_back(start_sync_with_server, 9001, &durchschnittsgeschwindigkeit, sync_durchschnittsgeschwindigkeit);
        threads.emplace_back(start_sync_with_server, 9002, &kilometerstand, sync_kilometerstand);
        threads.emplace_back(start_sync_with_server, 9003, &fuellstand, sync_fuellstand);
    }

    int64_t Headquarter::get_time()
    {
        return chrono::system_clock::now().time_since_epoch().count();
    }

    void Headquarter::join()
    {
        for (auto &thread : threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    void Headquarter::handle_client(Headquarter *headquarter, TCP::Client client)
    {
        auto request = client.receive();

        auto response = TCP::Response{};
        response.set_version(request.get_version());
        response.set_status_code("200");
        response.set_status_message("OK");
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Server", "Headquarter");

        //Paths
        REST::Path verkehrssituation_single{"/verkehrssituation/{index=[0-9]+}"};
        REST::Path durchschnittsgeschwindigkeit_single{"/durchschnittsgeschwindigkeit/{index=[0-9]+}"};
        REST::Path kilometerstand_single{"/kilometerstand/{index=[0-9]+}"};
        REST::Path fuellstand_single{"/fuellstand/{index=[0-9]+}"};
        if (request.get_method() != "GET")
        {
            response.set_status_code("404");
            response.set_status_message("Not Found");
        }
        else if (request.get_path() == "/")
        {
            nlohmann::json json{};
            {
                //Verkehrssituation
                {
                    std::lock_guard<std::mutex>{headquarter->verkehrssituation.mutex};
                    if (headquarter->verkehrssituation.data.empty())
                    {
                        json["verkehrssituation"] = "empty";
                    }
                    else
                    {
                        VerkehrssituationToHeadquarter verkehrssituation{};
                        verkehrssituation.ParseFromString(headquarter->verkehrssituation.data.back());
                        json["verkehrssituation"] = JSON::Verkehrssituation(verkehrssituation.data());
                    }
                }
                //Durchschnittsgeschwindigkeit
                {
                    std::lock_guard<std::mutex>{headquarter->durchschnittsgeschwindigkeit.mutex};
                    if (headquarter->durchschnittsgeschwindigkeit.data.empty())
                    {
                        json["durchschnittsgeschwindigkeit"] = "emtpy";
                    }
                    else
                    {
                        DurchschnittsgeschwindigkeitToHeadquarter durchschnittsgeschwindigkeit{};
                        durchschnittsgeschwindigkeit.ParseFromString(headquarter->durchschnittsgeschwindigkeit.data.back());
                        json["durchschnittsgeschwindigkeit"] = JSON::Durchschnittsgeschwindigkeit(durchschnittsgeschwindigkeit.data());
                    }
                }
                //Kilometerstand
                {
                    std::lock_guard<std::mutex>{headquarter->kilometerstand.mutex};
                    if (headquarter->kilometerstand.data.empty())
                    {
                        json["kilometerstand"] = "emtpy";
                    }
                    else
                    {
                        KilometerstandToHeadquarter kilometerstand{};
                        kilometerstand.ParseFromString(headquarter->kilometerstand.data.back());
                        json["kilometerstand"] = JSON::Kilometerstand(kilometerstand.data());
                    }
                }
                //Fuellstand
                {
                    std::lock_guard<std::mutex>{headquarter->fuellstand.mutex};
                    if (headquarter->fuellstand.data.empty())
                    {
                        json["fuellstand"] = "emtpy";
                    }
                    else
                    {
                        FuellstandToHeadquarter fuellstand{};
                        fuellstand.ParseFromString(headquarter->fuellstand.data.back());
                        json["fuellstand"] = JSON::Fuellstand(fuellstand.data());
                    }
                }
            }
            json["time"] = get_time();

            auto body = json.dump();

            response.add_header("Content-Type", "application/json");
            response.add_header("Content-Length", std::to_string(body.size()));
            response.set_body(body);
        }
        //Verkehrssituation
        else if (request.get_path() == "/verkehrssituation")
        {
            std::vector<nlohmann::json> history{};
            {
                std::lock_guard<std::mutex>{headquarter->verkehrssituation.mutex};

                auto iterator = headquarter->verkehrssituation.data.size() > 1000 ? headquarter->verkehrssituation.data.end() - 1000 : headquarter->verkehrssituation.data.begin();
                while (iterator != headquarter->verkehrssituation.data.end())
                {
                    VerkehrssituationToHeadquarter verkehrssituation{};
                    verkehrssituation.ParseFromString(*iterator);
                    history.push_back(JSON::Verkehrssituation(verkehrssituation.data()));
                    iterator++;
                }
            }

            nlohmann::json json{};
            json["data"] = history;
            json["time"] = get_time();

            auto body = json.dump();

            response.add_header("Content-Type", "application/json");
            response.add_header("Content-Length", std::to_string(body.size()));
            response.set_body(body);
        }
        else if (verkehrssituation_single.match(request.get_path()))
        {
            auto index = verkehrssituation_single.get_value<int>(request.get_path(), "index");
            std::lock_guard<std::mutex>{headquarter->verkehrssituation.mutex};
            if (index < headquarter->verkehrssituation.data.size())
            {
                nlohmann::json json{};
                VerkehrssituationToHeadquarter verkehrssituation{};
                verkehrssituation.ParseFromString(headquarter->verkehrssituation.data.at(index));
                json["data"] = JSON::Verkehrssituation(verkehrssituation.data());
                json["time"] = get_time();

                auto body = json.dump();

                response.add_header("Content-Type", "application/json");
                response.add_header("Content-Length", std::to_string(body.size()));
                response.set_body(body);
            }
            else
            {
                response.set_status_code("404");
                response.set_status_message("Not Found");
            }
        }
        //Durchschnittsgeschwindigkeit
        else if (request.get_path() == "/durchschnittsgeschwindigkeit")
        {
            std::vector<nlohmann::json> history{};
            {
                std::lock_guard<std::mutex>{headquarter->durchschnittsgeschwindigkeit.mutex};

                auto iterator = headquarter->durchschnittsgeschwindigkeit.data.size() > 1000 ? headquarter->durchschnittsgeschwindigkeit.data.end() - 1000 : headquarter->durchschnittsgeschwindigkeit.data.begin();
                while (iterator != headquarter->durchschnittsgeschwindigkeit.data.end())
                {
                    DurchschnittsgeschwindigkeitToHeadquarter durchschnittsgeschwindigkeit{};
                    durchschnittsgeschwindigkeit.ParseFromString(*iterator);
                    history.push_back(JSON::Durchschnittsgeschwindigkeit(durchschnittsgeschwindigkeit.data()));
                    iterator++;
                }
            }

            nlohmann::json json{};
            json["data"] = history;
            json["time"] = get_time();

            auto body = json.dump();

            response.add_header("Content-Type", "application/json");
            response.add_header("Content-Length", std::to_string(body.size()));
            response.set_body(body);
        }
        else if (durchschnittsgeschwindigkeit_single.match(request.get_path()))
        {
            auto index = durchschnittsgeschwindigkeit_single.get_value<int>(request.get_path(), "index");
            std::lock_guard<std::mutex>{headquarter->durchschnittsgeschwindigkeit.mutex};
            if (index < headquarter->durchschnittsgeschwindigkeit.data.size())
            {
                nlohmann::json json{};
                DurchschnittsgeschwindigkeitToHeadquarter durchschnittsgeschwindigkeit{};
                durchschnittsgeschwindigkeit.ParseFromString(headquarter->durchschnittsgeschwindigkeit.data.at(index));
                json["data"] = JSON::Durchschnittsgeschwindigkeit(durchschnittsgeschwindigkeit.data());
                json["time"] = get_time();

                auto body = json.dump();

                response.add_header("Content-Type", "application/json");
                response.add_header("Content-Length", std::to_string(body.size()));
                response.set_body(body);
            }
        }
        //Kilometerstand
        else if (request.get_path() == "/kilometerstand")
        {
            std::vector<nlohmann::json> history{};
            {
                std::lock_guard<std::mutex>{headquarter->kilometerstand.mutex};

                auto iterator = headquarter->kilometerstand.data.size() > 1000 ? headquarter->kilometerstand.data.end() - 1000 : headquarter->kilometerstand.data.begin();
                while (iterator != headquarter->kilometerstand.data.end())
                {
                    KilometerstandToHeadquarter kilometerstand{};
                    kilometerstand.ParseFromString(*iterator);
                    history.push_back(JSON::Kilometerstand(kilometerstand.data()));
                    iterator++;
                }
            }

            nlohmann::json json{};
            json["data"] = history;
            json["time"] = get_time();

            auto body = json.dump();

            response.add_header("Content-Type", "application/json");
            response.add_header("Content-Length", std::to_string(body.size()));
            response.set_body(body);
        }
        else if (kilometerstand_single.match(request.get_path()))
        {
            auto index = kilometerstand_single.get_value<int>(request.get_path(), "index");
            std::lock_guard<std::mutex>{headquarter->kilometerstand.mutex};
            if (index < headquarter->kilometerstand.data.size())
            {
                nlohmann::json json{};
                KilometerstandToHeadquarter kilometerstand{};
                kilometerstand.ParseFromString(headquarter->kilometerstand.data.at(index));
                json["data"] = JSON::Kilometerstand(kilometerstand.data());
                json["time"] = get_time();

                auto body = json.dump();

                response.add_header("Content-Type", "application/json");
                response.add_header("Content-Length", std::to_string(body.size()));
                response.set_body(body);
            }
        }
        //Fuellstand
        else if (request.get_path() == "/fuellstand")
        {
            std::vector<nlohmann::json> history{};
            {
                std::lock_guard<std::mutex>{headquarter->fuellstand.mutex};

                auto iterator = headquarter->fuellstand.data.size() > 1000 ? headquarter->fuellstand.data.end() - 1000 : headquarter->fuellstand.data.begin();
                while (iterator != headquarter->fuellstand.data.end())
                {
                    FuellstandToHeadquarter fuellstand{};
                    fuellstand.ParseFromString(*iterator);
                    history.push_back(JSON::Fuellstand(fuellstand.data()));
                    iterator++;
                }
            }

            nlohmann::json json{};
            json["data"] = history;
            json["time"] = get_time();

            auto body = json.dump();

            response.add_header("Content-Type", "application/json");
            response.add_header("Content-Length", std::to_string(body.size()));
            response.set_body(body);
        }
        else if (fuellstand_single.match(request.get_path()))
        {
            auto index = fuellstand_single.get_value<int>(request.get_path(), "index");
            std::lock_guard<std::mutex>{headquarter->fuellstand.mutex};
            if (index < headquarter->fuellstand.data.size())
            {
                nlohmann::json json{};
                FuellstandToHeadquarter fuellstand{};
                fuellstand.ParseFromString(headquarter->fuellstand.data.at(index));
                json["data"] = JSON::Fuellstand(fuellstand.data());
                json["time"] = get_time();

                auto body = json.dump();

                response.add_header("Content-Type", "application/json");
                response.add_header("Content-Length", std::to_string(body.size()));
                response.set_body(body);
            }
        }
        else
        {
            response.set_status_code("404");
            response.set_status_message("Not Found");
        }

        client.send(response);

        client.close();
    }

    void Headquarter::start_tcp_server(Headquarter *headquarter)
    {
        try
        {
            TCP::Server server{};
            server.listen(8081);

            while (true)
            {
                auto client = server.accept();

                headquarter->threads.emplace_back(handle_client, headquarter, client);
            }
            server.close();
        }
        catch (std::exception &e)
        {
            perror(e.what());
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    std::string Headquarter::sync_verkehrssituation(std::string &value, sockaddr_in &addr)
    {
        VerkehrssituationToHeadquarter verkehrssituation{};
        verkehrssituation.ParseFromString(value);

        VerkehrssituationToServer message{};
        message.mutable_data()->CopyFrom(verkehrssituation);

        message.set_to_address(inet_ntoa(addr.sin_addr));
        message.set_to_port(ntohs(addr.sin_port));

        message.set_send(get_time());
        return message.SerializeAsString();
    }

    std::string Headquarter::sync_durchschnittsgeschwindigkeit(std::string &value, sockaddr_in &addr)
    {
        DurchschnittsgeschwindigkeitToHeadquarter durchschnittsgeschwindigkeit{};
        durchschnittsgeschwindigkeit.ParseFromString(value);

        DurchschnittsgeschwindigkeitToServer message{};
        message.mutable_data()->CopyFrom(durchschnittsgeschwindigkeit);

        message.set_to_address(inet_ntoa(addr.sin_addr));
        message.set_to_port(ntohs(addr.sin_port));

        message.set_send(get_time());
        return message.SerializeAsString();
    }

    std::string Headquarter::sync_kilometerstand(std::string &value, sockaddr_in &addr)
    {
        KilometerstandToHeadquarter kilometerstand{};
        kilometerstand.ParseFromString(value);

        KilometerstandToServer message{};
        message.mutable_data()->CopyFrom(kilometerstand);

        message.set_to_address(inet_ntoa(addr.sin_addr));
        message.set_to_port(ntohs(addr.sin_port));

        message.set_send(get_time());
        return message.SerializeAsString();
    }

    std::string Headquarter::sync_fuellstand(std::string &value, sockaddr_in &addr)
    {
        FuellstandToHeadquarter fuellstand{};
        fuellstand.ParseFromString(value);

        FuellstandToServer message{};
        message.mutable_data()->CopyFrom(fuellstand);

        message.set_to_address(inet_ntoa(addr.sin_addr));
        message.set_to_port(ntohs(addr.sin_port));

        message.set_send(get_time());
        return message.SerializeAsString();
    }

    void Headquarter::start_sync_with_server(u_int16_t port, History *history, std::function<std::string(std::string &, sockaddr_in &)> serializer)
    {
        for (auto i = 0; true; i++)
        {
            try
            {
                while (true)
                {
                    if (i % 2 == 0)
                    {
                        auto host = gethostbyname("Server");
                        if (host == nullptr)
                        {
                            throw std::runtime_error(hstrerror(h_errno));
                        }

                        sockaddr_in addr{};
                        addr.sin_family = host->h_addrtype;
                        memcpy(&addr.sin_addr.s_addr, host->h_addr, sizeof(host->h_addr));
                        addr.sin_port = htons(port);

                        if (history->synced < history->data.size())
                        {
                            TCP::Client client{addr};
                            std::lock_guard<std::mutex>{history->mutex};
                            while (history->synced < history->data.size())
                            {
                                client.send_raw(serializer(history->data.at(history->synced), addr));
                                ++history->synced;
                            }
                            client.close();
                        }
                    }
                    else
                    {
                        auto host = gethostbyname("Backup");
                        if (host == nullptr)
                        {
                            throw std::runtime_error(hstrerror(h_errno));
                        }

                        sockaddr_in addr{};
                        addr.sin_family = host->h_addrtype;
                        memcpy(&addr.sin_addr.s_addr, host->h_addr, sizeof(host->h_addr));
                        addr.sin_port = htons(port);

                        if (history->synced < history->data.size())
                        {
                            std::lock_guard<std::mutex>{history->mutex};
                            while (history->synced < history->data.size())
                            {
                                TCP::Client client{addr};
                                client.send_raw(serializer(history->data.at(history->synced), addr));
                                ++history->synced;
                                client.close();
                            }
                        }
                    }
                    usleep(1);
                }
            }
            catch (std::exception &e)
            {
                perror(e.what());
            }

            sleep(1);
        }
    }
} // namespace Headquarter