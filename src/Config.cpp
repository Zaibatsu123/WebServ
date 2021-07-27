#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <list>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include "Request.hpp"
#include "../inc/output.hpp"
#include  "Server.hpp"

std::vector<std::string> readFile(char *config_name)
{
	std::ifstream               config_file(config_name);
	std::string                 temp;
	std::vector<std::string>    configuration;

	std::cout << "Start configuration reading" << std::endl;
	if (!config_file)
	{
		std::cerr << "Configuration file not found!" << std::endl;
		return (configuration);
	}
	while (config_file)
	{
		std::getline(config_file, temp);
		if (config_file.eof())
		{
			configuration.push_back(temp);
			break;
		}
		else
			configuration.push_back(temp);
	}
	config_file.close();
	return (configuration);
}

std::vector<Server>  *pars(std::vector<Server> *servers, std::vector<std::string> *configuration, int begin, int end)
{
	Server                      *temp = new Server;

	for (int i = begin; i < end; ++i)
	{
		std::string str = trim_end((*configuration)[i]);
		if (str.compare(0, 11, "    listen ") == 0)
		{
			t_socket *ts = new t_socket;
			ts->address = new char[strlen(str.substr(11, str.find(":") - 11).c_str()) + 1];
			memcpy(ts->address, str.substr(11, str.find(":") - 11).c_str(), strlen(str.substr(12, str.find(":") - 11).c_str()) + 1);
			ts->address[strlen(str.substr(11, str.find(":") - 11).c_str())] = '\0';
			ts->port = std::stoi(str.substr(str.find(":") + 1, str.length() - str.find(":")));
			temp->sockets.push_back(*ts);
			delete ts;
		}
		else if (str.compare(0, 16, "    server_name ") == 0)
			temp->server_name = str.substr(16, str.length() - 16);
		else if (str.compare(0, 17, "    auto_index on") == 0)
			temp->autoindex = 1;
		else if (str.compare(0, 18, "    auto_index off") == 0)
			temp->autoindex = 0;
		else if (str.compare(0, 14, "    location /") == 0 && str[str.length() - 1] == '/' && (*configuration)[i+1].compare(0, 13, "        root ") == 0)
		{
			std::string sstr = trim_end((*configuration)[++i]);
			sstr = sstr.substr(13, sstr.length() - 13);
			temp->locations.insert(std::make_pair(str.substr(13, str.length() - 13), sstr));
		}
		else if (str != "server")
			return (NULL);		
	}
	servers->push_back(*temp);
	delete temp;
	return (servers);
}


std::vector<Server> *parsingConfiguration(char *config_name)
{
	std::vector<Server>         *servers = new std::vector<Server>;
	std::vector<std::string>    configuration = readFile(config_name);
	int cnt = 0, begin = 0, end = 0;

	for (size_t i = 0; i < configuration.size(); ++i)
		if (configuration[i] == "server")
			cnt += 1;
	if (configuration.size() == 0 || cnt == 0)
		return (NULL);
	if (cnt == 1)
		servers = pars(servers, &configuration, 0, configuration.size());
	else if (cnt > 1)
	{
		for (size_t i = 0; i < configuration.size(); ++i)
		{
			if (configuration[i] == "server")
			{
				begin = i;
				for (size_t j = i + 1; j < configuration.size(); ++j)
					if (configuration[j] == "server" || j + 1 == configuration.size())
					{
						end = j + 1;
						std::cout << "\033[1;46m3333.2\033[0m" << begin << end <<std::endl;
						servers = pars(servers, &configuration, begin, end);
						break;
					}
			}
		}
	}
	std::cout << "\033[1;46m3333.3\033[0m" << (*servers)[0].sockets[0].address<< std::endl;
	return (servers);
}

// int main(void)
// {
// 	std::vector<Server>     *servers;
// 	char *str = new char[10];
// 	strcpy(str, "test.conf");
// 	servers = parsingConfiguration(str);
// 	for (std::vector<Server>::iterator i = servers->begin(); i != servers->end(); i++)
// 	{
// 		for (std::vector<t_socket>::iterator j = (*i).sockets.begin(); j != (*i).sockets.end(); j++)
// 			std::cout << "Running on http://" << (*j).address << ":" << (*j).port << "/ (Press CTRL+C to quit)" << std::endl;
// 		for (std::map<std::string, std::string>::iterator it = (*i).locations.begin(); it != (*i).locations.end(); it++ )
// 			std::cout << "location: " << it->first << " root: " << it->second << std::endl;
// 	}
// 	// while(1)
// 	// 	continue;
// }

