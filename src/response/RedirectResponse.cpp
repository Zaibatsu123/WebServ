//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/18/21				 //
//                                   //

#include "RedirectResponse.hpp"

RedirectResponse::RedirectResponse() : AResponse() {
}

RedirectResponse::RedirectResponse(int status, const std::string &redirect) : AResponse("", ""){
	_status = status;
	_redirect = redirect;
}

RedirectResponse::RedirectResponse(const RedirectResponse &rhi){
	*this = rhi;
}

RedirectResponse & RedirectResponse::operator=(const RedirectResponse &rhi){
	if (this == &rhi)
		return *this;
	AResponse::operator=(rhi);
	return *this;
}

RedirectResponse::~RedirectResponse(){
}

std::string RedirectResponse::generateResponse() {
	return generateHeader() + generateBody();
}

std::string RedirectResponse::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\r\n"
		<< "Server: Equal-Rights/0.1.23" << "\r\n"
		<< "Location: " << _redirect << "\r\n"
		<< "Connection: keep-alive" << "\r\n"
		<< "\r\n";
	return str.str();
}

std::string RedirectResponse::generateBody() {
	return "";
}

