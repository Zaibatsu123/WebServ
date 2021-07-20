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

std::string cgiExec(){
	pid_t pid;
	int fds[2];
	int status;
	std::stringstream str;
	char  buffer[1024];

	pipe(fds);
	pid = fork();
	char *kek[3] = {(char *)"/bin/ls", (char *)".", 0};
	if (pid == -1){
		std::cout << "error" << std::endl;
		return 0;
	}
	else if (pid == 0){
		close(fds[0]);
		dup2(fds[1], 1);
		execve("/bin/ls", kek, 0);
		_exit(1); //TODO: delete exit
	}
	waitpid(pid, &status, 0);
	if (status >= 0){
		close(fds[1]);
		std::cout << "status: " << status << std::endl;
		while (read(fds[0], buffer, 1024) > 0){
			std::cout << "buffer: " << buffer << std::endl;
			str << buffer;
		}
	}
	return str.str();

}

ssize_t response(const int clientSocket, const std::string & request){
	Response* 	response = new Response;
	ssize_t		result;
	std::string	requestBody = "test_text";

	response->setMethod("get");
	response->setUplFileName("test.txt");
	response->setRoot("./root/");

	std::cout << request << std::endl;

	if (response->getMethod() == "get"){
		if (request.find("bg.jpg") != std::string::npos)
			response->setFileName(response->getRoot() + "bg.jpg");
		else if (request.find("style.css") != std::string::npos)
			response->setFileName(response->getRoot() + "style.css");
		else if (request.find("upload.html") != std::string::npos)
			response->setFileName(response->getRoot() + "upload.html");
		else
			response->setFileName(response->getRoot() + "index.html");

		//TODO: раскомментить для включения CGI
		//response->setFileName(response->getRoot() + "test.cgi");

		if (response->getFileName().find(".cgi") != std::string::npos){
			std::cout << "CGI" << std::endl;
			std::stringstream str;
			str <<	"<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n"
		  			"<title>CGI</title>\n</head>\n<body style=\"text-align: center;\">\n"
	   				"<div>\n<h1>Проверка работы CGI</h1>\n<h2>Ваши данные: ";
			str << cgiExec();
			str <<	"</h2>\n</div>\n<br>\n<br>\n<hr>\n"
		  			"<h3>equal-rights 0.1.23</h3>\n</body>\n</html>\n";
			response->_buffer = response->generateHeader(200) + str.str();
		}
		else{
//			std::cout << "---\n" << response->generateHeader(200) << "---\n";
			response->_buffer = response->generateResponse(response->getFileName());
		}
	}
	if (response->getMethod() == "post"){
		response->setFileName(response->getRoot() + "uploadSuccess.html");
		response->setUplRoot("./root/tmp/");
		response->_buffer = response->upload(response->getUplFileName(), requestBody.c_str(), response->getFileName());
	}
	result = send(clientSocket, response->_buffer.c_str(), response->_buffer.length(), 0);
	delete response;
	return result;
}

