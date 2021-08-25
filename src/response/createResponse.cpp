//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../../inc/output.hpp"
extern Logger logs;

ssize_t response(s_client *client){
	AResponse* response;
	ssize_t result;

	if (client->responseNotSend)
		return sendall(client);

	t_location *location = get_location(client->request->getPath(), &(client->request->getServer()->locations));

	if (client->request->getErr())
		response = new BadResponse(client->request->getErr(), client->request->getServer()->error_pages[client->request->getErr()]);
	else if (location == NULL){
		response = new BadResponse(400, client->request->getServer()->error_pages[400]);
	}
	else if (location->redirect.length()){
		response = new RedirectResponse(301, location->redirect);
	}
	else{
		if (client->request->getMethod() == "GET")
			response = methodGet(client);

		if (client->request->getMethod() == "POST")
			response = methodPost(client);

		if (client->request->getMethod() == "HEAD")
			response = new BadResponse(405, client->request->getServer()->error_pages[405]);

		if (client->request->getMethod() == "DELETE")
			response = methodDelete(client);

		if (client->request->getMethod() == "PUT")
			response = methodPut(client);
	}
	logs << "[" << logs.timeStamp().c_str() << "] "
		 << client->request->getMethod().c_str() << " " << client->request->getPath().c_str()
		 << " status " << response->getStatus() << "\n";

	client->responseBuffer = response->generateResponse();
	client->responseNotSend = true;
	result = sendall(client);

	delete response;
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

		if (result == static_cast<ssize_t>(client->responseBuffer.length())){
			client->responseBuffer.clear();
			client->responseNotSend = false;
			client->status = 0;

			std::map<std::string, std::string> tmp = client->request->getHeaders_();
			std::map<std::string, std::string>::iterator it = tmp.find("connection");
			std::map<std::string, std::string>::iterator ite = tmp.end();

			delete client->request;
			client->request = NULL;

			if (it != ite && it->second == "close")
				return 0;

			return result;
		}
		try{
			client->responseBuffer = client->responseBuffer.substr(result);
		}
		catch (std::exception & e){
		}
	}
	return result;
}
