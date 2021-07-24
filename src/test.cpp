#include "Server.hpp"
#include <iostream>

int main(void)
{
    Server server;
    std::cout << (*server.sockets.begin()).address << std::endl;
    return (0);
}