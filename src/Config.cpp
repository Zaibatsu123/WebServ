//                                          ┏┻━━┻┓        //
// Created by:                        ╭┓┏╮  ┃▕▏▕▏┃  ╭┓┏╮  //
//        Jolene Radioactive          ┃┗┛┃  ┃┏┳┳┓┃  ┃┗┛┃  //
//         on:                        ╰┳┳╯  ┃┗┻┻┛┃  ╰┳┳╯  //
//              07/21				   ┃┃ ┏━┻━━━━┻━┓ ┃┃   //
//                                     ┃╰━┫╭━━━━━━╮┣━╯┃   //
//                                     ╰━━┫┃╱╲╱╲╱╲┃┣━━╯   //

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

std::vector<std::string> *readFile(char *config_name)
{
	std::ifstream               config_file(config_name);
	std::string                 temp;
	std::vector<std::string>    *configuration = new std::vector<std::string>;

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
			configuration->push_back(temp);
			break;
		}
		else
			configuration->push_back(temp);
	}
	config_file.close();
	return (configuration);
}

Server  *print_error(Server *temp, int i, int flag)
{
	if (flag == 1)
		std::cerr << "Configuration file:" << i + 1 << " Error not found" << std::endl;
	else if (flag == 2)
		std::cerr << "Configuration file:" << i + 1 << " Incorrect path" << std::endl;
	else if (flag == 3)
		std::cerr << "Configuration file:" << i + 1 << " Error reading file" << std::endl;
	temp = NULL;
	return (temp);
}

Server  *error_pages(Server *temp, std::string str, int i)
{
	std::map <int, std::string>::iterator it;
	int error;
	try
	{
		error = std::stoi(str.substr(15, 4));
	}
	catch(const std::exception& e)
	{
		std::cerr << "Configuration file:" << i + 1 << " Invalid error number" << std::endl;
		return (NULL);
	}
	it = temp->error_pages.find(error);
	if (it == temp->error_pages.end())
		temp = print_error(temp, i, 1);

	else
		it->second = trim(str.substr(19, str.length() - 19));
	if (it != temp->error_pages.end() && it->second.empty())
		temp = print_error(temp, i, 2);
	else
	{
		std::fstream fs(it->second, std::fstream::in);
		if (!fs.is_open())
			temp = print_error(temp, i, 3);
		else
			std::cout << COLOR_RED << it->first << "| |" << it->second << COLOR_RED << std::endl;
		fs.close();
	}
	return(temp);
}

Server  *listen(Server *temp, std::string str, int i)
{
	t_socket *ts = new t_socket;
	std::string ip =  trim(str.substr(11, str.find(":") - 11));
	ts->address = new char[strlen(ip.c_str()) + 1];
	memcpy(ts->address, ip.c_str(), strlen(str.substr(12, str.find(":") - 11).c_str()) + 1);
	ts->address[strlen(ip.c_str())] = '\0';
	try
	{
		ts->port = std::stoi(str.substr(str.find(":") + 1, str.length() - str.find(":")));
	}
	catch(const std::exception& e)
	{
		std::cerr << "Configuration file:" << i + 1 << " Incorrect port" << std::endl;
		delete ts;
		return (NULL);
	}
	temp->sockets.push_back(*ts);
	delete ts;
	return(temp);
}

std::vector<Server>  *pars(std::vector<Server> *servers, std::vector<std::string> *configuration, int begin, int end)
{
	Server                      *temp = new Server;

	for (int i = begin; i < end; ++i)
	{
		std::cout << (*configuration)[i] << std::endl;
		std::string str = trim_end((*configuration)[i]);
		if (str.compare(0, 11, "    listen ") == 0)
		{
			if ((temp = listen(temp, str, i)) == NULL)
				return (NULL);
		}
		else if (str.compare(0, 16, "    server_name ") == 0)
			temp->server_name = str.substr(16, str.length() - 16);
		else if (str.compare(0, 17, "    auto_index on") == 0)
			temp->autoindex = 1;
		else if (str.compare(0, 18, "    auto_index off") == 0)
			temp->autoindex = 0;
		else if (str.compare(0, 14, "    location /") == 0 && str[str.length() - 1] == '/' && (*configuration)[i+1].compare(0, 13, "        root ") == 0)
		{
			std::string root = trim_end((*configuration)[++i]);
			root = root.substr(13, root.length() - 14);
			temp->locations.insert(std::make_pair(str.substr(13, str.length() - 14), root));
		}
		else if (str.compare(0, 15, "    error_page ") == 0)
		{
			if ((temp = error_pages(temp, str, i)) == NULL)
			{
				delete temp;
				return (NULL);
			}
		}
		else if (str != "server")
		{
			std::cerr << "Configuration file:" << i + 1 << str << " Unknown directive" << std::endl;
			delete temp;
			return (NULL);		
		}
	}
	servers->push_back(*temp);
	delete temp;
	return (servers);
}


std::vector<Server> *parsingConfiguration(char *config_name)
{
	std::vector<Server>         *servers = new std::vector<Server>;
	std::vector<std::string>    *configuration = readFile(config_name);
	int cnt = 0, begin = 0, end = 0;

	for (size_t i = 0; i < configuration->size(); ++i)
		if ((*configuration)[i] == "server")
			cnt += 1;
	if (configuration->size() == 0 || cnt == 0)
	{
		std::cerr << "Configuration file: no mandatory directives" << std::endl;
		return (NULL);
	}
	if (cnt == 1)
		servers = pars(servers, configuration, 0, configuration->size());
	else if (cnt > 1)	
	{
		for (size_t i = 0; i < configuration->size(); ++i)
		{
			if ((*configuration)[i] == "server")
			{
				begin = i;
				for (size_t j = i + 1; j < configuration->size(); ++j)
					if ((*configuration)[j] == "server" || j + 1 == configuration->size())
					{
						end = j + 1;
						// std::cout << "\033[1;46m3333.2\033[0m" << begin << end <<std::endl;
						if ((servers = pars(servers, configuration, begin, end)) == NULL)
							return (NULL);
						break;
					}
			}
		}
	}
	delete configuration;
	// std::cout << "\033[1;46m3333.3\033[0m" << (*servers)[0].sockets[0].address<< std::endl;
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

