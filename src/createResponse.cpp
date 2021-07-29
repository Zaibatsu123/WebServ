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
#include <string>
#include <dirent.h>

#define MSG_NOSIGNAL 0x2000
#define CGI_INPUT_FILE "outputMy.txt"
#define CGI_OUTPUT_FILE "outputCGI.txt"


char** generateCgiEnv(){
	char **env;

	try
	{
		env = new char* [sizeof(char*) * 4];
	}
	catch (std::exception & e){
		return 0;
	}
	env[0] = (char *)"REQUEST_METHOD=get";
	env[1] = (char *)"SERVER_PROTOCOL=HTTP/1.1";
	env[2] = (char *)"PATH_INFO=./root/info.php";
	env[3] = 0;
	return env;
}

void cgiChild(const std::string & cgiName) {

	char **env = generateCgiEnv();
	if (env == NULL){
		exit(2);
	}

	int in = open(CGI_INPUT_FILE, O_RDONLY, S_IRUSR | S_IWUSR);
	int out = open(CGI_OUTPUT_FILE, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

	if(in == -1 || out == -1)
		exit(2);
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		exit(2);
	execve(cgiName.c_str(), 0, env);
	exit(1);
}

std::string cgiParent(pid_t pid, Response* response){
	int					status;
	std::stringstream	str;

	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
		status = WEXITSTATUS(status);

	if (status == EXIT_SUCCESS){
		std::ifstream inputCGI(CGI_OUTPUT_FILE, std::ifstream::in);

		if (!inputCGI.is_open()){
			std::cout << "cannot write to outputMY" << std::endl;
			response->setStatus(500);
			return response->generateBody();
		}

		std::string buf;
		while (std::getline(inputCGI, buf))
			str << buf << std::endl;
	}
	else if (status == EXIT_FAILURE){
		std::cout << "execve error" << std::endl;
		response->setStatus(502);
		return response->generateBody();
	}
	else {
		std::cout << "child error" << std::endl;
		response->setStatus(500);
		return response->generateBody();
	}
	return str.str();
}

//#define CGI "./root/cgi_tester"
#define CGI "./root/myCGI"

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;
	
	Response* response = new Response("./root", client->request->getPath());
	file_or_directory_existing(client->request->getFilename());
	if (client->request->getErr() != 0){
		response->setStatus(400);
		response->setFileSize(378);
	}
	else{
		if (client->request->getMethod() == "GET"){
			std::cout << "--> GET" << std::endl;
			requestFileValidator(response);
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

void requestFileValidator(Response *response){
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

	std::cout << "upload to: " << uplFileName << std::endl;

	dstFile.open(uplFileName.c_str(), std::ofstream::out);

	if (!dstFile.is_open())
		return EXIT_FAILURE;

	dstFile << std::string(data);
	dstFile.close();

	return EXIT_SUCCESS;
}

