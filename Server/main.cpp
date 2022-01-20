#include <iostream>

#include "Libary/Server.h"

int main()
{
    try
    {
        Server server{"mongodb://mongodb:27017"};
        server.sync("Backup");
        server.kill_me_at_some_point();
        server.join();
    }
    catch (std::exception &e)
    {
        perror(e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}