//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../../inc/output.hpp"

ssize_t sendall(int socket, std::string & buffer, int flags){
	ssize_t result;
	int it = 1;
	do {
		if ((result = send(socket, buffer.c_str(),buffer.length(), flags)) == -1)
			std::cout << "SEND ERROR: " << result << strerror(errno) << std::endl;
		if (static_cast<int>(result) == -1)
			perror("");
		std::cout
				<< "---- Pack: " << it++ << "\n"
				<< "Data Left:\t" << buffer.length() << "\n"
				<< "Send Result:\t" << result
				<< std::endl;

		//TODO: delete sleep for delay after send
		usleep(1000);

		if (result == static_cast<ssize_t>(buffer.length()))
			break;
		try{
			buffer = buffer.substr(result);
		}
		catch (std::exception & e){
			std::cout << e.what() << std::endl;
		}
	} while (result);
	return result;
}

ssize_t sendall2(s_client *client){
	ssize_t result;

	result = send(client->socket, client->responseBuffer.c_str(), client->responseBuffer.length(), MSG_NOSIGNAL);

	std::cout << "---- Pack:" << std::endl;
	std::cout << "Data Left:\t" << std::setw(9) << client->responseBuffer.length() << std::endl;
	std::cout << "Send Result:\t" << std::setw(9) << result << std::endl;
	if (result == static_cast<ssize_t>(client->responseBuffer.length())) {
		std::cout << "All data send. Clear buffer." << std::endl;
		client->responseBuffer.clear();
		client->status = 0;
		client->buffer.clear();
	}
	else if (result > 0 && result < static_cast<ssize_t>(client->responseBuffer.length())){
		std::cout << "Not all data send. Save buffer." << std::endl;
		client->responseBuffer = client->responseBuffer.substr(result);
		client->status = 1;
	}
	else if (result == 0){
		std::cout << "I have no data to send. Clear buffer." << std::endl;
		client->responseBuffer.clear();
		client->status = 0;
		client->buffer.clear();
	}
	else if (result < 0){
		std::perror("Send: ");
	}
	return result;
}

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;
	AResponse* response;
	ssize_t result;

	std::cout << "Data size to send:\t" << client->responseBuffer.length() << std::endl;
	if (!client->responseBuffer.empty()){
		std::cout << "--> Response have data to send" << std::endl;
		usleep(1000);
		return sendall2(client);
	}

	std::cout << "client check crushed ⛔️ <--" << std::endl;
	client->request->getMethod();
	client->request->getErr();
	t_location *location = get_location(client->request->getPath(), &client->server->locations);
	std::cout << "client check restored ✅ -->" << std::endl;

	if (client->request->getErr() != 0){
		response = new BadResponse(400, location->root);
	}
	else{
		if (client->request->getMethod() == "GET"){
			response = methodGet(client);
		}

		if (client->request->getMethod() == "POST")
			response = methodPost(client);

		if (client->request->getMethod() == "HEAD"){
//			response->setHead(1);
			response = new BadResponse(405, location->root);
		}

		if (client->request->getMethod() == "DELETE")
			response = methodDelete(client);

		if (client->request->getMethod() == "PUT"){
			response = methodPut(client);
//			response = new BadResponse(201, location->root);
		}
	}

	std::cout << response->generateHeader() << std::endl;
	client->responseBuffer = response->generateResponse();


	result = sendall2(client);
//	result = sendall(client->socket, buffer, MSG_NOSIGNAL);

	delete response;
	delete client->request;
	std::cout << "--------------------> Response END <------------ " << std::endl;
	return result;
}

