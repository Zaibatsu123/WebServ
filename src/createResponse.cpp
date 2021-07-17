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

int response(const int clientSocket, const std::string & request){
	int			result = 0;
	std::string	fileName;
	std::string	buffer;
	std::string method = "get";
	std::string dstFileName = "test.txt";
	std::string fileData = "test_text";
	Response* 	response = new Response;
	std::string	root = "./root/";
	//TODO: getfromparce

	std::cout << request << std::endl;
	if (method == "get"){
		if (request.find("bg.jpg") != std::string::npos)
			fileName = root + "bg.jpg";
		else if (request.find("style.css") != std::string::npos)
			fileName = root + "style.css";
		else if (request.find("upload.html") != std::string::npos)
			fileName = root + "upload.html";
		else
			fileName = root + "index.html";
//TODO: раскомментить для включения CGI
//		fileName = root + "test.cgi";
		if (fileName.find(".cgi") != std::string::npos){
			std::cout << "CGI" << std::endl;
			std::stringstream str;
			str << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<title>CGI</title>\n</head>\n<body style=\"text-align: center;\">\n<div>\n<h1>Проверка работы CGI</h1>\n<h2>Ваши данные: ";
			str << cgiExec();
			str << "</h2>\n</div>\n<br>\n<br>\n<hr>\n<h3>equal-rights 0.1.23</h3>\n</body>\n</html>\n";
			buffer = response->generateHeader(200) + str.str();
		}
		else{
			//TODO: validate ifstream
		//	response->display(fileName);
			std::cout << "---\n" << response->generateHeader(200) << "---\n";
			buffer = response->generateResponse(fileName);
		}
	}
	if (method == "post"){
		fileName = root + "uploadSuccess.html";
		response->setUplRoot("./root/tmp/");
		buffer = response->upload(dstFileName, fileData.c_str(), fileName);
	}
	result = send(clientSocket, buffer.c_str(), buffer.length(), 0);
	delete response;
	return result;
}

