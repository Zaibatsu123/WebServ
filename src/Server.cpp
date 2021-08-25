#include "Server.hpp"

const std::string errorPageFolder = "";

Server::Server()
{
	this->error_pages.insert(std::pair<int, std::string>(400, "./root/errorPages/400.html"));
	this->error_pages.insert(std::pair<int, std::string>(403, "./root/errorPages/403.html"));
	this->error_pages.insert(std::pair<int, std::string>(404, "./root/errorPages/404.html"));
	this->error_pages.insert(std::pair<int, std::string>(405, "./root/errorPages/405.html"));
    this->error_pages.insert(std::pair<int, std::string>(406, "./root/errorPages/406.html"));
    this->error_pages.insert(std::pair<int, std::string>(413, "./root/errorPages/413.html"));
	this->error_pages.insert(std::pair<int, std::string>(500, "./root/errorPages/500.html"));
	this->error_pages.insert(std::pair<int, std::string>(501, "./root/errorPages/501.html"));
	this->error_pages.insert(std::pair<int, std::string>(502, "./root/errorPages/502.html"));
	this->error_pages.insert(std::pair<int, std::string>(503, "./root/errorPages/503.html"));
	this->error_pages.insert(std::pair<int, std::string>(505, "./root/errorPages/505.html"));
}