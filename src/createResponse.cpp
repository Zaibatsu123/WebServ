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
#include "Config.hpp"

ssize_t response(s_client client){
	std::cout << "--------------------> Response part!! <------------ " << std::endl;
	Response* 	response = new Response;
	ssize_t		result;
	std::string	requestBody = "test_text";
	std::string	cgiName = "/usr/bin/php";
	std::string	name = "index.html";
//

	response->setRoot("./root");
	response->setMethod("get");
//	response->setFileName(response->getRoot() + client.request->getPath());
	response->setFileName(response->getRoot() + "index.html");
//	response->setMethod(client.request->getMethod());
	response->setFileName(response->getRoot() + client.request->getPath());
//
	std::cout << "RESPONSE!!!!" << std::endl;
//	response->setUplFileName(response->getRoot() + name);
	std::cout << response->getFileName() << std::endl;
	std::cout << "END!" << std::endl;

////	std::cout << client.socket << std::endl;
//	std::cout << client.buffer << std::endl;
//	std::cout << "ERROR!" << std::endl;
//	std::cout << client.request->getErr() << std::endl;
////	std::cout << client.request->getMethod() << std::endl;


	if (response->getMethod() == "get"){
		std::cout << "GET!!" << std::endl;

		//TODO: раскомментить для включения CGI
//		response->setFileName(response->getRoot() + "info.php");

		if (response->getFileName().find("info.php") != std::string::npos){
			std::cout << "CGI" << std::endl;
			std::stringstream str;
			std::string cgiString = response->cgi(cgiName);
			response->_fileSize = cgiString.length();
			response->_buffer = response->generateHeader() + cgiString;
		}
		else{
			response->_buffer = response->generateResponse(response->getFileName());
			std::cout << "--- HEADER ---\n" << response->generateHeader();
		}
	}
	if (response->getMethod() == "post"){
		std::cout << "POST!!" << std::endl;
		response->setFileName(response->getRoot() + "uploadSuccess.html");
		response->setUplRoot("./root/tmp/");
		response->_buffer = response->upload(response->getUplFileName(), requestBody.c_str(), response->getFileName());
		std::cout << "--- HEADER ---\n" << response->generateHeader();
	}

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
//			std::cout << "Cannot substring data: " << e.what() << std::endl;
		}

	} while (result);

	std::cout << "--------------------> Response END!! <------------ " << std::endl;
	delete response;
	delete client.request;
	return result;
}

