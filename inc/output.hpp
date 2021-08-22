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
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

#include "../src/Request.hpp"
#include "../src/Server.hpp"
#include "../src/response/GoodResponse.hpp"
#include "../src/response/BadResponse.hpp"
#include "../src/response/CgiResponse.hpp"
#include "../src/response/AutoIndexResponse.hpp"
#include "../src/response/RedirectResponse.hpp"
#include "../src/response/PutResponse.hpp"

#define CGI_OUTPUT "outputCGI.txt"
#define CGI "./root/cgi_tester"
#define MiB 1048576
#define COLOR_DEFAULT "\e[0m"
#define COLOR_RED "\e[31m"
#define COLOR_GREEN "\e[32m"
#define COLOR_GREY "\e[37m"

#ifdef __linux__
# include <algorithm>
#endif

#ifdef __APPLE__
# define MSG_NOSIGNAL 0x2000
#endif

typedef struct  s_client
{
    int         	socket;
    int         	status;
    int         	getRequestHead;
    std::string 	buffer;
	std::string 	head;
	std::string 	body;
	std::string 	needle;
	Request     	*request;
	std::string 	responseBuffer;
	bool 			responseNotSend;
	struct timeval 	time;
	t_socket		*parent;
}               t_client;

//createResponse.cpp
ssize_t	response(t_client *client);
ssize_t	sendall(s_client* client);

//methods.cpp
AResponse*	methodGet(s_client* client);
AResponse*	methodPost(s_client* client);
AResponse*	methodDelete(s_client* client);
AResponse*	methodPut(s_client* client);
std::string	getUploadFileName(s_client* client);
int			upload(s_client* client);

//cgiHandler.cpp
int		cgi(const std::string & cgiName, const std::string & pathToFile, s_client* client);
char	**generateEnv(s_client* client);

//other
std::vector<Server*>		*parsingConfiguration(char *config_name);
std::string					trim(std::string old_string);
std::string					trim_end(std::string old_string);
AResponse*					file_or_directory_existing(t_client *client);

//Config.cpp
std::vector<Server*> *parsingConfiguration(char *config_name);

//Utils.cpp
std::vector<std::string> splitvector(std::vector<std::string> old_vector, std::string str);
std::string trim(std::string old_string);
std::string trim_end(std::string old_string);
std::string rslash_from_end(std::string string);
std::string rduplicate_slashes(std::string string);
std::vector<std::string> getarray(std::string req);
int count_str(std::string input_str, std::string str);
std::string content(std::string string, std::string boundary);

std::string alter_trim_end(std::string old_string, std::string elems);
std::string rrepeats_from_end(std::string string);
std::string rslash_from_end(std::string string);
std::string rduplicate_slashes(std::string string);
t_location *get_location(std::string root, std::map<std::string, t_location *> *locations);

#endif //OUTPUT_HPP
