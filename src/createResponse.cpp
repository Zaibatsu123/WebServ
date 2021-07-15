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
	int result;
	std::string uri;
	//TODO: getfromparce
	std::string root = "./";
//	TODO: validate uri
	std::string fileName = "";
	std::cout << request << std::endl;
	if (request.find("bg.jpg") != std::string::npos)
		fileName = root + "bg.jpg";
	else if (request.find("style.css") != std::string::npos)
		fileName = root + "style.css";
	else
		fileName = root + "index.html";
	Response* response = new Response;

	//TODO: validate ifstream
	std::ifstream srcFile(fileName.c_str());
	std::stringstream resBuffer;
	std::string buffer;
	resBuffer << response->getHeader();
	while (std::getline(srcFile, buffer)){
		resBuffer << buffer;
		resBuffer << "\n";
	}
	response->setResponse(resBuffer.str());
	result = send(clientSocket,response->getResponse().c_str(), response->getResponse().length(), 0);
//	std::cout << response->getResponse();
	delete response;
	return result;
}

