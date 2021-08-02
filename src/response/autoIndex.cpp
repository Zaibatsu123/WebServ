#include <dirent.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../../inc/output.hpp"

int autoindex(const char *directory, t_client *client, AResponse *response)
{
	DIR                 *dir = NULL;
	struct dirent       *de;
	std::stringstream   str;
	(void)client;

	std::cout << "Directory" << directory << std::endl;
	if ((dir = opendir(directory)) == NULL)
	{
		std::cout << strerror(errno) << std::endl;
		return (1);
	}
	std::string prefix = "/";
	if (client->request->getPath()[client->request->getPath().size() - 1] == '/')
		prefix = "";
	str << "<!DOCTYPE html><html><head><title>Вы находитесь в директории: " << directory << "</title></head><body><H1>Autoindex</H1>";
	while(dir)
	{
		de = readdir(dir);
		if (!de)
			break;
		if (!strcmp(de->d_name, "."))
			continue;
		str << "<p><a href=\""  << client->request->getPath() << prefix << de->d_name << "\">" << de->d_name << "</a></p>" << std::endl;
	}
	str << "</body></html>";
	response->setBody(str.str());
	// std::ofstream file("answer.html");
	// if (!file)
	//     std::cout << "File opening error!" << std::endl;
	// file << str.str();
	closedir(dir);
	return (0);
}

int file_or_directory_existing(t_client *client, AResponse *response)
{
	std::ifstream		file;

	std::cout << "AUTOINDEX ENTER" << std::endl;
	std::string fullpath = client->server->locations["/"]->root;
	fullpath += client->request->getPath();
	if (client->server->locations["/"]->autoindex == 1)
	{
		std::cout << "path:|" << fullpath + "/index.html" << "|" << std::endl;
		std::cout << "internet path:|" << client->request->getPath() << "|" << std::endl;
		file.open(fullpath + "/index.html");
		if (file.is_open()){
			std::cout << "path:|" << fullpath + "index.html" << "|" << std::endl;
			response->setRoot(fullpath);
			response->setFileName("/index.html");
			file.close();
			return (1);
		}
		if (!autoindex(fullpath.c_str(), client, response))
		{
			std::cout << "after autoindex" << std::endl;
			return (2);
		}
		file.open(fullpath);
		if (file.is_open()){
			std::cout << "FILEpath:|" << fullpath << "|" << std::endl;
			response->setRoot(client->server->locations["/"]->root);
			// response->setFileName(client->request->getPath());
			file.close();
			return (3);
		}
		else
			return (404);
	}
	return (0);
}

bool is_good(Request* request, AResponse* response){
	int result = 0;

	std::list<std::string>::const_iterator it;
	for (it = response->getAllowedMethods().begin(); it != response->getAllowedMethods().end(); ++it){
		if (*it == (request->getMethod()))
			result += 1;
	}
	if (!request->getPath().empty() && request->getPath().at(0) == '/')
		result += 1;

	if (request->getProtocol() == "HTTP/1.1" || request->getProtocol() == "HTTP/1.0")
		result += 1;

	if (!request->getHost().empty())
		result += 1;
	return result == 4 ? false : true;
}