//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../inc/output.hpp"
#include "Response.hpp"
#include <dirent.h>

#define MSG_NOSIGNAL 0x2000
#define CGI_INPUT_FILE "outputMy.txt"
#define CGI_OUTPUT_FILE "outputCGI.txt"


// char** generateCgiEnv(){
// 	char **env;

// 	try
// 	{
// 		env = new char* [sizeof(char*) * 4];
// 	}
// 	catch (std::exception & e){
// 		return 0;
// 	}
// 	env[0] = (char *)"REQUEST_METHOD=get";
// 	env[1] = (char *)"SERVER_PROTOCOL=HTTP/1.1";
// 	env[2] = (char *)"PATH_INFO=./root/info.php";
// 	env[3] = 0;
// 	return env;
// }

// void cgiChild(const std::string & cgiName) {

// 	char **env = generateCgiEnv();
// 	if (env == NULL){
// 		exit(2);
// 	}

// 	int in = open(CGI_INPUT_FILE, O_RDONLY, S_IRUSR | S_IWUSR);
// 	int out = open(CGI_OUTPUT_FILE, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

// 	if(in == -1 || out == -1)
// 		exit(2);
// 	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
// 		exit(2);
// 	execve(cgiName.c_str(), 0, env);
// 	exit(1);
// }

// std::string cgiParent(pid_t pid, Response* response){
// 	int					status;
// 	std::stringstream	str;

// 	waitpid(pid, &status, 0);

// 	if (WIFEXITED(status))
// 		status = WEXITSTATUS(status);

// 	if (status == EXIT_SUCCESS){
// 		std::ifstream inputCGI(CGI_OUTPUT_FILE, std::ifstream::in);

// 		if (!inputCGI.is_open()){
// 			std::cout << "cannot write to outputMY" << std::endl;
// 			response->setStatus(500);
// 			return response->generateBody();
// 		}

// 		std::string buf;
// 		while (std::getline(inputCGI, buf))
// 			str << buf << std::endl;
// 	}
// 	else if (status == EXIT_FAILURE){
// 		std::cout << "execve error" << std::endl;
// 		response->setStatus(502);
// 		return response->generateBody();
// 	}
// 	else {
// 		std::cout << "child error" << std::endl;
// 		response->setStatus(500);
// 		return response->generateBody();
// 	}
// 	return str.str();
// }

int autoindex(const char *directory, t_client *client, Response *response)
{
    DIR                 *dir = NULL;
    struct dirent       *de;
    std::stringstream   str;
	(void)client;

	std::cout << "Directory" << directory << std::endl;
    if ((dir = opendir(directory)) == NULL)
    {
        std::cout << strerror(errno) << std::endl;
        return (1);
    }
    str << "<!DOCTYPE html><html><head><title>Вы находитесь в директории: " << directory << "</title></head><body><H1>Autoindex</H1>";
    while(dir)
    {
        de = readdir(dir);
        if (!de)
            break;
        str << "<p><a href=\"" << client->server->locations["/"] << "/" << de->d_name << "\">" << de->d_name << "</a></p>" << std::endl;
    }
    str << "</body></html>";
	response->setBody(str.str());
    // std::ofstream file("answer.html");
    // if (!file)
    //     std::cout << "File opening error!" << std::endl;
    // file << str.str();
    closedir(dir);
	return (0);
}


#define CGI "./root/myCGI"

int file_or_directory_existing(t_client *client, Response *response)
{
	std::ifstream		file;

	std::cout << "AUTOINDEX ENTER" << std::endl;
	std::string fullpath = client->server->locations["/"].c_str();
	fullpath += client->request->getPath();
	if (client->server->autoindex == 1)
	{
		std::cout << "path:|" << client->server->locations["/"] +  client->request->getPath() << "|" << std::endl;
		file.open(fullpath + "index.html");
		if (file.is_open()){
			std::cout << "path:|" << fullpath + "index.html" << "|" << std::endl;
			response->setRoot(fullpath);
			response->setFileName("index.html");
			file.close();
			return (1);
		}
		if (!autoindex(fullpath.c_str(), client, response))
		{
			std::cout << "after autoindex" << std::endl;
			return (2);
		}
		file.open(fullpath);
		if (file.is_open()){
			std::cout << "FILEpath:|" << fullpath << "|" << std::endl;
			response->setRoot(client->server->locations["/"]);
			// response->setFileName(client->request->getPath());
			file.close();
			return (3);
		}
		else
			return (404);
	}
	return (0);
}

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;
	int res;

	Response* response = new Response("/", client->request->getPath());
	if (client->request->getErr() != 0){
		response->setStatus(400);
	}
	else{
		if (client->request->getMethod() == "GET"){
			std::cout << "--> GET" << std::endl;
			// requestFileValidator(response);
			res = file_or_directory_existing(client, response);
			std::cout << "res checking" << res << std::endl;
			if (res == 2){
				std::string buffer = response->generateResponse(1);
				std::cout << "RESPONSE CHEEEEECK:____________________________" << std::endl;
				std::cout << buffer << std::endl;
				std::cout << "____________________________" << std::endl;
				ssize_t result = sendall(client->socket, buffer, MSG_NOSIGNAL);
				return (result);
			}
			else if(res == 404)
				response->setStatus(404);
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
			if (ret == EXIT_FAILURE)
				response->setFileName("/uploadFailure.html");
			else
				response->setFileName("/uploadSuccess.html");
		}
	}
	std::cout << response->generateHeader(0) << std::endl;
	std::string buffer = response->generateResponse(0);


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

	if (!srcFile.is_open()){
		response->setStatus(404);
		return false;
	}
	srcFile.close();
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

