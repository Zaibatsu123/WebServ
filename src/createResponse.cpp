//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../inc/output.hpp"
#include "Response.hpp"

#define MSG_NOSIGNAL 0x2000

//#define CGI "./root/cgi_tester"
#define CGI "./root/myCGI"

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;
	Response* response;

	std::string method = "GET";
	std::string file = "/index.html";
	std::string protocol = "HTTP/1.1";

	try{
		response = new Response(10000,"./root", client->request->getPath());
	}
	catch (std::exception & e){
		std::cout << e.what() << std::endl;
	}

	if (client->request->getErr() != 0)
		response->setStatus(400);
	else{
		if (client->request->getMethod() == "GET"){
			std::cout << "--> GET" << std::endl;
			requestFileValidator(response);
			if (client->request->getPath().find(".php") != std::string::npos){
				std::cout << "----> CGI" << std::endl;

				std::string cgiString = cgi(CGI, response);
				response->setFileSize(cgiString.length());
				response->setRoot("");
				response->setFileName("outputCGI.txt");
				std::cout << response->getFileName() << std::endl;
				//TODO: FIX CGI OUT BUFFER
				//response->_buffer = response->generateResponseCGI(CGI, cgi);
			}
		}

		if (client->request->getMethod() == "POST"){
			std::cout << "--> POST" << std::endl;

			int ret = upload(client->request->getFilename(), client->request->getBodyCnt().c_str());
			if (ret == EXIT_FAILURE)
				response->setFileName("/uploadFailure.html");
			else
				response->setFileName("/uploadSuccess.html");
		}

		if (client->request->getMethod() == "DELETE"){
			std::remove((response->getRoot() + response->getFileName()).c_str());
			response->setStatus(505);
		}
	}

	requestContentSizeValidator(response);
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

bool requestFileValidator(Response * response){
	std::ifstream	srcFile;
	srcFile.open((response->getRoot() + response->getFileName()).c_str(), std::ifstream::in);

	//TODO: delete when evaluate
	std::cout << response->getRoot() + response->getFileName() << std::endl;
	if (!srcFile.is_open()){
		response->setStatus(404);
		return false;
	}
	srcFile.close();
	return true;
}

bool requestContentSizeValidator(Response *response){
	std::ifstream	srcFile;
	srcFile.open((response->getRoot() + response->getFileName()).c_str(), std::ifstream::in);

	long long size;
	if (!srcFile.is_open()){
		response->setStatus(404);
		return false;
	}

	srcFile.seekg (0, srcFile.end);
	size = srcFile.tellg();
	srcFile.seekg (0, srcFile.beg);
	srcFile.close();

	if (size > response->getMaxContent()){
		response->setStatus(413);
		return false;
	}
	return true;
}

int upload(const std::string & uplFileName, const char *data) {
	std::ofstream	dstFile;

	if (!data || !std::strlen(data) || !uplFileName.length()){
		std::cout << "--> Error: Empty File <--" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "upload to: " << uplFileName << std::endl;
	dstFile.open(uplFileName.c_str(), std::ofstream::out);

	if (!dstFile.is_open())
		return EXIT_FAILURE;

	dstFile << std::string(data);
	dstFile.close();

	return EXIT_SUCCESS;
}

