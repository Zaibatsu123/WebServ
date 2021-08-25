//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/21/21				 //
//                                   //

#include "../../inc/output.hpp"

std::string getUploadFileName(s_client* client){
	t_location *location = get_location(client->request->getPath(), &client->request->getServer()->locations);
	std::string filename;

	if (!client->request->getFilename().empty()){
		return client->request->getServer()->upload_file_to + client->request->getFilename();
	}

	if (client->request->getTransferCode() == "chunked"){
		if (client->request->getPath() == location->location)
			return "";
		return location->root + client->request->getPath().substr(client->request->getPath().find(location->location) + location->location.length());
	}
	return "";
}

int upload(s_client* client) {
	std::string filename = getUploadFileName(client);
	if (filename.empty())
		return 0;

	std::ofstream dstFile;
	dstFile.open(filename, std::ofstream::out);
	if (!dstFile.is_open()){
		return 1;
	}
	dstFile << std::string(client->request->getBodyCnt());
	dstFile.close();
	return 0;
}