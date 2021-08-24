//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "../../inc/output.hpp"
extern Logger logs;

AResponse* methodGet(s_client* client){
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);

	if ((location->methods & 4) >> 2 != 1)
		return new BadResponse(405, client->request->getServer()->error_pages[405]);

	if (client->request->getPath().find(".php") != std::string::npos){
		int status = cgi(CGI, location->root + client->request->getPath(), client);
		if (status)
			return new BadResponse(status, client->request->getServer()->error_pages[status]);
		return new CgiResponse(CGI_OUTPUT);
	}
	AResponse* resp = file_or_directory_existing(client);
	return resp;
}

AResponse* methodPost(s_client* client){
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);

	std::cout << "bodysize: " << location->max_body_size << std::endl;
	std::cout << (int)client->request->getBodyCnt().length() << std::endl;
	client->request->setBodySize(client->request->getBodyCnt().length());
	if (location->max_body_size && location->max_body_size < (int)client->request->getBodyCnt().length())
		return new BadResponse(413, client->request->getServer()->error_pages[413]);
	if ((location->methods & 2) >> 1 != 1)
		return new BadResponse(405, client->request->getServer()->error_pages[405]);

	std::string filename = getUploadFileName(client);

	if (upload(client) == EXIT_FAILURE)
		return new BadResponse(500, client->request->getServer()->error_pages[500]);

	if (filename.find(".bla") != std::string::npos){
		std::cout << "----> CGI" << std::endl;
		int status = cgi(CGI, filename, client);
		if (status)
			return new BadResponse(status, client->request->getServer()->error_pages[status]);
		return new CgiResponse(CGI_OUTPUT);
	}
	return new GoodResponse(location->root + "/uploadSuccess.html");
}

AResponse*	methodDelete(s_client* client){
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);

	if ((location->methods & 1) != 1)
		return new BadResponse(405, client->request->getServer()->error_pages[405]);

	if (std::remove((location->root + client->request->getPath()).c_str()) == -1){
		logs.addMessage("Missing file\n");
		return new BadResponse(404, client->request->getServer()->error_pages[404]);
	}
	logs.addMessage("Deleted\n");
	return new GoodResponse(location->root + "/deleted.html");
}

AResponse*	methodPut(s_client* client){
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);

	if ((location->methods & 8) >> 3 != 1)
		return new BadResponse(405, client->request->getServer()->error_pages[405]);

	if (upload(client))
		return new BadResponse(500, client->request->getServer()->error_pages[500]);
	return new PutResponse(201);
}
