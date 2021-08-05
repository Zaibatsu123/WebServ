//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/5/21				 //
//                                   //

#include "../inc/output.hpp"

std::string recvAcceptor(const std::string & buffer){
	std::cout << "-------> Request Acceptor Begin <---------" << std::endl;
	size_t pos = buffer.find("\n\r\n\r", 0);
	if (pos != std::string::npos){
		std::string head = buffer.substr(0, pos);
		std::string body = buffer.substr(pos + 2);

		std::cout << "Head:--" << std::endl;
		std::cout << head << std::endl;
		std::cout << "Body:--" << std::endl;
		std::cout << body << std::endl;
	}
	std::cout << "-------> Request Acceptor End <---------" << std::endl;
	return "";
}