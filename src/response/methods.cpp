//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "../../inc/output.hpp"

AResponse* methodGet(s_client* client){
	std::cout << "--> GET" << std::endl;
	t_location *location = get_location(client->request->getPath(), &client->server->locations);

	if (client->request->getPath().find(".php") != std::string::npos){
		std::cout << "----> CGI" << std::endl;

		int status = cgi(CGI, location->root + client->request->getPath());

		if (status)
			return new BadResponse(status, location->root);

		return new CgiResponse("outputCGI.txt");
	}

	AResponse* resp = file_or_directory_existing(client);

	return resp;
}



AResponse* methodPost(s_client* client){
	std::cout << "--> POST" << std::endl;
	t_location *location = get_location(client->request->getPath(), &client->server->locations);

	if (client->request->getBodyCnt().length() == 0){
		std::cout << "--> POST: Error: client sent invalid chunked body" << std::endl;
		return new BadResponse(405, location->root);
	}

	int status = upload(client->request->getFilename(), client->request->getBodyCnt().c_str());

	if (status){
		std::cout << "--> Error: Cannot create file <--" << std::endl;
		return new BadResponse(status, location->root);
	}
	return new GoodResponse(location->root, "/uploadSuccess.html");
}



AResponse*	methodDelete(s_client* client){
	std::cout << "--> DELETE" << std::endl;
	t_location *location = get_location(client->request->getPath(), &client->server->locations);

	std::ifstream file(location->root + client->request->getPath());
	if (!file.is_open())
		return new BadResponse(404, location->root);
	file.close();

	int res = std::remove((location->root + client->request->getPath()).c_str());
	if (res == -1)
		return new BadResponse(500, location->root);

	return new GoodResponse(location->root, "/deleted.html");
}



AResponse*	methodPut(s_client* client){
	std::cout << "---> PUT" << std::endl;
	t_location *location = get_location(client->request->getPath(), &client->server->locations);

	std::cout << "PUT FileName" << std::endl;
	std::cout << location->root + client->request->getPath() << std::endl;
	std::cout << "PUT Data" << std::endl;
	std::cout << client->request->getBodyCnt() << std::endl;

//	if (client->request->getBodyCnt().length() == 0){
//		std::cout << "--> PUT: Error: client sent invalid chunked body" << std::endl;
//		return new BadResponse(405, location->root);
//	}

	std::ofstream file(location->root + client->request->getPath());
	if (!file.is_open())
		return new BadResponse(500, location->root);
	file << client->request->getBodyCnt();
	file.close();

	AResponse* res = new GoodResponse(location->root, "");
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
		return 500;

	dstFile << std::string(data);
	dstFile.close();

	return 0;
}
