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

//----------------------------------------------------------------------------------
typedef struct  s_socket 
{
    int         socket;
    char        *address;
    int         port;
}               t_socket;

typedef struct  s_location 
{
    std::string location;
    std::string root;
}               t_location;

class   Server
{
    public:
        std::vector<t_socket>               sockets;
        std::string                         server_name;
        std::map<std::string, std::string>  locations;
        std::map<int, std::string>          error_pages;
        int                                 max_body_size; //по умолчанию ?
        int                                 autoindex;                                 
    public:
        Server();
        ~Server() {};
};

typedef struct  s_client 
{
    int         socket;
    int         status;
    std::string buffer;
    Request     *request;
}               t_client;

//----------------------------------------------------------------------------------

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


// void    configurationPrint(std::vector<Server> servers)
// {
//     std::cout << "___________________________" << std::endl;
//     std::cout << "PARSED CONFIG:" << std::endl;
//     std::cout << "IP:" << "|" << this->__address << "|" << std::endl;
//     std::cout << "PORT:" << "|" << this->__port << "|" << std::endl;
//     std::cout << "___________________________" << std::endl;
// }

int parsingConfiguration(char *config_name)
{
    std::vector<std::string>    configuration = readFile(config_name);
    std::string                 next_string;
    std::vector<Server>         servers;
    Server                      *temp = new Server;


    if (configuration.size() == 0)
        return (EXIT_FAILURE);
    for (std::vector<std::string>::iterator iter = configuration.begin(); iter != configuration.end(); iter++)
    {
        if ((*iter).find("server") != std::string::npos)
        {
            server>__address = new char[strlen((*iter).substr((*iter).find("ip") + 3).c_str()) + 1];
            memcpy(this->__address, (*iter).substr((*iter).find("ip") + 3).c_str(), strlen((*iter).substr((*iter).find("ip") + 3).c_str()));
            this->__address[strlen((*iter).substr((*iter).find("ip") + 3).c_str())] = '\0';
        }
        else if((*iter).find("port") != std::string::npos)
            this->__port = std::stoi((*iter).substr((*iter).find("port") + 5));
    }
    // if (checkigConfiguration() == EXIT_FAILURE)
    //     return (EXIT_FAILURE);
    // configurationPrint(servers);
    return (EXIT_SUCCESS);
}


// server
//     listen 127.0.0.1:80
//     listen 127.0.0.1:808
//     listen 127.0.0.1:8088
//     server_name www.pornhub.com
//     auto_index on
//     location /
//         root ~/www/
//     location /images/
//         root ~/www/images/