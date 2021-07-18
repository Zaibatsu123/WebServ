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
#include "Config.hpp"

void console_promt(Config *configuration){
	std::cout << COLOR_GREY << "Loading complete." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_GREEN << "Welcome to Equal Rights WebServer." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_RED << "WARNING: This is development server. Do not use it in a production deployment." << COLOR_DEFAULT << std::endl;
	std::cout << "Use a production WSGI server instead." << COLOR_DEFAULT << std::endl;
	std::cout << "Running on http://" << configuration->address << ":" << configuration->port << "/ (Press CTRL+C to quit)" << std::endl;
}

int	main(int argc, char **argv, char **env){
    Config configuration;

    if (argc < 2 || !argv || !env)
    {
        std::cout << "Something wrong with parametrs!" << std::endl;
        return (EXIT_FAILURE);
    }
    if (configuration.parsingConfiguration(argv[1]) == EXIT_FAILURE)
        return (EXIT_FAILURE);
	console_promt(&configuration);
	if (master_process(&configuration) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}