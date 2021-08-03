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

	std::cout << "client check fail <--" << std::endl;
	client->request->getMethod();
	std::cout << "client check work -->" << std::endl;

	if (client->request->getErr() != 0){
		response = new BadResponse(400, "./root");
	}
	else{
		if (client->request->getMethod() == "GET"){
			response = methodGet(client);
		}

		if (client->request->getMethod() == "POST")
			response = methodPost(client);

		if (client->request->getMethod() == "HEAD"){
			response = new BadResponse(405, "./root");
		}

		if (client->request->getMethod() == "DELETE")
			response = methodDelete(client);

		if (client->request->getMethod() == "PUT")
			response = methodPut(client);
	}
	std::cout << response->generateHeader() << std::endl;
	std::string buffer = response->generateResponse();

	ssize_t result = sendall(client->socket, buffer, MSG_NOSIGNAL);

	delete response;
	delete client->request;
	std::cout << "--------------------> Response END <------------ " << std::endl;
	return result;
}

