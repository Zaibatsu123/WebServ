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

//#define CGI "./root/cgi_tester"
#define CGI "./root/myCGI"

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;

	Response* response = new Response("./root", client->request->getPath());

	if (client->request->getErr() != 0){
		response->setStatus(400);
		response->setFileSize(378);
	}
	else{

		if (client->request->getMethod() == "GET"){
			std::cout << "--> GET" << std::endl;

			if (client->request->getPath().find(".php") != std::string::npos){
				std::cout << "----> CGI" << std::endl;

				std::string cgiString = cgi(CGI, response);
				response->setFileSize(cgiString.length());
				response->setFileName("outputCGI");
				//TODO: FIX CGI OUT BUFFER
				//response->_buffer = response->generateResponseCGI(CGI, cgi);
			}
		}

		if (client->request->getMethod() == "POST"){
			std::cout << "--> POST" << std::endl;

			int ret = upload(client->request->getFilename(), client->request->getBodyCnt().c_str());
			if (ret == EXIT_FAILURE){
				response->setFileName("/uploadFailure.html");
				response->setFileSize(307);
			}
			else {
				response->setFileName("/uploadSuccess.html");
				response->setFileSize(249);
			}
		}

		requestFileValidator(response);
	}

	std::string buffer = response->generateResponse();

	ssize_t result = sendall(client->socket, buffer, MSG_NOSIGNAL);

	delete response;
	delete client->request;
	std::cout << "--------------------> Response END <------------ " << std::endl;
	return result;
}

ssize_t sendall(int socket, std::string & buffer, int flags){
	ssize_t result;
	int it = 1;
	do {
		if ((result = send(socket, buffer.c_str(),buffer.length(), flags)) == -1)
			std::cout << "SEND ERROR: " << result << strerror(errno) << std::endl;
		if (static_cast<int>(result) == -1)
			perror("");
		std::cout
				<< "---- Pack: " << it++ << "\n"
				<< "Data Left:\t" << buffer.length() << "\n"
				<< "Send Result:\t" << result
				<< std::endl;

		//TODO: delete sleep for delay after send
		usleep(1000);

		if (result == static_cast<ssize_t>(buffer.length()))
			break;
		try{
			buffer = buffer.substr(result);
		}
		catch (std::exception & e){
			std::cout << e.what() << std::endl;
		}
	} while (result);
	return result;
}

void requestFileValidator(Response * response){
	std::ifstream	srcFile;
	srcFile.open((response->getRoot() + response->getFileName()).c_str(), std::ifstream::in);

	if (!srcFile.is_open()){
		response->setStatus(404);
		//TODO: KOSTYL!
		response->setFileSize(409);
	}
	else {
		srcFile.seekg (0, srcFile.end);
		response->setFileSize(srcFile.tellg());
		srcFile.seekg (0, srcFile.beg);
	}
}

int upload(const std::string & uplFileName, const char *data) {
	std::ofstream	dstFile;

	if (!data || !std::strlen(data))
		return EXIT_FAILURE;
	std::cout << "upload to: " << uplFileName << std::endl;

	dstFile.open(uplFileName.c_str(), std::ofstream::out);

	if (!dstFile.is_open())
		return EXIT_FAILURE;

	dstFile << std::string(data);
	dstFile.close();

	return EXIT_SUCCESS;
}

