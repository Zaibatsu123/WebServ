//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <map>
#include <sstream>

class Response{
private:
	std::string	_response;
	std::map<std::string, std::string>	_c;
public:
	Response();
	~Response();

	void display() const;

	const std::string &	getResponse() const;
	void				setResponse(const std::string &response);
	void 				expandResponse(const std::string & string);
	void				addMapField(const std::string & key, const std::string & value);
};


#endif //RESPONSE_HPP
