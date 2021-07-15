//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../inc/output.hpp"
#include "Response.hpp"


int response(const int clientSocket, const std::string & request){
	int result;
	std::string uri;
	Response response;

	uri = request.rfind("/");
	//TODO: validate uri

	result = send(clientSocket,response.getResponse().c_str(), response.getResponse().length(), 0);
	return result;
}

