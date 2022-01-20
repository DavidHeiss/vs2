#include <iostream>
#include <mutex>

#include "src/headquarter/HeadquarterUDP.h"
#include "src/headquarter/HeadquarterMQTT.h"

#include <mqtt/client.h>

int main()
{
    try
    {
        Headquarter::MQTT server{};
        server.join();
    }
    catch (std::exception &e)
    {
        perror(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}