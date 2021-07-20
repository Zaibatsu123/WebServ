//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "Response.hpp"

const std::string Response::_protocol = "HTTP/1.1";

std::map<int, std::string> Response::_code = Response::_createMap();

std::map<int, std::string> Response::_createMap() {
	std::map<int, std::string> m;
	m.insert(std::pair<int, std::string>(200, "OK"));
	m.insert(std::pair<int, std::string>(404, "not found"));
	return m;
}

Response::Response() : _status(0), _method("get"), _root(""), _fileName(""), _uplRoot(""), _uplFileName(""), _buffer(""){
}

Response::~Response(){
}

std::string Response::generateResponse(const std::string &fileName) {
	std::ifstream	srcFile;

	srcFile.open(fileName.c_str(), std::ifstream::in);

	// if file cannot fine -> response 404
	if (!srcFile.is_open()){
		_status = 404;
		srcFile.open("./root/404.html", std::ifstream::in);
	}
	else {
		_status = 200;
	}

	// Get file size:
	srcFile.seekg (0, srcFile.end);
	_fileSize = srcFile.tellg();
	srcFile.seekg (0, srcFile.beg);


	return this->generateHeader() + this->generateBody(fileName);
}

std::string Response::generateHeader() {
	std::stringstream str;

	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\n"
		<< "Connection: keep-alive\n"
//		<< "Content-Type: " << _fileType << "\n"
		<< "Content-Length: " << _fileSize << "\n"
		<< "\n";
	return str.str();
}

std::string Response::generateBody(const std::string & fileName) {
	std::string			buffer;
	std::ifstream		file;
	std::stringstream	str;

	if (_status == 404)
		file.open("./root/404.html", std::ifstream::in);
	else
		file.open(fileName.c_str(), std::ifstream::in);
	while (file.good()) {
		std::getline(file, buffer);
		str << buffer;
		str << "\n";
	}
	return str.str();
}

std::string Response::upload(const std::string & fileName, const char *data, const std::string & responseFileName) {
	std::ofstream	dstFile;
	std::string		tmp = _uplRoot + fileName;
	std::cout << "upload: " << tmp << std::endl;
	dstFile.open(tmp.c_str(), std::ofstream::out);
	dstFile << std::string(data);
	return this->generateHeader() + this->generateBody(responseFileName);
}

void Response::setStatus(int n) {
	_status = n;
}

int Response::getStatus() const {
	return _status;
}

void Response::setMethod(const std::string & method) {
	_method = method;
}

const std::string & Response::getMethod() const {
	return _method;
}

void Response::setRoot(const std::string & root) {
	_root = root;
}

const std::string & Response::getRoot() const {
	return _root;
}

void Response::setFileName(const std::string & fileName) {
	_fileName = fileName;
}

const std::string & Response::getFileName() const {
	return _fileName;
}

void Response::setUplRoot(const std::string & root) {
	_uplRoot = root;
}

const std::string & Response::getUplRoot() const {
	return _uplRoot;
}

void Response::setUplFileName(const std::string & uplFileName) {
	_uplFileName = uplFileName;
}

const std::string & Response::getUplFileName() const {
	return _uplFileName;
}
