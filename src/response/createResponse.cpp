//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../../inc/output.hpp"

ssize_t response(s_client *client, std::ofstream *logs){
	*logs << "======================> Response part <====================== " << std::endl;
	AResponse* response;
	ssize_t result;

	if (client->responseNotSend){
		return sendall(client);
	}

	client->request->getErr();
	t_location *location = get_location(client->request->getPath(), &client->server->locations);

	if (location == NULL){
		*logs << "            ---->"  << " ---> Broken location <---" << std::endl;
		client->status = 0;
		return -1;
	}

	if (client->request->getErr() != 0){
		*logs << "            ---->"   << "Request error" << std::endl;
		client->responseBuffer = "la la la";
		std::string a = location->root;
		*logs << "            ---->"  << "Request error 2" << std::endl;
		response = new BadResponse(400, location->root);
		*logs << "            ---->"   << "Request error 3" << std::endl;
	}
	else{
		if (client->request->getMethod() == "GET")
			response = methodGet(client);

		if (client->request->getMethod() == "POST")
			response = methodPost(client);

		if (client->request->getMethod() == "HEAD")
			response = new BadResponse(405, location->root);

		if (client->request->getMethod() == "DELETE")
			response = methodDelete(client);

		if (client->request->getMethod() == "PUT")
			response = methodPut(client);
	}

	*logs << response->generateHeader() << std::endl;
	*logs << response->generateBody() << std::endl;

	client->responseBuffer = response->generateResponse();
	client->responseNotSend = true;

	client->buffer.clear();
	result = sendall(client);

	delete response;
	delete client->request;
	client->request = NULL;
	*logs << "======================> Response END <====================== " << std::endl;
	return result;
}

ssize_t sendall(s_client* client){
	ssize_t result = 0;

	if (client->responseNotSend){
		result = send(client->socket, client->responseBuffer.c_str(), client->responseBuffer.length(), MSG_NOSIGNAL);

		if (static_cast<int>(result) == -1 || static_cast<int>(result) == 0){
			perror("");
			return result;
		}
		std::cout
		<< "---- Pack: " << "\n"
		<< "Data Left:\t" << client->responseBuffer.length() << "\n"
		<< "Send Result:\t" << result
		<< std::endl;

		if (result == static_cast<ssize_t>(client->responseBuffer.length())){
			std::cout << "All data was send. Clear buffer." << std::endl;
			client->responseBuffer.clear();
			client->responseNotSend = false;
			client->status = 0;
			std::map<std::string , std::string > tmp = client->request->getHeaders_();

			if (tmp["Connection"] == "close")
				return 0;
			return result;
		}
		try{
			client->responseBuffer = client->responseBuffer.substr(result);
		}
		catch (std::exception & e){
			std::cout << e.what() << std::endl;
		}
	}
	return result;
}
