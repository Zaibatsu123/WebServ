//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#include "AutoIndexResponse.hpp"

AutoIndexResponse::AutoIndexResponse() : AResponse() {
}

AutoIndexResponse::AutoIndexResponse(const std::string & content) : AResponse(0, "",""){
	_body = content;
}

AutoIndexResponse::~AutoIndexResponse(){
}

std::string AutoIndexResponse::generateResponse() {
	return "";
}

std::string AutoIndexResponse::generateHeader() {
	return "";
}

std::string AutoIndexResponse::generateResponse(int) {
	if (getHead())
		return generateHeader(getBody().size());
	return generateHeader(getBody().size()) + getBody().c_str();
}

std::string AutoIndexResponse::generateHeader(int status) {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << std::endl
		<< "Server: Equal-Rights/0.1.23" << std::endl
		<< "Date: " << _dateTime()
		<< "Content-Type: text/html" << std::endl
		<< "Content-Length: "<< status << std::endl
		<< "Connection: keep-alive" << std::endl
		<< std::endl;
	return str.str();
}

std::string AutoIndexResponse::generateBody() {
	return "";
}

