//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "BadResponse.hpp"

BadResponse::BadResponse() : AResponse() {
}

BadResponse::BadResponse(int status, const std::string &errorPagePath) : AResponse("", errorPagePath){
	_status = status;
}

BadResponse::BadResponse(const BadResponse &rhi){
	*this = rhi;
}

BadResponse & BadResponse::operator=(const BadResponse &rhi){
	if (this == &rhi)
		return *this;
	AResponse::operator=(rhi);
	return *this;
}

BadResponse::~BadResponse(){
}

std::string BadResponse::generateResponse() {
	return generateHeader() + generateBody();
}

std::string BadResponse::generateHeader() {
	std::stringstream str;
	_fileSize = _calculateFileSize();
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n"
		<< "Content-Type: " << _indicateFileType() << "\r\n"
		<< "Content-Length: " << _fileSize << "\r\n"
		<< "Connection: close" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string BadResponse::generateBody() {
	std::string		buf;
	std::ifstream	file;

	file.open(_fileName, std::ifstream::in);

	buf.reserve(_fileSize);
	buf.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	return buf;
}
