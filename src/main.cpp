//                                   //
// Created by:                       //
//              Equal rights team    //
//         on:                       //
//              7/07/21              //
//                                   //

#include "../inc/output.hpp"
#include "Response.hpp"
#include <stdlib.h>
#include "webserver.hpp"
#include "Server.hpp"

void console_promt(std::vector<Server> *servers){
	std::cout << COLOR_GREY << "Loading complete." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_GREEN << "Welcome to Equal Rights WebServer." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_RED << "WARNING: This is development server. Do not use it in a production deployment." << COLOR_DEFAULT << std::endl;
	std::cout << "Use a production WSGI server instead." << COLOR_DEFAULT << std::endl;
    // std::cout << servers->size() << std::endl;
    int o = 1;
    for (std::vector<Server>::iterator i = servers->begin(); i != servers->end(); i++)
    {
        // std::cout << i->sockets.size() << std::endl;
        for (std::vector<t_socket>::iterator j = (*i).sockets.begin(); j != (*i).sockets.end(); j++)
        {
            if ((*j).port == 2142)
            {
                std::cout << "!!!" << std::endl;
                (*j).port += o;
                o += 1;
            }
            // std::cout << "socket:" << std::endl;
            // std::cout << "address1 " << (&(*j).address) << std::endl;
            // std::cout << "socket1 " << &(*j) << std::endl;
            std::cout << "Running on http://" << (*j).address << ":" << (*j).port << "/ (Press CTRL+C to quit)" << std::endl;
        }
    }
    // std::cout << COLOR_GREY << "Promt end" << COLOR_DEFAULT << std::endl;
}

int	main(int argc, char **argv, char **env){
    std::vector<Server> *servers = NULL;

    if (argc != 2 || !argv || !env || !argv[1])
    {
        std::cout << "Something wrong with parametrs!" << std::endl;
        return (EXIT_FAILURE);
    }
    if ((servers = parsingConfiguration(argv[1])) == NULL)
        return (EXIT_FAILURE);
    while (1);
    // std::cout << "1111" << std::endl;
    console_promt(servers);
    if (master_process(servers) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}