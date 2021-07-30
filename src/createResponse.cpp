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

//#define CGI "./root/cgi_tester"
#define CGI "./root/myCGI"

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
	std::string prefix = "/";
	if (client->request->getPath()[client->request->getPath().size() - 1] == '/')
		prefix = "";
	str << "<!DOCTYPE html><html><head><title>Вы находитесь в директории: " << directory << "</title></head><body><H1>Autoindex</H1>";
	while(dir)
	{
		de = readdir(dir);
		if (!de)
			break;
		if (!strcmp(de->d_name, "."))
			continue;
		str << "<p><a href=\""  << client->request->getPath() << prefix << de->d_name << "\">" << de->d_name << "</a></p>" << std::endl;
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

int file_or_directory_existing(t_client *client, Response *response)
{
	std::ifstream		file;

	std::cout << "AUTOINDEX ENTER" << std::endl;
	std::string fullpath = client->server->locations["/"].c_str();
	fullpath += client->request->getPath();
	if (client->server->autoindex == 1)
	{
		std::cout << "path:|" << fullpath + "/index.html" << "|" << std::endl;
		std::cout << "internet path:|" << client->request->getPath() << "|" << std::endl;
		file.open(fullpath + "/index.html");
		if (file.is_open()){
			std::cout << "path:|" << fullpath + "index.html" << "|" << std::endl;
			response->setRoot(fullpath);
			response->setFileName("/index.html");
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

	std::cout << client->request->getMethod() << std::endl;
	Response* response;
	try{
		response = new Response(0,"./root", client->request->getPath());
	}
	catch (std::exception & e){
		std::cout << e.what() << std::endl;
	}

	if (client->request->getErr() != 0)
		//TODO: set to 400
		response->setStatus(400);
	else{

		if (client->request->getMethod() == "GET"){
			std::cout << "--> GET" << std::endl;

			int res;
			res = file_or_directory_existing(client, response);
			std::cout << "res checking" << res << std::endl;
			if (res == 2){
				std::string buffer = response->generateResponse(1);
				ssize_t result = sendall(client->socket, buffer, MSG_NOSIGNAL);
				return (result);
			}
			else if(res == 404)
				response->setStatus(404);


			if (client->request->getPath().find(".php") != std::string::npos){
				std::cout << "----> CGI" << std::endl;

				std::string cgiString = cgi(CGI, response);
				response->setFileSize(cgiString.length());
				response->setRoot("");
				response->setFileName("outputCGI.txt");
			}
		}


		if (client->request->getMethod() == "POST"){
			std::cout << "--> POST" << std::endl;

			if (client->request->getBodyCnt().length() == 0)
				response->setStatus(405);
			else{
				int ret = upload(client->request->getFilename(), client->request->getBodyCnt().c_str());
				if (ret == EXIT_FAILURE)
					response->setFileName("/uploadFailure.html");
				else
					response->setFileName("/uploadSuccess.html");
			}
		}

		if (client->request->getMethod() == "DELETE"){
			std::remove((response->getRoot() + response->getFileName()).c_str());
			response->setStatus(505);
		}
	}
	std::cout << response->generateHeader(0) << std::endl;
	std::string buffer = response->generateResponse(0);


	ssize_t result = sendall(client->socket, buffer, MSG_NOSIGNAL);

//	delete response;
//	delete client->request;
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

	srcFile.seekg (0, srcFile.end);
	size = srcFile.tellg();
	srcFile.seekg (0, srcFile.beg);
	srcFile.close();

	if (response->getMaxContent() && size > response->getMaxContent()){
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


