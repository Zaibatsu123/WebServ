//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "CgiResponse.hpp"

CgiResponse::CgiResponse() : AResponse() {
}

CgiResponse::CgiResponse(long long maxContent, const std::string &root, const std::string &fileName) : AResponse(maxContent, root,fileName){
}

CgiResponse::~CgiResponse(){
}

std::string CgiResponse::generateResponse() {
	return "";
}

std::string CgiResponse::generateHeader() {
	return "";
}

std::string CgiResponse::generateResponse(int) {
	if (getHead())
		return generateHeader();
	return generateHeader(0) + generateBody();
}

std::string CgiResponse::generateHeader(int) {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << std::endl
		<< "Server: Equal-Rights/0.1.23" << std::endl
		<< "Date: " << _dateTime()
		<< "Content-Type: text/html" << std::endl
		<< "Content-Length: " << _calculateFileSize() << std::endl
		<< "Connection: keep-alive" << std::endl
		<< std::endl;
	return str.str();
}

std::string CgiResponse::generateBody() {
	std::string			buffer;
	std::ifstream		file;
	std::stringstream	str;

	file.open((_root + _fileName).c_str(), std::ifstream::in);

	while (file.good()) {
		std::getline(file, buffer);
		str << buffer;
		if (file.good())
			str << "\n";
	}
	file.close();
	std::remove((_root + _fileName).c_str());
	std::remove("outputMy.txt");
	return str.str();
}
