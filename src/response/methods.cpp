//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "../../inc/output.hpp"

AResponse* methodGet(s_client* client, AResponse* response){
	std::cout << "--> GET" << std::endl;

	if (client->request->getPath().find(".php") != std::string::npos){
		std::cout << "----> CGI" << std::endl;
		AResponse* response1 = new CgiResponse();
		//TODO: buffer string not used
		std::string cgiString = cgi(CGI, response);
		response1->setFileSize(cgiString.length());
		response1->setRoot("");
		response1->setFileName("outputCGI.txt");
		return response1;
	}

	AResponse* resp;
	resp = file_or_directory_existing(client, response);

	return resp;
}



AResponse* methodPost(s_client* client, AResponse*){
	std::cout << "--> POST" << std::endl;

	if (client->request->getBodyCnt().length() == 0){
		std::cout << "--> Error: Empty body <--" << std::endl;
		return new BadResponse(405, "./root");
	}
	else{

		int ret = upload(client->request->getFilename(), client->request->getBodyCnt().c_str());
		if (ret == EXIT_FAILURE){
			std::cout << "--> Error: Cannot create file <--" << std::endl;
			return new BadResponse(500, "./root");
		}
		else
			return new GoodResponse("./root", "/uploadSuccess.html");
	}
}



AResponse*	methodDelete(s_client* client){
	std::cout << "--> DELETE" << std::endl;

	std::ifstream file("./root" + client->request->getPath());
	if (!file.is_open())
		return new BadResponse(404, "./root");
	file.close();

	int res = std::remove(("./root" + client->request->getPath()).c_str());
	if (res == -1)
		return new BadResponse(500, "./root");

	return new GoodResponse("./root", "/deleted.html");
}



AResponse*	methodPut(s_client* client){
	std::ofstream file("./root" + client->request->getPath());
	std::cout << client->request->getPath() << std::endl;
	if (!file.is_open())
		return new BadResponse(500, "./root");
	file << client->request->getBodyCnt();
	file.close();
	AResponse* res = new GoodResponse("./root", "");
	res->setStatus(201);
	return res;
}



int upload(const std::string & uplFileName, const char *data) {

	if (!data || !std::strlen(data) || !uplFileName.length()){
		std::cout << "--> Error: Empty File <--" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "upload to: " << uplFileName << std::endl;
	std::ofstream dstFile;
	dstFile.open(uplFileName.c_str(), std::ofstream::out);

	if (!dstFile.is_open())
		return EXIT_FAILURE;

	dstFile << std::string(data);
	dstFile.close();

	return EXIT_SUCCESS;
}
