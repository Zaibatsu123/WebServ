//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/3/21				 //
//                                   //

#include "PutResponse.hpp"

PutResponse::PutResponse() : AResponse() {
}

PutResponse::PutResponse(int status) : AResponse("", ""){
	_status = status;
}

PutResponse::PutResponse(const PutResponse &rhi){
	*this = rhi;
}

PutResponse & PutResponse::operator=(const PutResponse &rhi){
	if (this == &rhi)
		return *this;
	AResponse::operator=(rhi);
	return *this;
}

PutResponse::~PutResponse(){
}

std::string PutResponse::generateResponse() {
	return generateHeader() + generateBody();
}
std::string PutResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n"
//		<< "Content-Length: 0" << "\r\n"
		<< "Connection: keep-alive" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string PutResponse::generateBody() {
	return "";
}
