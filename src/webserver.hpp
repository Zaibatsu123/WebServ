#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Server.hpp"
#include "../inc/output.hpp"
#include "response/GoodResponse.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

class WebServer
{
    private:
        int                     request_count;
        std::vector<Server*>    *__servers;
        std::list<t_client *>   __clients;
        fd_set                  __read_fds;
        fd_set                  __write_fds;
        int                     __max_fd;
        std::ofstream           logs;

        t_client    *initClient(int new_client_socket, Server *server);
        int         connectingNewClients();
        int         creatingSocketServers();
        int         addingSocketsToSets();
        int         checkIncomingRequests();
        int         checkOutcomingResponces();
        void        proccessRequestHead(std::list<t_client *>::iterator i);
        void        proccessRequestBody(std::list<t_client *>::iterator i);
        void        deleteOldClients();

    public:
        WebServer();
        void    setServer(std::vector<Server*>  *servers);
        int     startServer();
        void    announceServerSettings();
        std::list<t_client *>::iterator clientDelete(std::list<t_client *>::iterator i);
};

int rebind(int listen_socket);
int bind_socket(int listen_socket);
int create_socket();
int master_process(std::vector<Server*> *servers);

#endif