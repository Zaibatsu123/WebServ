//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "Response.hpp"

Response::Response() {
	std::stringstream str;
	str << "HTTP/1.1 200 OK\n";
	str << "Content-Type: text/html; charset=UTF-8\n";
	str << "Connection: close\n";
//		TODO: int to len convert
//		this->expandResponse("Content-Length: 21\n");
	str << "\n";
	str << "<!DOCTYPE html>\n"
		   "<html>\n"
		   "<head>\n"
		   "<title>Welcome to server!</title>\n"
		   "<style>\n"
		   "    body {\n"
		   "        width: 35em;\n"
		   "        margin: 0 auto;\n"
		   "        font-family: Tahoma, Verdana, Arial, sans-serif;\n"
		   "    }\n"
		   "</style>\n"
		   "</head>\n"
		   "<body>\n"
		   "<h1>EQUAL RIGHT SERVER!</h1>\n"
		   "<p>Hello! It is working!</p>\n"
		   "\n"
		   "<p>For online support please refer to\n"
		   "<a href=\"http://google.com/\">->TAP<-</a>.<br/>\n"
		   "Commercial support is available!\n"
		   "\n"
		   "<p><em>Thank you for using our server.</em></p>\n"
		   "</body>\n"
		   "</html>\n";
	_response = str.str();
}

Response::~Response(){
}

void Response::display() const{
	std::cout << "\e[37m" << _response << "\e[0m" << std::endl;
}

void Response::expandResponse(const std::string &string){
	std::stringstream str;
	str << _response;
	str << string;
	_response = str.str();
}

void Response::addMapField(const std::string &key, const std::string &value){
	_c.insert(std::pair<std::string, std::string>(key, value));
}

void Response::setResponse(const std::string &response){
	_response = response;
}

const std::string &Response::getResponse() const{
	return _response;
}
