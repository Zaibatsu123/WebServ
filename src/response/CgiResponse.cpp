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
	return generateHeader() + generateBody();
//	return generateBody();
}

std::string CgiResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n"
		<< "Content-Type: text/html" << "\r\n"
		<< "Content-Length: " << _calculateFileSize() - 58 << "\r\n"
		<< "X-Secret-Header-For-Test: 1" << "\r\n"
		<< "Connection: keep-alive" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string CgiResponse::generateBody() {
	std::string		buf;
	std::ifstream	file;

	file.open((_root + _fileName).c_str(), std::ifstream::in);
	buf.reserve(_calculateFileSize() - 58);
	file.seekg(58);
	buf.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

//	std::remove((_root + _fileName).c_str());
//	std::remove("outputMy.txt");

	return buf;
}
