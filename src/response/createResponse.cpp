//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../../inc/output.hpp"

ssize_t response(s_client *client){
	std::cout << "--------------------> Response part <------------ " << std::endl;
	AResponse* response;
	response = new GoodResponse(0,"./root", client->request->getPath());
	if (client->request->getErr() != 0){
		delete response;
		response = new BadResponse(0,"./root", client->request->getPath());
		response->setStatus(400);
	}
	else{
		if (client->request->getMethod() == "GET")
			methodGet(client, response);

		if (client->request->getMethod() == "POST")
			methodPost(client, response);

		if (client->request->getMethod() == "DELETE")
			methodDelete(response);
	}
	std::cout << response->generateHeader(0) << std::endl;
	std::string buffer = response->generateResponse(0);

	ssize_t result = sendall(client->socket, buffer, MSG_NOSIGNAL);

	delete response;
	delete client->request;
	std::cout << "--------------------> Response END <------------ " << std::endl;
	return result;
}

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

//bool requestFileValidator(AResponse * response){
//	std::ifstream	srcFile;
//	srcFile.open((response->getRoot() + response->getFileName()).c_str(), std::ifstream::in);
//
//	//TODO: delete when evaluate
//	std::cout << response->getRoot() + response->getFileName() << std::endl;
//
//	if (!srcFile.is_open()){
//		response->setStatus(404);
//		return false;
//	}
//	srcFile.close();
//	return true;
//}
//
//bool requestContentSizeValidator(AResponse *response){
//	std::ifstream	srcFile;
//	srcFile.open((response->getRoot() + response->getFileName()).c_str(), std::ifstream::in);
//
//	long long size;
//
//	srcFile.seekg (0, srcFile.end);
//	size = srcFile.tellg();
//	srcFile.seekg (0, srcFile.beg);
//	srcFile.close();
//
//	if (response->getMaxContent() && size > response->getMaxContent()){
//		response->setStatus(413);
//		return false;
//	}
//	return true;
//}




