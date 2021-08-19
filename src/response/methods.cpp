//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "../../inc/output.hpp"

AResponse* methodGet(s_client* client){
	std::cout << "--> GET" << std::endl;
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);

	// std::cout << "WHAT?" << location->methods << std::endl;
	if (location->methods != 4 && location->methods != 6)
		return new BadResponse(405, client->request->getServer()->error_pages[405]);
	if (client->request->getPath().find(".php") != std::string::npos){
		std::cout << "----> CGI" << std::endl;
		int status = cgi(CGI, location->root + client->request->getPath(), client);
		if (status)
			return new BadResponse(status, client->request->getServer()->error_pages[status]);
		return new CgiResponse("outputCGI.txt");
	}
	AResponse* resp = file_or_directory_existing(client);
	return resp;
}

AResponse* methodPost(s_client* client){
	std::cout << "--> POST" << std::endl;
	int status;
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);
	std::cout << "LOCATION:|" << location->root << "|" << "PATH:|" << client->request->getPath() << "|"<< std::endl;
	std::cout << "FilENAME:|" << client->request->getPath() << "|"<< std::endl;

	if (location->max_body_size && location->max_body_size < client->request->getBodyCnt().length())
		return new BadResponse(413, client->request->getServer()->error_pages[413]);
	if (location->methods != 2 && location->methods != 6)
		return new BadResponse(405, client->request->getServer()->error_pages[405]);

	status = upload(location->root + client->request->getPath(), client);
	if (status){
		std::cout << "--> Error: Cannot create file <--" << std::endl;
		return new BadResponse(status, client->request->getServer()->error_pages[status]);
	}
	if (client->request->getPath().find(".bla") != std::string::npos){
		std::cout << "----> CGI" << std::endl;
		status = cgi(CGI, location->root + client->request->getPath(), client);
		if (status)
			return new BadResponse(status, client->request->getServer()->error_pages[status]);
		return new CgiResponse("outputCGI.txt");
	}
	return new GoodResponse(location->root, "/uploadSuccess.html");
}

AResponse*	methodDelete(s_client* client){
	std::cout << "--> DELETE" << std::endl;
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);

	std::ifstream file(location->root + client->request->getPath());
	if (!file.is_open())
		return new BadResponse(404, client->request->getServer()->error_pages[404]);
	file.close();
	int res = std::remove((location->root + client->request->getPath()).c_str());
	if (res == -1)
		return new BadResponse(500, client->request->getServer()->error_pages[500]);
	return new GoodResponse(location->root, "/deleted.html");
}

AResponse*	methodPut(s_client* client){
	std::cout << "---> PUT" << std::endl;
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);

	std::cout << "PUT FileName" << std::endl;
	std::cout << location->root + client->request->getPath() << std::endl;
	std::cout << "PUT Data" << std::endl;

	std::ofstream file(location->root + client->request->getPath());
	if (!file.is_open())
		return new BadResponse(500, client->request->getServer()->error_pages[500]);
	file << client->request->getBodyCnt();
	file.close();

	AResponse* res = new GoodResponse(location->root, "");
	res->setStatus(201);
	return res;
}


int upload(const std::string & uplFileName, s_client* client) {
	std::cout << "upload to: " << uplFileName << std::endl;
	std::ofstream dstFile;
	dstFile.open(uplFileName.c_str(), std::ofstream::out);
	if (!dstFile.is_open())
		return 500;
	dstFile << std::string(client->request->getBodyCnt());
	dstFile.close();
	return 0;
}
