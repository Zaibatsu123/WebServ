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
#include <sys/wait.h>

ssize_t response(s_client client){
	std::cout << "--------------------> Response part!! <------------ " << std::endl;

	Response* 	response = new Response;
	ssize_t		result;

// debug variables
	std::string	requestBody = "test_text";
	std::string	cgiName = "/usr/bin/php";

// if parse errors
//	if (client.request->getErr())
//		return 0;

// init configs
	response->setUplRoot("./root/tmp/");
	response->setRoot("./root");
	response->setMethod("get");
	response->setFileName(response->getRoot() + client.request->getPath());
//	std::cout << response->getFileName() << std::endl;
//	response->setFileName(response->getRoot() + "/bg.jpg");

// get method
	if (response->getMethod() == "get"){
		std::cout << "GET!!" << std::endl;

	// if need use CGI
//		if (response->getFileName().find("/info.php") != std::string::npos){
//			std::cout << "CGI" << std::endl;
//			std::stringstream str;
//			std::string cgiString = response->cgi(cgiName);
//			response->_fileSize = cgiString.length();
//			response->_buffer = response->generateHeader() + cgiString;
//		}
//		else{
			response->_buffer = response->generateResponse(response->getFileName());
//		std::cout << response->_buffer << std::endl;
//		}
	}

// post method
	if (response->getMethod() == "post"){
		std::cout << "POST!!" << std::endl;

		response->setFileName(response->getRoot() + "uploadSuccess.html");
		response->_buffer = response->upload(response->getUplFileName(), requestBody.c_str(), response->getFileName());
	}

// send
	result = 0;
	int it = 1;
	do {
		result = send(client.socket, response->_buffer.c_str(), response->_buffer.length(), 0);

		if (static_cast<int>(result) == -1)
			perror("");
		std::cout
		<< "---- Pack: " << it++ << "\n"
		<< "Data Left:\t" << response->_buffer.length() << "\n"
		<< "Send Result:\t" << result
		<< std::endl;

	//TODO: delete sleep for delay after send
		usleep(1000);

		// break loop if all data send
		if (result == static_cast<ssize_t>(response->_buffer.length()))
			break;

		try
		{
			response->_buffer = response->_buffer.substr(result);
		}
		catch (std::exception & e){
		}

	} while (result);

	std::cout << "--------------------> Response END!! <------------ " << std::endl;
	delete response;
	delete client.request;
	return result;
}

