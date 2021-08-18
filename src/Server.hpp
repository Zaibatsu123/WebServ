#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <list>
#include <vector>
#include <time.h>

class   Server;

typedef struct  s_socket 
{
	int         				socket;
	char        				*address;
	int         				port;
	std::vector<Server *>		servers;
}               t_socket;

typedef struct  s_location 
{
	std::string	root;
	std::string	index;
	std::string redirect;
	int			autoindex;
	int			methods;
	long long int						max_body_size;	
}				t_location;

class   Server
{
	public:
		std::vector<t_socket *>             sockets;
		std::vector<std::string>            server_name;
		std::map<std::string, t_location*>	locations;             
		std::map<int, std::string>          error_pages;
		std::string                         upload_file_to;
	public:
		Server();
		~Server() {};
};

#endif