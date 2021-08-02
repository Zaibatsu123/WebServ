//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "GoodResponse.hpp"

GoodResponse::GoodResponse() : AResponse() {
}

GoodResponse::GoodResponse(long long maxContent, const std::string &root, const std::string &fileName) : AResponse(maxContent, root,fileName){
}

GoodResponse::~GoodResponse(){
}

std::string GoodResponse::generateResponse() {
	return "";
}

std::string GoodResponse::generateHeader() {
	return "";
}

std::string GoodResponse::generateResponse(int res) {
	if (res > 0){
		if (getHead())
			return generateHeader(this->getBody().size());
		return generateHeader(this->getBody().size()) + this->getBody().c_str();
	}
	if (getHead())
		return generateHeader();
	return generateHeader(0) + generateBody();
}

std::string GoodResponse::generateHeader(int status) {
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

std::string GoodResponse::generateBody() {
	std::string			buffer;
	std::ifstream		file;
	std::stringstream	str;

	if (_status != 200){
		std::cout << "STATUS: " << _status << std::endl;
		std::cout << _errorPage[_status] << std::endl;
		file.open(_errorPage[_status], std::ifstream::in);
	}
	else
		file.open((_root + _fileName).c_str(), std::ifstream::in);

	while (file.good()) {
		std::getline(file, buffer);
		str << buffer;
		if (file.good())
			str << "\n";
	}
	file.close();
	return str.str();
}
