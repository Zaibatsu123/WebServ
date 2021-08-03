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

AutoIndexResponse::~AutoIndexResponse(){
}

std::string AutoIndexResponse::generateResponse() {
	if (getHead())
		return generateHeader();
	return generateHeader() + getBody();
}

std::string AutoIndexResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << std::endl
		<< "Server: Equal-Rights/0.1.23" << std::endl
		<< "Date: " << _dateTime()
		<< "Content-Type: text/html" << std::endl
		<< "Content-Length: "<< getBody().size() << std::endl
		<< "Connection: keep-alive" << std::endl
		<< std::endl;
	return str.str();
}

std::string AutoIndexResponse::generateBody() {
	return "";
}

