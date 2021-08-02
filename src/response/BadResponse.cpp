//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "BadResponse.hpp"

BadResponse::BadResponse() : AResponse() {
}

BadResponse::BadResponse(long long maxContent, const std::string &root, const std::string &fileName) : AResponse(maxContent, root,fileName){
}

BadResponse::~BadResponse(){
}

std::string BadResponse::generateResponse(int res) {
	if (res > 0)
		return generateHeader(this->getBody().size()) + this->getBody().c_str();
	return generateHeader(0) + generateBody();
}

std::string BadResponse::generateHeader(int status) {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << std::endl
		<< "Server: Equal-Rights/0.1.23" << std::endl
		<< "Date: " << _dateTime();

	str << "Content-Type: " << _indicateFileType() << std::endl;

	if (status > 0)
		str << "Content-Length: "<< status << std::endl;
	else
		str << "Content-Length: " << _calculateFileSize() << std::endl;

	if (_status == 200)
		str << "Connection: keep-alive" << std::endl;
	else
		str << "Connection: close" << std::endl;

	str << std::endl;
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
