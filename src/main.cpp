//                                   //
// Created by:                       //
//              Equal rights team    //
//         on:                       //
//              7/07/21              //
//                                   //

#include "../inc/output.hpp"
#include "response/GoodResponse.hpp"
#include <stdlib.h>
#include "webserver.hpp"
#include "Server.hpp"
#include <map>

int	main(int argc, char **argv, char **env){
    std::vector<Server*> *servers = NULL;
    WebServer webserver;

    if (argc != 2 || !argv || !env || !argv[1])
    {
        std::cout << "Something wrong with parametrs!" << std::endl;
        return (EXIT_FAILURE);
    }
    if ((servers = parsingConfiguration(argv[1])) == NULL)
        return (EXIT_FAILURE);
    // while(1){}
    webserver.setServer(servers);
    webserver.announceServerSettings();
    if (webserver.startServer() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}