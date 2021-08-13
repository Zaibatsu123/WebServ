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
	if (getHead())
		return generateHeader();
//	return generateHeader() + generateBody();
	return generateBody();
}

std::string CgiResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << std::endl
		<< "Server: Equal-Rights/0.1.23" << std::endl
		<< "Content-Type: text/html" << std::endl
		<< "Content-Length: " << _calculateFileSize() << std::endl
		<< "Connection: keep-alive" << std::endl
		<< std::endl;
	return str.str();
}

std::string CgiResponse::generateBody() {
	std::string		buf;
	std::ifstream	file;

	file.open((_root + _fileName).c_str(), std::ifstream::in);

	file.seekg(0, std::ifstream::end);
	buf.reserve(file.tellg());
	file.seekg(0, std::ifstream::beg);

	buf.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	std::remove((_root + _fileName).c_str());
	std::remove("outputMy.txt");

	return buf;
}
