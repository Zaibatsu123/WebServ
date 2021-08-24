//                                   //
// Created by:                       //
//              Equal rights team    //
//         on:                       //
//              7/07/21              //
//                                   //

#include "../inc/output.hpp"
#include "../inc/global.hpp"
#include "webserver.hpp"
#include "Server.hpp"

extern Logger logs;

int	main(int argc, char **argv, char **env){
    std::vector<Server*> *servers = NULL;
    WebServer webserver;

    if (argc != 2 || !argv || !env || !argv[1])
    {
        logs << "Something wrong with parametrs!\n";
        return (EXIT_FAILURE);
    }
    if ((servers = parsingConfiguration(argv[1])) == NULL)
        return (EXIT_FAILURE);
    webserver.setServer(servers);
    if (webserver.startServer() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}