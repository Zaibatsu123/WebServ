#include "Server.hpp"

Server::Server()
{
    // char *address = new char[10];
    // strlcpy(address, "127.0.0.1", 10);
    // t_socket *socket = new t_socket;
    // socket->address = address;
    // socket->port = 2142;
    // sockets.push_back(*socket);
    this->server_name = "default_server";
    this->autoindex = 1;
    this->max_body_size = 1024;
}