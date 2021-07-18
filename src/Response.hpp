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
#include <fstream>
#include <sstream>

class Response{
private:
	static const std::string			_protocol;
	static std::map<int, std::string>	_code;
	static std::map<int, std::string>	_createMap();

	int 		_status;
	std::string	_uplRoot;
public:
	Response();
	~Response();

	void 				setStatus(int n);
	int 				getStatus() const;
	void 				setUplRoot(const std::string & root);
	const std::string & getUplRoot() const;

	void				display(const std::string & fileName) const;
	std::string 		upload(const std::string & fileName, const char * data, const std::string & responseFileName) const;
	std::string			generateResponse(const std::string & fileName) const;
	std::string			generateHeader(int status) const;
	std::string			generateBody(int status, const std::string & fileName) const;
};


#endif //RESPONSE_HPP
