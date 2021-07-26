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
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

#define CHILD 0
#define FAILURE -1
#define BUFFER_SIZE 1024

class Response{
private:
	static const std::string			_protocol;
	static const std::string			_errorPageFolder;
	static std::map<int, std::string>	_errorPage;
	static std::map<int, std::string>	_code;
	static std::map<int, std::string>	_createMap();
	static std::map<int, std::string>	_createErrorPage();

	int 		_status;
	std::string	_method;
	std::string	_root;
	size_t		_fileSize;
	std::string	_fileName;
	std::string	_uplRoot;
	std::string	_uplFileName;

	void 		cgiChild(int pipeIn[2], int pipeOut[2], const std::string & cgiName);
	std::string	cgiParent(int pipeIn[2], int pipeOut[2], pid_t pid);

public:
	size_t getFileSize() const;

private:
	char**		generateCgiEnv();

public:
	std::string _buffer;

	Response();
	~Response();

	void 				setStatus(int n);
	int 				getStatus() const;
	void 				setMethod(const std::string &);
	const std::string & getMethod() const;
	void 				setRoot(const std::string &);
	const std::string & getRoot() const;
	void 				setFileName(const std::string &);
	const std::string & getFileName() const;
	void 				setUplRoot(const std::string &);
	const std::string & getUplRoot() const;
	void 				setUplFileName(const std::string &);
	const std::string & getUplFileName() const;

	std::string			cgi(const std::string & cgiName);
	std::string 		upload(const char * data);
	std::string			generateResponse();
	std::string			generateResponseCGI();
	std::string			generateHeader();
	std::string			generateBody();
};


#endif //RESPONSE_HPP
