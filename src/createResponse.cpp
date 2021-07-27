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
#include <sys/types.h>
#include <sys/socket.h>
#define MSG_NOSIGNAL 0x2000

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part!! <------------ " << std::endl;

	Response* 	response = new Response;

// if parse errors
if (client->request->getErr())
	return 0;

// init configs
	response->setUplRoot("./root/tmp");
	response->setRoot("./root");
//	response->setMethod(client.request);
	response->setFileName(client->request->getPath());
	std::cout << client->request->getMethod() << std::endl;
// get method
	if (client->request->getMethod() == "GET"){
		std::cout << "--> GET" << std::endl;
	// if need use CGI
		if (response->getFileName().find(".php") != std::string::npos){
			std::cout << "----> CGI" << std::endl;
			response->_buffer = response->generateResponseCGI();
		}
		else {
			response->_buffer = response->generateResponse();
		}
	}

// post method
	if (client->request->getMethod() == "POST"){
		std::cout << "POST!!" << std::endl;
		//TODO: add body from parse
		response->_buffer = response->upload(client->request->getBody().c_str());
//		response->_buffer = response->upload("Здесь мог бы быть файл");
	}

// send
	std::cout << response->generateHeader() << std::endl;
	std::cout << response->getFileSize() << std::endl;
	ssize_t result = 0;

	int it = 1;
	// if ((result = send(client->socket, response->_buffer.c_str(),response->_buffer.length(), 0)) == -1)
	// 			std::cout << "SEND ERROR: " << result << strerror(errno) << std::endl;
	// if ((result = send(client->socket, response->_buffer.c_str(), response->_buffer.length(), 0)) == -1)
	// 	std::cout << "SEND ERROR: " << result << strerror(errno) << std::endl;
	// if (result > 0)
	// {
		do {
			if ((result = send(client->socket, response->_buffer.c_str(),response->_buffer.length(), MSG_NOSIGNAL)) == -1)
				std::cout << "SEND ERROR: " << result << strerror(errno) << std::endl;
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
				std::cout << "EXCEPION!" << std::endl;
				// continue;
			}

		} while (result);
	// }

	// delete response;
	// delete client->request;
	std::cout << "--------------------> Response END!! <------------ " << std::endl;
	return result;
}
