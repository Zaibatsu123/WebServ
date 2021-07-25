#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <list>
#include <vector>

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
        std::vector<t_socket>             sockets;
        std::string                         server_name;
        std::map<std::string, std::string>  locations;
        std::map<int, std::string>          error_pages;
        int                                 max_body_size; //по умолчанию ?
        int                                 autoindex;                                 
    public:
        Server();
        ~Server() {};
};

#endif