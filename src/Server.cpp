#include "Server.hpp"

Server::Server()
{
    t_socket socket = {-1, "127.0.0.1", 80};
    sockets.push_back(socket);
    this->autoindex = 1;
}