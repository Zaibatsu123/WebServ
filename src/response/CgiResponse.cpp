//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "CgiResponse.hpp"

CgiResponse::CgiResponse() : AResponse() {
}

CgiResponse::CgiResponse(const std::string &fileName) : AResponse("", fileName){
}

CgiResponse::CgiResponse(const CgiResponse &rhi){
	*this = rhi;
}

CgiResponse & CgiResponse::operator=(const CgiResponse &rhi){
	if (this == &rhi)
		return *this;
	AResponse::operator=(rhi);
	return *this;
}

CgiResponse::~CgiResponse(){
}

std::string CgiResponse::generateResponse() {
	return generateHeader() + generateBody();
}

std::string CgiResponse::generateHeader() {
	std::stringstream str;
	_fileSize = _calculateFileSize() - 58;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n"
		<< "Content-Type: text/html" << "\r\n"
		<< "Content-Length: " << _fileSize << "\r\n"
		<< "Connection: keep-alive" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string CgiResponse::generateBody() {
	std::string		buf;
	std::ifstream	file;

	file.open(CGI_OUTPUT, std::ifstream::in);
	buf.reserve(_fileSize);
	file.seekg(58);
	buf.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	std::remove(CGI_OUTPUT);

	return buf;
}
