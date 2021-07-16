//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "Response.hpp"

const std::string Response::_protocol = "HTTP/1.1";

std::map<int, std::string> Response::_code = Response::_createMap();

std::map<int, std::string> Response::_createMap() {
	std::map<int, std::string> m;
	m.insert(std::pair<int, std::string>(200, "OK"));
	m.insert(std::pair<int, std::string>(404, "not found"));
	return m;
}

Response::Response() : _status(0){
}

Response::~Response(){
}

void Response::display(const std::string & fileName) const{
	std::cout	<< "\e[37m" << "---\n"
//				<< this->generateHeader()
//				<< this->generateBody(fileName)
				<< "---\e[0m\n" << std::endl;
	(void )fileName;
}

//void Response::expandResponse(const std::string &string){
//	std::stringstream str;
//	str << _responseBody;
//	str << string;
//	_responseBody = str.str();
//}

//void Response::addMapField(const std::string &key, const std::string &value){
//	_c.insert(std::pair<std::string, std::string>(key, value));
//}

std::string Response::generateResponse(const std::string &fileName) const {
	int 			status;
	std::ifstream	srcFile;

	srcFile.open(fileName.c_str(), std::ifstream::in);
	if (!srcFile.is_open())
		status = 404;
	else {
		srcFile.close();
		status = 200;
	}
	return this->generateHeader(status) + this->generateBody(status, fileName);
}

std::string Response::generateHeader(int status) const {
	std::stringstream str;

	str << _protocol << " "
		<< status << " "
		<< _code[status] << "\n"
		<< "Connection: close\n"
		<< "\n";
//	str << "Content-Type: text/html; charset=UTF-8\n";
//	str << "Content-Length: 21\n";
	return str.str();
}

std::string Response::generateBody(int status, const std::string & fileName) const {
	std::string			buffer;
	std::ifstream		file;
	std::stringstream	str;

	if (status == 404)
		file.open("404.html", std::ifstream::in);
	else
		file.open(fileName.c_str(), std::ifstream::in);
	while (file.good()) {
		std::getline(file, buffer);
		str << buffer;
		str << "\n";
	}
	return str.str();
}

//void Response::setResponse(const std::string &response){
//	_responseBody = response;
//}
//
//const std::string &Response::getResponse() const{
//	return _responseBody;
//}

void Response::setStatus(int n) {
	_status = n;
}

int Response::getStatus() const {
	return _status;
}
