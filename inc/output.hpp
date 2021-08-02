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
#include "../src/response/GoodResponse.hpp"
#include "../src/response/BadResponse.hpp"
#include "../src/response/CgiResponse.hpp"
#include "../src/response/AutoIndexResponse.hpp"
#include <sys/types.h>
#include <sys/wait.h>

#include <dirent.h>

#define MSG_NOSIGNAL 0x2000

#define CGI "./root/cgi_tester"

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

ssize_t						response(t_client *client);
std::vector<Server*>		*parsingConfiguration(char *config_name);
std::string					trim(std::string old_string);
std::string					trim_end(std::string old_string);
std::vector<std::string>	splitvector(std::vector<std::string> old_vector, std::string str);
bool						requestFileValidator(AResponse * response);
ssize_t						sendall(int socket, std::string & buffer, int flags);
int							upload(const std::string & uplFileName, const char *data);
std::string					cgi(const std::string & cgiName, AResponse* response);
bool						requestContentSizeValidator(AResponse *response);

bool is_good(Request* request, AResponse* response);
int file_or_directory_existing(t_client *client, AResponse *response);

ssize_t methodGet(s_client* client, AResponse* response);
void methodPost(s_client* client, AResponse* response);
void methodDelete(AResponse* response);
int upload(const std::string & uplFileName, const char *data);

#endif //OUTPUT_HPP
