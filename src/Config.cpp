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
#include <dirent.h>
#include <sys/types.h>

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
	else if (flag == 4)
		std::cerr << "Configuration file:" << i + 1 << " Incorrect size" << std::endl;
	else if (flag == 5)
		std::cerr << "Configuration file:" << i + 1 << " Unknown directive in location" << std::endl;
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
	if (temp != NULL && it != temp->error_pages.end() && it->second.empty())
		temp = print_error(temp, i, 2);
	else if (temp != NULL)
	{
		std::fstream fs(it->second, std::fstream::in);
		if (!fs.is_open())
			temp = print_error(temp, i, 3);
		else
			fs.close();
	}
	return(temp);
}

Server  *listen(Server *temp, std::string str, int i)
{
	t_socket *ts = new t_socket;
	if (str.find(":") == std::string::npos)
	{
		delete ts;
		return (NULL);
	}
	try
	{
		std::string ip =  trim(str.substr(11, str.find(":") - 11));
		ts->address = new char[strlen(ip.c_str()) + 1];
		memcpy(ts->address, ip.c_str(), strlen(str.substr(12, str.find(":") - 11).c_str()) + 1);
		ts->address[strlen(ip.c_str())] = '\0';
		ts->port = std::stoi(str.substr(str.find(":") + 1, str.length() - str.find(":")));
	}
	catch(const std::exception& e)
	{
		std::cerr << "Configuration file:" << i + 1 << " Incorrect ip or port" << std::endl;
		delete ts;
		return (NULL);
	}
	temp->sockets.push_back(*ts);
	delete ts;
	return(temp);
}

void init(t_location *lctn)
{
	lctn->index = "";
	lctn->root = "";
	lctn->methods = 0;
	lctn->autoindex = 0;
}

int getAllowsMethods(std::string str, int i)
{
	if (str.compare("GET") == 0)
		return (4);
	else if (str.compare("POST") == 0)
		return (2);
	else if (str.compare("DELETE") == 0)
		return (1);
	else if (str.compare("GET POST") == 0 || str.compare("POST GET") == 0)
		return (6);
	else if (str.compare("GET DELETE") == 0 || str.compare("DELETE GET") == 0)
		return (5);
	else if (str.compare("POST DELETE") == 0 || str.compare("DELETE POST") == 0)
		return (3);
	else if (str.compare("GET POST DELETE") == 0 || str.compare("DELETE POST GET") == 0 || str.compare("POST DELETE GET") == 0)
		return (7);
	else if (str.compare("GET DELETE POST") == 0 || str.compare("DELETE GET POST") == 0 || str.compare("POST GET DELETE") == 0)
		return (7);
	std::cerr << "Configuration file:" << i + 1 << " Unknown method" << std::endl;
	return (-1);
}

Server  *location(Server *temp, std::vector<std::string> *configuration, int i)
{
	t_location *lctn = new t_location;
	init(lctn);
	std::string loc = trim_end((*configuration)[i]);
	int k = 0;
	for (size_t j = i + 1; k == 0 && j < (*configuration).size(); ++j)
	{
		if (j + 1 != (*configuration).size())
			k = (*configuration)[j + 1].compare(0, 8, "        ");
		if ((*configuration)[j].compare(0, 13, "        root ") == 0)
			lctn->root = trim((*configuration)[j].substr(13, (*configuration)[j].length() - 14));
		else if ((*configuration)[j].compare(0, 14, "        index ") == 0)
			lctn->index = trim((*configuration)[j].substr(14, (*configuration)[j].length() - 14));
		else if ((*configuration)[j].compare(0, 21, "        auto_index on") == 0)
			lctn->autoindex = 1;
		else if ((*configuration)[j].compare(0, 22, "        auto_index off") == 0)
			lctn->autoindex = 0;
		else if ((*configuration)[j].compare(0, 22, "        allow_methods ") == 0)
			lctn->methods = getAllowsMethods(trim((*configuration)[j].substr(22, (*configuration)[j].length() - 22)), j);
		else 
			temp = print_error(temp, i, 5);
	}
	if (lctn->methods == - 1)
	{
		delete lctn;		
		return (NULL);
	}
	if (temp != NULL)
	{
		std::string ll = "/" + (loc.substr(14, loc.length() - 15));
		temp->locations[ll] = lctn;
	}
	return (temp);
}

Server  *upload_file_to(Server *temp, std::string str, int i)
{
	DIR *dir = NULL;
	temp->upload_file_to = trim(str.substr(19, str.length() - 19));
	if ((dir = opendir(temp->upload_file_to.c_str())) == NULL)
    {
        std::cerr << "Configuration file: " << i + 1 << " " << strerror(errno) << std::endl;
        return (NULL);
    }
	return (temp);
}

Server  *max_body_size(Server *temp, std::string str, int i)
{
	size_t n = std::count(str.begin(), str.end(), ' ');
	std::string size = str.substr(str.length() - 1, 1);
	int kb = 1;
	if (n > 1 || (size != "M" && size != "K" && size != "G"))
		temp = print_error(temp, i, 4);
	if (size == "M")
		kb = 1024;
	else if (size == "G") 
		kb = 1024 * 1024;
	try
	{
		if (temp != NULL && std::stoll(str.substr(0, str.length() - 2)) > 0)
			temp->max_body_size = std::stoll(str.substr(0, str.length() - 2)) * kb;
	}
	catch(const std::exception& e)
	{
		temp = print_error(temp, i, 4);
	}
	return (temp);
}

std::vector<Server*>  *pars(std::vector<Server*> *servers, std::vector<std::string> *configuration, int begin, int end)
{
	Server                      *temp = new Server;

	for (int i = begin; i < end && temp != NULL; ++i)
	{
		std::string str = trim_end((*configuration)[i]);
		if (str.compare(0, 11, "    listen ") == 0)
			temp = listen(temp, str, i);
		else if (str.compare(0, 16, "    server_name ") == 0)
			temp->server_name = str.substr(16, str.length() - 16);
		else if (str.compare(0, 14, "    location /") == 0 && str[str.length() - 1] == '/' && (*configuration)[i+1].compare(0, 13, "        root ") == 0)
			temp = location(temp, configuration, i);
		else if (str.compare(0, 15, "    error_page ") == 0)
			temp = error_pages(temp, str, i);
		else if (str.compare(0, 19, "    upload_file_to ") == 0)
			temp = upload_file_to(temp, str, i);
		else if (str.compare(0, 18, "    max_body_size ") == 0)
			temp = max_body_size(temp, trim(str.substr(18, str.length() - 18)), i);
		else if (str == "server" || str.compare(0, 8, "        ") == 0)
			continue;
		else
		{
			std::cerr << "Configuration file:" << i + 1 << " Unknown directive" << std::endl;
			temp = NULL;
		}
	}
	if (temp == NULL)
		return (NULL);
	servers->push_back(temp);
	return (servers);
}

// void print_serv(std::vector<Server*> *servers)
// {
// 	int k = 1;
// 	for (std::vector<Server *>::iterator i = servers->begin(); i != servers->end(); i++)
// 	{
// 		std::cout << "---------===SERVER===---------" << "num: " << k <<  std::endl;
// 		std::cout << "---------===Sockets===---------" << std::endl;
// 		for (std::vector<t_socket>::iterator j = (*i)->sockets.begin(); j != (*i)->sockets.end(); j++)
// 			std::cout << "Running on http://" << (*j).address << ":" << (*j).port << "/ (Press CTRL+C to quit)" << std::endl;
// 		std::cout << "---------===locations===---------size" << (*i)->locations.size() << std::endl;
// 		for (std::map<std::string, t_location *>::iterator it = (*i)->locations.begin(); it != (*i)->locations.end(); it++ )
// 		{
// 			std::cout << "location: " << (*it).first  << std::endl;
// 			std::cout << "second: " << (*i)->locations[(*it).first]->index  << std::endl;
// 			std::cout << "autoindex: " << (*it).second->autoindex << " root: " << (*it).second->root << std::endl;
// 			std::cout << "index: " << (*it).second->index << " methods: " << (*it).second->methods << std::endl;
// 			std::cout << "---------======---------" << std::endl;
// 		}
// 		k++;
// 	}
// }

std::vector<Server*> *parsingConfiguration(char *config_name)
{
	std::vector<Server*>         *servers = new std::vector<Server*>;
	std::vector<std::string>    *configuration = readFile(config_name);
	int cnt = 0, begin = 0, end = 0;

	for (size_t i = 0; i < configuration->size(); ++i)
	{
		if ((*configuration)[i] == "server")
			cnt += 1;
		if (trim((*configuration)[i]).empty())
		{
			std::cerr << "Configuration file: "<< i + 1 << " Empty string" << std::endl;
			return (NULL);
		}
	}
	if (configuration->size() == 0 || cnt == 0)
	{
		std::cerr << "Configuration file: no mandatory directives" << std::endl;
		return (NULL);
	}
	if (cnt == 1)
	{
		if ((servers = pars(servers, configuration, 0, configuration->size())) == NULL)
			return (NULL);
	}
	else if (cnt > 1)	
		for (size_t i = 0; i < configuration->size(); ++i)
			if ((*configuration)[i] == "server")
			{
				begin = i + 1;
				for (size_t j = i + 1; j < configuration->size(); ++j)
					if ((*configuration)[j] == "server" || j + 1 == configuration->size())
					{
						end = j + 1;
						if ((servers = pars(servers, configuration, begin, end)) == NULL)
							return (NULL);
						break;
					}
			}
	delete configuration;
	// if (servers != NULL)
	// 	print_serv(servers);
	return (servers);
}

// int main(void)
// {
// 	std::vector<Server*>     *servers;
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

