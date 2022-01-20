#pragma once

#include <vector>
#include <thread>
#include <unistd.h>
#include <chrono>
#include <mutex>
#include <netdb.h>
#include <random>

#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>

#include "TCP/TCPServer.h"
#include "History.h"
#include "serialize.h"
#include "message.pb.h"

using namespace Headquarter;

class Server
{
private:
    mongocxx::instance instance;
    mongocxx::pool pool;

    std::vector<std::thread> threads{};

    History verkehrssituation{};
    History durchschnittsgeschwindigkeit{};
    History kilometerstand{};
    History fuellstand{};

    static void start_server(Server *server, const u_int16_t &port, const char *database);
    static void handle_client(Server *server, TCP::Client client, const char *database);
    static void start_sync_to(Server *server, const u_int16_t &port, const char *database, const char *server_name);
    static void start_sync_with(Server *server, const u_int16_t &port, const char *database);

    static uint64_t get_time();

public:
    explicit Server(std::string uri);
    void sync(const char *server);
    void join();
    void kill_me_at_some_point();
};

Server::Server(std::string uri) : instance{}, pool{mongocxx::uri{uri}}
{
    threads.emplace_back(start_server, this, 9000, "verkehrssituation");
    threads.emplace_back(start_server, this, 9001, "durchschnittsgeschwindigkeit");
    threads.emplace_back(start_server, this, 9002, "kilometerstand");
    threads.emplace_back(start_server, this, 9003, "fuellstand");

    threads.emplace_back(start_sync_with, this, 9004, "verkehrssituation");
    threads.emplace_back(start_sync_with, this, 9005, "durchschnittsgeschwindigkeit");
    threads.emplace_back(start_sync_with, this, 9006, "kilometerstand");
    threads.emplace_back(start_sync_with, this, 9007, "fuellstand");
}

void Server::start_server(Server *server_ptr, const u_int16_t &port, const char *database)
{
    TCP::Server server{};
    server.listen(port);
    while (true)
    {
        auto client = server.accept();
        try
        {
            handle_client(server_ptr, client, database);
        }
        catch (std::runtime_error &e)
        {
            perror(e.what());
        }
        client.close();
        usleep(1);
    }
    server.close();
}

void Server::handle_client(Server *server, TCP::Client client, const char *database)
{
    auto database_client = server->pool.acquire();

    while (true)
    {
        auto message = client.receive_raw();

        if (message == "\000" || message.size() == 1)
        {
            break;
        }

        if (database == "verkehrssituation")
        {
            VerkehrssituationToServer verkehrssituation{};
            verkehrssituation.ParseFromString(message);
            verkehrssituation.set_from_address(client.get_addr());
            verkehrssituation.set_from_port(client.get_port());
            verkehrssituation.set_received(get_time());
            verkehrssituation.set_latency(verkehrssituation.received() - verkehrssituation.send());
            verkehrssituation.set_round_trip(verkehrssituation.latency() + verkehrssituation.data().latency());

            auto value = verkehrssituation.has_data();

            auto json = JSON::VerkehrssituationToServer(verkehrssituation);
            std::cout << verkehrssituation.DebugString() << std::endl;
            database_client->database("Sensor").collection(database).insert_one(bsoncxx::from_json(json.dump()));
            std::lock_guard<std::mutex>{server->verkehrssituation.mutex};
            server->verkehrssituation.data.push_back(verkehrssituation.SerializeAsString());
        }
        else if (database == "durchschnittsgeschwindigkeit")
        {
            DurchschnittsgeschwindigkeitToServer durchschnittsgeschwindigkeit{};
            durchschnittsgeschwindigkeit.ParseFromString(message);
            durchschnittsgeschwindigkeit.set_from_address(client.get_addr());
            durchschnittsgeschwindigkeit.set_from_port(client.get_port());
            durchschnittsgeschwindigkeit.set_received(get_time());
            durchschnittsgeschwindigkeit.set_latency(durchschnittsgeschwindigkeit.received() - durchschnittsgeschwindigkeit.send());
            durchschnittsgeschwindigkeit.set_round_trip(durchschnittsgeschwindigkeit.latency() + durchschnittsgeschwindigkeit.data().latency());

            auto json = JSON::DurchschnittsgeschwindigkeitToServer(durchschnittsgeschwindigkeit);
            std::cout << durchschnittsgeschwindigkeit.DebugString() << std::endl;
            database_client->database("Sensor").collection(database).insert_one(bsoncxx::from_json(json.dump()));
            std::lock_guard<std::mutex>{server->durchschnittsgeschwindigkeit.mutex};
            server->durchschnittsgeschwindigkeit.data.push_back(durchschnittsgeschwindigkeit.SerializeAsString());
        }
        else if (database == "kilometerstand")
        {
            KilometerstandToServer kilometerstand{};
            kilometerstand.ParseFromString(message);
            kilometerstand.set_from_address(client.get_addr());
            kilometerstand.set_from_port(client.get_port());
            kilometerstand.set_received(get_time());
            kilometerstand.set_latency(kilometerstand.received() - kilometerstand.send());
            kilometerstand.set_round_trip(kilometerstand.latency() + kilometerstand.data().latency());

            auto json = JSON::KilometerstandToServer(kilometerstand);
            std::cout << kilometerstand.DebugString() << std::endl;
            database_client->database("Sensor").collection(database).insert_one(bsoncxx::from_json(json.dump()));
            std::lock_guard<std::mutex>{server->kilometerstand.mutex};
            server->kilometerstand.data.push_back(kilometerstand.SerializeAsString());
        }
        else if (database == "fuellstand")
        {
            FuellstandToServer fuellstand{};
            fuellstand.ParseFromString(message);
            fuellstand.set_from_address(client.get_addr());
            fuellstand.set_from_port(client.get_port());
            fuellstand.set_received(get_time());
            fuellstand.set_latency(fuellstand.received() - fuellstand.send());
            fuellstand.set_round_trip(fuellstand.latency() + fuellstand.data().latency());

            auto json = JSON::FuellstandToServer(fuellstand);
            std::cout << fuellstand.DebugString() << std::endl;
            database_client->database("Sensor").collection(database).insert_one(bsoncxx::from_json(json.dump()));
            std::lock_guard<std::mutex>{server->fuellstand.mutex};
            server->fuellstand.data.push_back(fuellstand.SerializeAsString());
        }
        usleep(1);
    }
}

uint64_t Server::get_time()
{
    return std::chrono::system_clock::now().time_since_epoch().count();
}

void Server::join()
{
    if (threads.size() > 0)
    {
        auto thread = threads.begin();
        while (thread != threads.end())
        {
            if (thread->joinable())
            {
                thread->join();
            }
            thread = threads.erase(thread);
        }

        usleep(1);
        join();
    }
}

void Server::sync(const char *server)
{
    threads.emplace_back(start_sync_to, this, 9004, "verkehrssituation", server);
    threads.emplace_back(start_sync_to, this, 9005, "durchschnittsgeschwindigkeit", server);
    threads.emplace_back(start_sync_to, this, 9006, "kilometerstand", server);
    threads.emplace_back(start_sync_to, this, 9007, "fuellstand", server);
}

void Server::start_sync_to(Server *server, const u_int16_t &port, const char *database, const char *server_name)
{
    while (true)
    {
        try
        {
            auto host = gethostbyname(server_name);
            if (host == nullptr)
            {
                throw std::runtime_error(hstrerror(h_errno));
            }

            sockaddr_in addr{};
            addr.sin_family = host->h_addrtype;
            memcpy(&addr.sin_addr.s_addr, host->h_addr, sizeof(host->h_addr));
            addr.sin_port = htons(port);

            while (true)
            {
                if (database == "verkehrssituation")
                {
                    std::lock_guard<std::mutex>{server->verkehrssituation.mutex};
                    if (server->verkehrssituation.synced < server->verkehrssituation.data.size())
                    {
                        TCP::Client client{addr};
                        while (server->verkehrssituation.synced < server->verkehrssituation.data.size())
                        {
                            VerkehrssituationFromServer verkehrssituation{};
                            VerkehrssituationToServer data{};
                            data.ParseFromString(server->verkehrssituation.data.at(server->verkehrssituation.synced));
                            verkehrssituation.mutable_data()->CopyFrom(data);
                            verkehrssituation.set_to_address(inet_ntoa(addr.sin_addr));
                            verkehrssituation.set_to_port(ntohs(addr.sin_port));
                            verkehrssituation.set_send(get_time());
                            client.send_raw(verkehrssituation.SerializeAsString());
                            ++server->verkehrssituation.synced;
                        }
                        client.close();
                    }
                }
                else if (database == "durchschnittsgeschwindigkeit")
                {
                    std::lock_guard<std::mutex>{server->durchschnittsgeschwindigkeit.mutex};
                    if (server->durchschnittsgeschwindigkeit.synced < server->durchschnittsgeschwindigkeit.data.size())
                    {
                        TCP::Client client{addr};
                        while (server->durchschnittsgeschwindigkeit.synced < server->durchschnittsgeschwindigkeit.data.size())
                        {
                            DurchschnittsgeschwindigkeitFromServer durchschnittsgeschwindigkeit{};
                            DurchschnittsgeschwindigkeitToServer data{};
                            data.ParseFromString(server->durchschnittsgeschwindigkeit.data.at(server->durchschnittsgeschwindigkeit.synced));
                            durchschnittsgeschwindigkeit.mutable_data()->CopyFrom(data);
                            durchschnittsgeschwindigkeit.set_to_address(inet_ntoa(addr.sin_addr));
                            durchschnittsgeschwindigkeit.set_to_port(ntohs(addr.sin_port));
                            durchschnittsgeschwindigkeit.set_send(get_time());
                            client.send_raw(durchschnittsgeschwindigkeit.SerializeAsString());
                            ++server->durchschnittsgeschwindigkeit.synced;
                        }
                        client.close();
                    }
                }
                else if (database == "kilometerstand")
                {
                    std::lock_guard<std::mutex>{server->kilometerstand.mutex};
                    if (server->kilometerstand.synced < server->kilometerstand.data.size())
                    {
                        TCP::Client client{addr};
                        while (server->kilometerstand.synced < server->kilometerstand.data.size())
                        {
                            KilometerstandFromServer kilometerstand{};
                            KilometerstandToServer data{};
                            data.ParseFromString(server->kilometerstand.data.at(server->kilometerstand.synced));
                            kilometerstand.mutable_data()->CopyFrom(data);
                            kilometerstand.set_to_address(inet_ntoa(addr.sin_addr));
                            kilometerstand.set_to_port(ntohs(addr.sin_port));
                            kilometerstand.set_send(get_time());
                            client.send_raw(kilometerstand.SerializeAsString());
                            ++server->kilometerstand.synced;
                        }
                        client.close();
                    }
                }
                else if (database == "fuellstand")
                {
                    std::lock_guard<std::mutex>{server->fuellstand.mutex};
                    if (server->fuellstand.synced < server->fuellstand.data.size())
                    {
                        TCP::Client client{addr};
                        while (server->fuellstand.synced < server->fuellstand.data.size())
                        {
                            FuellstandFromServer fuellstand{};
                            FuellstandToServer data{};
                            data.ParseFromString(server->fuellstand.data.at(server->fuellstand.synced));
                            fuellstand.mutable_data()->CopyFrom(data);
                            fuellstand.set_to_address(inet_ntoa(addr.sin_addr));
                            fuellstand.set_to_port(ntohs(addr.sin_port));
                            fuellstand.set_send(get_time());
                            client.send_raw(fuellstand.SerializeAsString());
                            ++server->fuellstand.synced;
                        }
                        client.close();
                    }
                }
                usleep(1);
            }
        }
        catch (const std::exception &e)
        {
            perror(e.what());
        }
        sleep(1);
    }
}

void Server::start_sync_with(Server *server, const u_int16_t &port, const char *database)
{
    TCP::Server tcp_server{};
    tcp_server.listen(port);
    while (true)
    {
        auto client = tcp_server.accept();
        try
        {
            auto database_client = server->pool.acquire();

            while (true)
            {
                auto message = client.receive_raw();

                if (message == "\000" || message.size() == 1)
                {
                    break;
                }

                if (database == "verkehrssituation")
                {
                    VerkehrssituationFromServer verkehrssituation{};
                    verkehrssituation.ParseFromString(message);
                    verkehrssituation.set_from_address(client.get_addr());
                    verkehrssituation.set_from_port(client.get_port());
                    verkehrssituation.set_received(get_time());
                    verkehrssituation.set_latency(verkehrssituation.received() - verkehrssituation.send());
                    verkehrssituation.set_round_trip(verkehrssituation.latency() + verkehrssituation.data().latency());

                    auto value = verkehrssituation.has_data();

                    auto json = JSON::VerkehrssituationFromServer(verkehrssituation);
                    std::cout << verkehrssituation.DebugString() << std::endl;
                    database_client->database("Sensor").collection(database).insert_one(bsoncxx::from_json(json.dump()));
                }
                else if (database == "durchschnittsgeschwindigkeit")
                {
                    DurchschnittsgeschwindigkeitFromServer durchschnittsgeschwindigkeit{};
                    durchschnittsgeschwindigkeit.ParseFromString(message);
                    durchschnittsgeschwindigkeit.set_from_address(client.get_addr());
                    durchschnittsgeschwindigkeit.set_from_port(client.get_port());
                    durchschnittsgeschwindigkeit.set_received(get_time());
                    durchschnittsgeschwindigkeit.set_latency(durchschnittsgeschwindigkeit.received() - durchschnittsgeschwindigkeit.send());
                    durchschnittsgeschwindigkeit.set_round_trip(durchschnittsgeschwindigkeit.latency() + durchschnittsgeschwindigkeit.data().latency());

                    auto json = JSON::DurchschnittsgeschwindigkeitFromServer(durchschnittsgeschwindigkeit);
                    std::cout << durchschnittsgeschwindigkeit.DebugString() << std::endl;
                    database_client->database("Sensor").collection(database).insert_one(bsoncxx::from_json(json.dump()));
                }
                else if (database == "kilometerstand")
                {
                    KilometerstandFromServer kilometerstand{};
                    kilometerstand.ParseFromString(message);
                    kilometerstand.set_from_address(client.get_addr());
                    kilometerstand.set_from_port(client.get_port());
                    kilometerstand.set_received(get_time());
                    kilometerstand.set_latency(kilometerstand.received() - kilometerstand.send());
                    kilometerstand.set_round_trip(kilometerstand.latency() + kilometerstand.data().latency());

                    auto json = JSON::KilometerstandFromServer(kilometerstand);
                    std::cout << kilometerstand.DebugString() << std::endl;
                    database_client->database("Sensor").collection(database).insert_one(bsoncxx::from_json(json.dump()));
                }
                else if (database == "fuellstand")
                {
                    FuellstandFromServer fuellstand{};
                    fuellstand.ParseFromString(message);
                    fuellstand.set_from_address(client.get_addr());
                    fuellstand.set_from_port(client.get_port());
                    fuellstand.set_received(get_time());
                    fuellstand.set_latency(fuellstand.received() - fuellstand.send());
                    fuellstand.set_round_trip(fuellstand.latency() + fuellstand.data().latency());

                    auto json = JSON::FuellstandFromServer(fuellstand);
                    std::cout << fuellstand.DebugString() << std::endl;
                    database_client->database("Sensor").collection(database).insert_one(bsoncxx::from_json(json.dump()));
                }
            }
        }
        catch (std::runtime_error &e)
        {
            perror(e.what());
        }
        client.close();
        usleep(1);
    }
    tcp_server.close();
}

void Server::kill_me_at_some_point()
{
    threads.emplace_back([]() {
        std::mt19937 engine{};
        std::uniform_int_distribution range{10, 60};
        sleep(range(engine));
        throw std::runtime_error("Killing server...");
    });
}