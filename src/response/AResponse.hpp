//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#ifndef ARESPONSE_HPP
#define ARESPONSE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/fcntl.h>
#include <map>
#include <list>

class AResponse{
protected:
	static const std::string					_protocol;
	static const std::string					_errorPageFolder;
	static std::map<int, std::string>			_errorPage;
	static std::map<int, std::string>			_code;
	static std::map<int, std::string>			_createMap();
	static std::map<int, std::string>			_createErrorPage();
	static std::map<std::string, std::string>	_fileTypes;
	static std::map<std::string, std::string>	_createTypesMap();

	int 					_status;
	int 					_head;
	std::string				_root;
	size_t					_fileSize;
	std::string				_fileName;
	std::string				_uplRoot;
	std::string				_uplFileName;
	std::string 			_body;
	std::list<std::string>	_allowedMethods;

	size_t		_calculateFileSize() const;
	std::string	_indicateFileType() const;
	std::string _dateTime() const;

public:
	AResponse();
	virtual ~AResponse();

	AResponse(const std::string & root, const std::string & fileName);

	void							setHead(int head);
	int								getHead() const;
	void 							setStatus(int n);
	int 							getStatus() const;
	void 							setRoot(const std::string &);
	const std::string &				getRoot() const;
	void 							setFileName(const std::string &);
	const std::string &				getFileName() const;
	void							setFileSize(size_t fileSize);
	size_t							getFileSize() const;
	void 							setUplRoot(const std::string &);
	const std::string &				getUplRoot() const;
	void 							setUplFileName(const std::string &);
	const std::string &				getUplFileName() const;
	void							setBody(const std::string &body);
	const std::string &				getBody() const;
	void							setAllowedMethods(const std::list<std::string> &allowedMethods);
	const std::list<std::string> &	getAllowedMethods() const;



	virtual std::string	generateResponse()	= 0;
	virtual std::string	generateHeader()	= 0;
	virtual std::string	generateBody()		= 0;

};


#endif //ARESPONSE_HPP
