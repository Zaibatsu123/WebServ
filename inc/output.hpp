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

//createResponse.cpp
ssize_t						response(t_client *client);
ssize_t						sendall(int socket, std::string & buffer, int flags);

//methods.cpp
ssize_t		methodGet(s_client* client, AResponse* response);
void		methodPost(s_client* client, AResponse* response);
void		methodDelete(AResponse* response);
int			upload(const std::string & uplFileName, const char *data);

//cgiHandler.cpp
std::string					cgi(const std::string & cgiName, AResponse* response);

//other
std::vector<Server*>		*parsingConfiguration(char *config_name);
std::string					trim(std::string old_string);
std::string					trim_end(std::string old_string);
std::vector<std::string>	splitvector(std::vector<std::string> old_vector, std::string str);
int							file_or_directory_existing(t_client *client, AResponse *response);

//Config.cpp
std::vector<Server*> *parsingConfiguration(char *config_name);

//Utils.cpp
std::vector<std::string> splitvector(std::vector<std::string> old_vector, std::string str);
std::string trim(std::string old_string);
std::string trim_end(std::string old_string);
std::string rslash_from_end(std::string string);
std::string rduplicate_slashes(std::string string);
std::vector<std::string> getarray(std::string req);

std::string alter_trim_end(std::string old_string, std::string elems);
std::string rrepeats_from_end(std::string string);
std::string rslash_from_end(std::string string);
std::string rduplicate_slashes(std::string string);
t_location *get_location(std::string root, std::map<std::string, t_location *> *locations);

#endif //OUTPUT_HPP
