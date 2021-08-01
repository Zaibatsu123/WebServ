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
    std::string root;
    std::string index;
    int         autoindex;
    int         methods;
}               t_location;

class   Server
{
    public:
        std::vector<t_socket>               sockets;
        std::string                         server_name;
        std::map<std::string, t_location*>  locations;
        std::map<int, std::string>          error_pages;
        long long int                       max_body_size;
        std::string                         upload_file_to;
    public:
        Server();
        ~Server() {};
};
#endif
