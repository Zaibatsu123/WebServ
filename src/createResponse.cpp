//                             //
// Created by                  //
//            Zaibatsu         //
// on                          //
//            14.07.2021       //
//                             //

#include "../inc/output.hpp"
#include "Response.hpp"
#include <fstream>
#include <sstream>

int response(const int clientSocket, const std::string & request){
	int			result = 0;
	std::string	fileName;
	std::string	buffer;
	std::string method = "post";
	std::string dstFileName = "test.txt";
	std::string fileData = "test_text";
	Response* 	response = new Response;
	std::string	root = "./root/";
	//TODO: getfromparce

	std::cout << request << std::endl;
	if (method == "get"){
		if (request.find("bg.jpg") != std::string::npos)
			fileName = root + "bg.jpg";
		else if (request.find("style.css") != std::string::npos)
			fileName = root + "style.css";
		else if (request.find("upload.html") != std::string::npos)
			fileName = root + "upload.html";
		else
			fileName = root + "index.html";
		//TODO: validate ifstream
	//	response->display(fileName);
		std::cout << "---\n" << response->generateHeader(200) << "---\n";
		buffer = response->generateResponse(fileName);
	}
	if (method == "post"){
		fileName = root + "uploadSuccess.html";
		response->setUplRoot("./root/tmp/");
		buffer = response->upload(dstFileName, fileData.c_str(), fileName);
	}
	result = send(clientSocket, buffer.c_str(), buffer.length(), 0);
	delete response;
	return result;
}

