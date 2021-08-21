//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "AutoIndexResponse.hpp"

AutoIndexResponse::AutoIndexResponse() : AResponse() {
}

AutoIndexResponse::AutoIndexResponse(const std::string & content) : AResponse("",""){
	_body = content;
}

AutoIndexResponse::AutoIndexResponse(const AutoIndexResponse &rhi){
	*this = rhi;
}

AutoIndexResponse & AutoIndexResponse::operator=(const AutoIndexResponse &rhi){
	if (this == &rhi)
		return *this;
	AResponse::operator=(rhi);
	return *this;
}

AutoIndexResponse::~AutoIndexResponse(){
}

std::string AutoIndexResponse::generateResponse() {
	return generateHeader() + getBody();
}

std::string AutoIndexResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n"
		<< "Content-Type: text/html" << "\r\n"
		<< "Content-Length: "<< getBody().size() << "\r\n"
		<< "Connection: keep-alive" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string AutoIndexResponse::generateBody() {
	return "";
}

