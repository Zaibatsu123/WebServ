//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/8/21				 //
//                                   //

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <map>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include "../src/Request.hpp"
#include "../src/Server.hpp"
#include "../src/Response.hpp"
#include <sys/types.h>
#include <sys/wait.h>

#define COLOR_DEFAULT "\e[0m"
#define COLOR_RED "\e[31m"
#define COLOR_GREEN "\e[32m"
#define COLOR_GREY "\e[37m"

typedef struct  s_client 
{
    Server      *server;
    int         socket;
    int         status;
    std::string buffer;
    Request     *request;
}               t_client;

ssize_t response(t_client *client);
// ssize_t response(s_client client);
bool requestFileValidator(Response * response);
ssize_t sendall(int socket, std::string & buffer, int flags);
int upload(const std::string & uplFileName, const char *data);
std::string cgi(const std::string & cgiName, Response* response);
bool requestContentSizeValidator(Response *response);

//Config.cpp
std::vector<Server*> *parsingConfiguration(char *config_name);

// Utils.cpp
std::vector<std::string> splitvector(std::vector<std::string> old_vector, std::string str);
std::string trim(std::string old_string);
std::string trim_end(std::string old_string);
std::string rslash_from_end(std::string string);
std::string rduplicate_slashes(std::string string);


#endif //OUTPUT_HPP
