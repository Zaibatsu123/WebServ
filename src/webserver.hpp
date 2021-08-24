#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Server.hpp"
#include "../inc/output.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Request.hpp"

class WebServer
{
    private:
        std::vector<Server*>    *__servers;
        std::list<t_socket *>   __binded_sockets;
        std::list<t_client *>   __clients;
        fd_set                  __read_fds;
        fd_set                  __write_fds;
        int                     __max_fd;
        size_t                  __delay_client_disconnect_sec;
        struct timeval          __select_delay_time;
        int                     request_count;

        t_client    *initClient(int new_client_socket, t_socket *parent);
        int         connectingNewClients();
        int         creatingSocketServers();
        int         addingSocketsToSets();
        int         checkIncomingRequests();
        int         checkOutcomingResponces();
        int         returnSameSocket(t_socket *socket);
        int         checkSameSocketOnServers();
        void        proccessRequestHead(std::list<t_client *>::iterator i);
        void        proccessRequestBody(std::list<t_client *>::iterator i);
        void        deleteOldClients();
        void        bindServersToSockets();
        int         checkServerName(std::string host, Server* server);

    public:
        WebServer();
        void    setServer(std::vector<Server*>  *servers);
        int     startServer();
        void    announceServerSettings();
        std::list<t_client *>::iterator clientDelete(std::list<t_client *>::iterator i);
        Server  *findDefaultServer(t_client *client);
};

int rebind(int listen_socket);
int bind_socket(int listen_socket);
int create_socket();
int master_process(std::vector<Server*> *servers);

#endif