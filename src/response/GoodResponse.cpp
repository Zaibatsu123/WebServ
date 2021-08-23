//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "GoodResponse.hpp"

GoodResponse::GoodResponse() : AResponse() {
}

GoodResponse::GoodResponse(const std::string &fileName) : AResponse("", fileName){
}

GoodResponse::GoodResponse(const GoodResponse &rhi){
	*this = rhi;
}

GoodResponse & GoodResponse::operator=(const GoodResponse &rhi){
	if (this == &rhi)
		return *this;
	AResponse::operator=(rhi);
	return *this;
}

GoodResponse::~GoodResponse(){
}

std::string GoodResponse::generateResponse() {
	return generateHeader() + generateBody();
}

std::string GoodResponse::generateHeader() {
	std::stringstream str;
	_fileSize = _calculateFileSize();
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n"
		<< "Content-Type: " << _indicateFileType() << "\r\n"
		<< "Content-Length: " << _fileSize << "\r\n"
		<< "Connection: keep-alive" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string GoodResponse::generateBody() {
	std::string		buf;
	std::ifstream	file;

	std::cout << "FileName: " << _fileName << std::endl;
	file.open(_fileName, std::ifstream::in);
	if (!file.is_open())
		return "";

	buf.reserve(_fileSize);
	buf.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	return buf;
}
