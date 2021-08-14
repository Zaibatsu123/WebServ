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

	if (location->methods != 4 && location->methods != 6)
		return new BadResponse(405, location->root);

	if (client->request->getPath().find(".php") != std::string::npos){
		std::cout << "----> CGI" << std::endl;

		int status = cgi(CGI, location->root + client->request->getPath(), client);

		if (status)
			return new BadResponse(status, location->root);

		return new CgiResponse("outputCGI.txt");
	}
	AResponse* resp = file_or_directory_existing(client);

	return resp;
}


AResponse* methodPost(s_client* client){
	std::cout << "--> POST" << std::endl;
	int status;
	t_location *location = get_location(client->request->getPath(), &client->server->locations);
	std::cout << "LOCATION:|" << location->root << "|" << "PATH:|" << client->request->getPath() << "|"<< std::endl;
	std::cout << "FilENAME:|" << client->request->getPath() << "|"<< std::endl;


	if (location->max_body_size && location->max_body_size < client->request->getBodyCnt().length())
		return new BadResponse(413, location->root);
	if (location->methods != 2 && location->methods != 6)
		return new BadResponse(405, location->root);

	status = upload(location->root + client->request->getPath(), client);
	if (status){
		std::cout << "--> Error: Cannot create file <--" << std::endl;
		return new BadResponse(status, location->root);
	}

	if (client->request->getPath().find(".bla") != std::string::npos){
		std::cout << "----> CGI" << std::endl;


		status = cgi(CGI, location->root + client->request->getPath(), client);

		if (status)
			return new BadResponse(status, location->root);
		return new CgiResponse("outputCGI.txt");
//		return new GoodResponse("", "");
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
	// std::cout << client->request->getBodyCnt() << std::endl;

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


int upload(const std::string & uplFileName, s_client* client) {

	// if (!client->request->getBodyCnt() || !std::strlen(data) || !uplFileName.length()){
	// 	std::cout << "--> Error: Empty File <--" << std::endl;
	// 	return EXIT_FAILURE;
	// }

	std::cout << "upload to: " << uplFileName << std::endl;
	std::ofstream dstFile;
	dstFile.open(uplFileName.c_str(), std::ofstream::out);

	if (!dstFile.is_open())
		return 500;

	dstFile << std::string(client->request->getBodyCnt());
	dstFile.close();

	return 0;
}


