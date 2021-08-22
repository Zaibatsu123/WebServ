//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../../inc/output.hpp"
extern std::ofstream	g_logs;

ssize_t response(s_client *client){
	g_logs << "======================> Response part <====================== " << std::endl;
	AResponse* response;
	ssize_t result;

	if (client->responseNotSend)
		return sendall(client);
	client->request->getErr();

	t_location *location = get_location(client->request->getPath(), &(client->request->getServer()->locations));
	if (location == NULL){
		g_logs << "            ---->"  << " ---> Broken location <---" << std::endl;
		client->status = 0;
		return -1;
	}
	else if (client->request->getErr() != 0)
		response = new BadResponse(400, client->request->getServer()->error_pages[400]);
	else if (location->redirect.length()){
		g_logs << "            ---->" << "Redirected to " << location->redirect << std::endl;
		std::cout << location->redirect << std::endl;
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
	g_logs << response->generateHeader();

	client->responseBuffer = response->generateResponse();
	client->responseNotSend = true;

	result = sendall(client);

	delete response;
	delete client->request;
	client->request = NULL;
	g_logs << "======================> Response END <====================== " << std::endl;
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

//			std::map<std::string , std::string > tmp = client->request->getHeaders_();
//			if (tmp["Connection"] == "close")
//				return 0;

//			if (client->request->getHeaders_()["Connection"] == "close")
//				return 0;
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
