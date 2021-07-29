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
#include <sys/fcntl.h>

class Response{
private:
	static const std::string			_protocol;
	static const std::string			_errorPageFolder;
	static std::map<int, std::string>	_errorPage;
	static std::map<int, std::string>	_code;
	static std::map<int, std::string>	_createMap();
	static std::map<int, std::string>	_createErrorPage();

	int 		_status;
	std::string	_root;
	size_t		_fileSize;
	std::string	_fileName;
	std::string	_uplRoot;
	std::string	_uplFileName;
	std::string _body;

	size_t _calculateFileSize(const std::string & fileName);

public:

	Response();
	~Response();

	Response(const std::string & root, const std::string & fileName);

	void 				setStatus(int n);
	int 				getStatus() const;
	void 				setRoot(const std::string &);
	const std::string &	getRoot() const;
	void 				setFileName(const std::string &);
	const std::string &	getFileName() const;
	void				setFileSize(size_t fileSize);
	size_t				getFileSize() const;
	void 				setUplRoot(const std::string &);
	const std::string &	getUplRoot() const;
	void 				setUplFileName(const std::string &);
	const std::string &	getUplFileName() const;
	void				setBody(const std::string &body);
	const std::string &	getBody() const;

	std::string			generateResponse(int res);
	std::string			generateHeader(int status);
	std::string			generateBody();
};


#endif //RESPONSE_HPP
