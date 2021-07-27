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

#define CHILD 0
#define FAILURE -1
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



std::string cgi(const std::string & cgiName, Response* response){

	std::ofstream outMy(CGI_INPUT_FILE,std::ofstream::out);

	if (!outMy.is_open()){
		std::cout << "cannot write to outputMY" << std::endl;
		response->setStatus(500);
		return response->generateBody();
	}

	outMy << response->generateBody();
	outMy.close();

	pid_t pid = fork();
	if (pid == FAILURE){
		std::cout << "fork error" << std::endl;
		response->setStatus(500);
		return response->generateBody();
	}
	else if (pid == CHILD){
		cgiChild(cgiName);
	}
	return cgiParent(pid, response);
}


std::string upload(const char *data, Response* response) {
	std::ofstream	dstFile;

	std::cout << "upload to: " << response->getUplFileName() << std::endl;

	dstFile.open(response->getUplFileName().c_str(), std::ofstream::out);

	if (!dstFile.is_open()){
		response->setFileName("/uploadFailure.html");
		response->setFileSize(307);
		return response->generateHeader() + response->generateBody();
	}
	dstFile << std::string(data);
	dstFile.close();

	//TODO: KOSTYL!
	response->setFileSize(249);
	response->setFileName("/uploadSuccess.html");

	return response->generateHeader() + response->generateBody();
}

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;

	if (client->request->getErr())
		return 0;

	Response* response = new Response("./root", client->request->getPath());

	if (client->request->getMethod() == "GET"){
		std::cout << "--> GET" << std::endl;

		if (response->getFileName().find(".php") != std::string::npos){
			std::cout << "----> CGI" << std::endl;
			response->_buffer = response->generateResponseCGI(cgi);
		}
		else {
			response->_buffer = response->generateResponse();
		}
	}

	if (client->request->getMethod() == "POST"){
		std::cout << "--> POST" << std::endl;

		response->setUplFileName(client->request->getFilename());
		response->_buffer = upload(client->request->getBodyCnt().c_str(), response);
	}

//	std::cout << response->generateHeader() << std::endl;

	ssize_t result;
	int it = 1;
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

		if (result == static_cast<ssize_t>(response->_buffer.length()))
			break;
		try{
			response->_buffer = response->_buffer.substr(result);
		}
		catch (std::exception & e){
			std::cout << e.what() << std::endl;
		}
	} while (result);

	delete response;
	delete client->request;
	std::cout << "--------------------> Response END <------------ " << std::endl;
	return result;
}
