#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Server.hpp"
#include "../inc/output.hpp"
#include "response/GoodResponse.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


class WebServer
{
    private:
        std::vector<Server*>    *__servers;
        std::list<t_client *>   __clients;
        fd_set                  __read_fds;
        fd_set                  __write_fds;
        int                     __max_fd;
        std::ofstream           logs;

        int     connectingNewClients();
        int     creatingSocketServers();
        int     addingSocketsToSets();
        int     checkIncomingRequests();
        int     checkOutcomingResponces();

    public:
        void    setServer(std::vector<Server*>  *servers);
        int     startServer();
        void    announceServerSettings();
        
};

int rebind(int listen_socket);
int bind_socket(int listen_socket);
int create_socket();
int master_process(std::vector<Server*> *servers);

#endif