#include "Server.hpp"
#include <iostream>

std::vector<Server> *parsing_configuration(char *config_name)
{
    (void)config_name;
    std::vector<Server> *servers = new std::vector<Server>;
    Server *server1 = new Server;
    Server *server2 = new Server;
    t_socket *socket1 = new t_socket;
    t_socket *socket2 = new t_socket;
    char *address1 = NULL;
    char *address2 = NULL;
    address1 = new char[10];
    address2 = new char[10];
    strcpy(address1, "127.0.0.1");
    strcpy(address2, "127.0.0.1");
    socket1->port = 8808;
    socket2->port = 9909;
    socket1->address = address1;
    socket2->address = address2;
    server1->sockets.push_back(*socket1);
    server2->sockets.push_back(*socket2);
    servers->push_back(*server1);
    servers->push_back(*server2);
    return (servers);
}