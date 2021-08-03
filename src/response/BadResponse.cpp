//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "BadResponse.hpp"

BadResponse::BadResponse() : AResponse() {
}

BadResponse::BadResponse(int status, long long maxContent, const std::string &root) : AResponse(maxContent, root,""){
	_status = status;
}

BadResponse::~BadResponse(){
}

std::string BadResponse::generateResponse() {
	if (getHead())
		return generateHeader();
	return generateHeader() + generateBody();
}

std::string BadResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << std::endl
		<< "Server: Equal-Rights/0.1.23" << std::endl
		<< "Date: " << _dateTime()
		<< "Content-Type: " << _indicateFileType() << std::endl
		<< "Content-Length: " << _calculateFileSize() << std::endl
//		TODO: need alive or close?
//		<< "Connection: keep-alive" << std::endl
		<< "Connection: close" << std::endl
		<< std::endl;
	return str.str();
}

std::string BadResponse::generateResponse(int) {
	if (getHead())
		return generateHeader();
	return generateHeader() + generateBody();
}

std::string BadResponse::generateHeader(int) {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << std::endl
		<< "Server: Equal-Rights/0.1.23" << std::endl
		<< "Date: " << _dateTime()
		<< "Content-Type: " << _indicateFileType() << std::endl
		<< "Content-Length: " << _calculateFileSize() << std::endl
		<< "Connection: close" << std::endl
		<< std::endl;
	return str.str();
}

std::string BadResponse::generateBody() {
	std::string			buffer;
	std::ifstream		file;
	std::stringstream	str;

	file.open(_errorPage[_status], std::ifstream::in);

	while (file.good()) {
		std::getline(file, buffer);
		str << buffer;
		if (file.good())
			str << "\n";
	}
	file.close();
	return str.str();
}
