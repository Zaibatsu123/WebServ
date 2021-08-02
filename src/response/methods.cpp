//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "../../inc/output.hpp"

ssize_t methodGet(s_client* client, AResponse* response){
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
	return 0;
}

void methodPost(s_client* client, AResponse* response){
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

void methodDelete(AResponse* response){
	std::remove((response->getRoot() + response->getFileName()).c_str());
	response->setStatus(505);
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
