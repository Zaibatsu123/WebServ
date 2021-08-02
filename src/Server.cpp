#include "Server.hpp"

const std::string errorPageFolder = "";

Server::Server()
{
    // char *address = new char[10];
    // strlcpy(address, "127.0.0.1", 10);
    // t_socket *socket = new t_socket;
    // socket->address = address;
    // socket->port = 2142;
    // sockets.push_back(*socket);
    this->server_name = "127.0.0.1";
    // this->autoindex = 1;
    this->max_body_size = 1024;
    // this->locations.insert(std::pair<std::string, std::string>("/", "./root"));
	this->error_pages.insert(std::pair<int, std::string>(400, "./root/errorPages/400.html"));
	this->error_pages.insert(std::pair<int, std::string>(403, "./root/errorPages/403.html"));
	this->error_pages.insert(std::pair<int, std::string>(404, "./root/errorPages/404.html"));
	this->error_pages.insert(std::pair<int, std::string>(405, "./root/errorPages/405.html"));
	this->error_pages.insert(std::pair<int, std::string>(500, "./root/errorPages/500.html"));
	this->error_pages.insert(std::pair<int, std::string>(501, "./root/errorPages/501.html"));
	this->error_pages.insert(std::pair<int, std::string>(502, "./root/errorPages/502.html"));
	this->error_pages.insert(std::pair<int, std::string>(503, "./root/errorPages/503.html"));
	this->error_pages.insert(std::pair<int, std::string>(505, "./root/errorPages/505.html"));
}