//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "output.hpp"

std::string getUploadFileName(s_client* client){
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);
	std::string filename;

	std::cout << "Path: " << client->request->getPath() << std::endl;
	std::cout << "location: " << location->location << std::endl;
	if (client->request->getPath().find(location->location) == std::string::npos || client->request->getPath() == location->location)
		return "";

	if (client->request->getTransferCode() == "chunked")
		filename = client->request->getServer()->upload_file_to + client->request->getPath().substr(client->request->getPath().find(location->location) + location->location.length() + 1);
	else
		filename = client->request->getServer()->upload_file_to + client->request->getFilename();
	return filename;
}

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
	int status = 0;
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);
	std::cout << "LOCATION:|" << location->root << "|" << "PATH:|" << client->request->getPath() << "|"<< std::endl;
	std::cout << "FilENAME:|" << client->request->getPath() << "|"<< std::endl;

	if (location->max_body_size && location->max_body_size < (int)client->request->getBodyCnt().length())
		return new BadResponse(413, client->request->getServer()->error_pages[413]);
	if (location->methods != 2 && location->methods != 6)
		return new BadResponse(405, client->request->getServer()->error_pages[405]);

	std::string filename = getUploadFileName(client);
	if (!filename.empty())
		status = upload(filename, client);
	if (status){
		std::cout << "--> Error: Cannot create file <--" << std::endl;
		return new BadResponse(status, client->request->getServer()->error_pages[status]);
	}
	if (filename.find(".bla") != std::string::npos){
		std::cout << "----> CGI" << std::endl;
		status = cgi(CGI, filename, client);
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

	std::string filename;
	if (client->request->getTransferCode() == "chunked")
		filename = client->request->getServer()->upload_file_to + client->request->getPath().substr(client->request->getPath().find(location->location) + location->location.length() + 1);
	else
		filename = client->request->getServer()->upload_file_to + client->request->getFilename();

	int status = upload(filename, client);
	if (status)
		return new BadResponse(status, client->request->getServer()->error_pages[status]);

	return new PutResponse(201);
}


int upload(const std::string & filename, s_client* client) {
	std::cout << "upload to :" << filename << std::endl;
	std::ofstream dstFile;
	dstFile.open((filename).c_str(), std::ofstream::out);
	if (!dstFile.is_open())
		return 500;
	dstFile << std::string(client->request->getBodyCnt());
	dstFile.close();
	return 0;
}
