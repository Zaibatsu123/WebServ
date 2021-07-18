#ifndef CONFIG_HPP
#define CONFIG_HPP

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

typedef struct  s_client 
{
    int         socket;
    int         status;
    std::string buffer;
}               t_client;

class Config
{
    public:
        class ConfigException : public std::exception
        {
            private:
                std::string __error;
            public:
                ConfigException(std::string error);
                virtual const char* what() const throw();
                virtual ~ConfigException() throw() {};
        };

        Config();
        std::list<t_client> clients;
        int                 port;
        char                *address;
        int                 *fd;
        int                 socket;

        int                 getPort();
        char                *getAddress();
        int                 readConfig();
        int                 parsingConfiguration();
        ~Config();
};

#endif