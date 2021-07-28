#include "Server.hpp"

Server::Server()
{
    // char *address = new char[10];
    // strlcpy(address, "127.0.0.1", 10);
    // t_socket *socket = new t_socket;
    // socket->address = address;
    // socket->port = 2142;
    // sockets.push_back(*socket);
	error_pages.insert(std::pair<int, std::string>(403, "./root/errorPages/403.html"));
	error_pages.insert(std::pair<int, std::string>(400, "./root/errorPages/400.html"));
	error_pages.insert(std::pair<int, std::string>(404, "./root/errorPages/404.html"));
	error_pages.insert(std::pair<int, std::string>(405, "./root/errorPages/405.html"));
	error_pages.insert(std::pair<int, std::string>(500, "./root/errorPages/500.html"));
	error_pages.insert(std::pair<int, std::string>(501, "./root/errorPages/501.html"));
	error_pages.insert(std::pair<int, std::string>(502, "./root/errorPages/502.html"));
	error_pages.insert(std::pair<int, std::string>(503, "./root/errorPages/503.html"));
	error_pages.insert(std::pair<int, std::string>(505, "./root/errorPages/505.html"));
    this->server_name = "default_server";
    this->autoindex = 1;
    this->max_body_size = 1024;
}