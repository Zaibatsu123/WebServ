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


ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;
	AResponse* response;
	if (client->request->getErr() != 0){
		response = new BadResponse(400, 0,"./root");
	}
	else{
		response = new GoodResponse(0,"./root", client->request->getPath());
		if (client->request->getMethod() == "GET"){
			response = methodGet(client, response);
		}

		if (client->request->getMethod() == "POST")
			response = methodPost(client, response);

		if (client->request->getMethod() == "HEAD"){
			response = methodGet(client, response);
			response->setHead(1);
		}

		if (client->request->getMethod() == "DELETE")
			response = methodDelete(response);

		if (client->request->getMethod() == "PUT")
			response = methodPut(response);
	}
	std::cout << response->generateHeader(0) << std::endl;
	std::string buffer = response->generateResponse(0);

	ssize_t result = sendall(client->socket, buffer, MSG_NOSIGNAL);

	delete response;
	delete client->request;
	std::cout << "--------------------> Response END <------------ " << std::endl;
	return result;
}

