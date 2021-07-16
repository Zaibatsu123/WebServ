//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../inc/output.hpp"
#include "Response.hpp"
#include <fstream>
#include <sstream>

int response(const int clientSocket, const std::string & request){
	int			result;
	std::string	fileName;
	Response* 	response = new Response;
	std::string	root = "./";
	//TODO: getfromparce

	std::cout << request << std::endl;
	if (request.find("bg.jpg") != std::string::npos)
		fileName = root + "bg.jpg";
	else if (request.find("style.css") != std::string::npos)
		fileName = root + "style.css";
	else
		fileName = root + "index.html";
//	fileName = "test";
	//TODO: validate ifstream
	std::string			buffer;
//	response->display(fileName);
	std::cout << "---\n" << response->generateHeader(200) << "---\n";
	buffer = response->generateResponse(fileName);
	result = send(clientSocket, buffer.c_str(), buffer.length(), 0);
	delete response;
	return result;
}

