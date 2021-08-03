//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "../../inc/output.hpp"

AResponse* methodGet(s_client* client, AResponse* response){
	std::cout << "--> GET" << std::endl;

	int res = file_or_directory_existing(client, response);
	//!! delete after debug
//	res = 0;
	std::cout << "res checking" << res << std::endl;

	if (res == 2){
		AResponse* response1 = new AutoIndexResponse();
//		std::string buffer = response->generateResponse(1);
//		sendall(client->socket, buffer, MSG_NOSIGNAL);
		return response1;
	}
	else if (res == 404){
		return new BadResponse(404,0,"./root");
	}

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
	return new GoodResponse(0,"./root", client->request->getPath());
}



AResponse* methodPost(s_client* client, AResponse*){
	std::cout << "--> POST" << std::endl;

	if (client->request->getBodyCnt().length() == 0){
		std::cout << "--> Error: Empty body <--" << std::endl;
		return new BadResponse(405, 0, "./root");
	}
	else{
		int ret = upload(client->request->getFilename(), client->request->getBodyCnt().c_str());
		if (ret == EXIT_FAILURE){
			std::cout << "--> Error: Cannot create file <--" << std::endl;
			return new BadResponse(500, 0, "./root");
		}
		else
			return new GoodResponse(0, "./root", "/uploadSuccess.html");
	}
}



AResponse*	methodDelete(AResponse* ){
//	std::remove((response->getRoot() + response->getFileName()).c_str());
	return new BadResponse(501, 0, "./root");
}



AResponse*	methodPut(AResponse* ){
	return new BadResponse(501, 0, "./root");
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
