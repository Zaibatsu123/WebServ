//                                   //
// Created by:                       //
//              Equal rights team    //
//         on:                       //
//              7/07/21              //
//                                   //

#include <iostream>
#include "../inc/output.hpp"

int	main(int argc, char *argv[], char *env[]){
	if (argc > 1 && argv && env)
		std::cout << "Loading parameters..." << std::endl;
	std::cout << COLOR_GREY << "Loading complete." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_GREEN << "Welcome to Equal Rights WebServer." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_RED << "WARNING: This is development server. Do not use it in a production deployment." << COLOR_DEFAULT << std::endl;
	std::cout << "Use a production WSGI server instead." << std::endl;
	std::cout << "Running on http://127.0.0.1:5000/ (Press CTRL+C to quit)" << std::endl;
	while (true){
	}
	return 0;
}