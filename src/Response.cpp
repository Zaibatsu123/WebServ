//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "Response.hpp"

const std::string Response::_protocol = "HTTP/1.1";

const std::string Response::_errorPageFolder = "./root/errorPages/";

std::map<int, std::string> Response::_code = Response::_createMap();

std::map<int, std::string> Response::_errorPage = Response::_createErrorPage();

std::map<int, std::string> Response::_createMap() {
	std::map<int, std::string> m;
	m.insert(std::pair<int, std::string>(200, "OK"));
	m.insert(std::pair<int, std::string>(400, "Bad Request"));
	m.insert(std::pair<int, std::string>(403, "Forbidden"));
	m.insert(std::pair<int, std::string>(404, "Not Found"));
	m.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	m.insert(std::pair<int, std::string>(500, "Internal Server error"));
	m.insert(std::pair<int, std::string>(501, "Not Implemented"));
	m.insert(std::pair<int, std::string>(502, "Bad Gateway"));
	m.insert(std::pair<int, std::string>(503, "Service Unavailable"));
	m.insert(std::pair<int, std::string>(505, "HTTP Version Not Supported"));
	return m;
}

std::map<int, std::string> Response::_createErrorPage() {
	std::map<int, std::string> m;
	m.insert(std::pair<int, std::string>(403, _errorPageFolder + "403.html"));
	m.insert(std::pair<int, std::string>(400, _errorPageFolder + "400.html"));
	m.insert(std::pair<int, std::string>(404, _errorPageFolder + "404.html"));
	m.insert(std::pair<int, std::string>(405, _errorPageFolder + "405.html"));
	m.insert(std::pair<int, std::string>(500, _errorPageFolder + "500.html"));
	m.insert(std::pair<int, std::string>(501, _errorPageFolder + "501.html"));
	m.insert(std::pair<int, std::string>(502, _errorPageFolder + "502.html"));
	m.insert(std::pair<int, std::string>(503, _errorPageFolder + "503.html"));
	m.insert(std::pair<int, std::string>(505, _errorPageFolder + "505.html"));

	return m;
}

Response::Response() : _status(200), _root(""), _fileName(""), _uplRoot(""), _uplFileName(""){
}

Response::Response(const std::string & root, const std::string & fileName) : _status(200), _root(root), _fileName(fileName), _uplRoot(root + "/tmp/"), _uplFileName(""){
}

Response::~Response(){
}


std::string Response::generateResponse() {
	return generateHeader() + generateBody();
}



std::string Response::generateHeader() {
	std::stringstream str;
	str << _protocol << " "
		<< _status << " "
		<< _code[_status] << "\n"
		<< "Connection: keep-alive\n"
//		<< "Content-Type: " << "text/html" << "\n"
		<< "Content-Length: " << _calculateFileSize(_root + _fileName) << "\n"
		<< "\n";
	return str.str();
}

std::string Response::generateBody() {
	std::string			buffer;
	std::ifstream		file;
	std::stringstream	str;

	if (_status != 200){
		std::cout << "STATUS: " << _status << std::endl;
		std::cout << _errorPage[_status] << std::endl;
		file.open(_errorPage[_status], std::ifstream::in);
	}
	else
		file.open((_root + _fileName).c_str(), std::ifstream::in);

	while (file.good()) {
		std::getline(file, buffer);
		str << buffer;
		if (file.good())
			str << "\n";
	}
	file.close();
	return str.str();
}

size_t Response::_calculateFileSize(const std::string &fileName){
	std::ifstream	srcFile;

	if (_status != 200)
		srcFile.open(_errorPage[_status], std::ifstream::in);
	else
		srcFile.open(fileName.c_str(), std::ifstream::in);

	if (!srcFile.is_open()){
		return 0;
	}

	srcFile.seekg (0, srcFile.end);
	size_t size = srcFile.tellg();
	srcFile.seekg (0, srcFile.beg);

	srcFile.close();
	return size;
}

void Response::setStatus(int n) {
	_status = n;
}

int Response::getStatus() const {
	return _status;
}

void Response::setRoot(const std::string & root) {
	_root = root;
}

const std::string & Response::getRoot() const {
	return _root;
}

void Response::setFileName(const std::string & fileName) {
	if (fileName == "/")
		_fileName = "/index.html";
	else
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
	_uplFileName = _uplRoot + uplFileName;
}

const std::string & Response::getUplFileName() const {
	return _uplFileName;
}

size_t Response::getFileSize() const
{
	return _fileSize;
}

void Response::setFileSize(size_t fileSize)
{
	_fileSize = fileSize;
}

const std::string &Response::getBody() const
{
	return _body;
}

void Response::setBody(const std::string &body)
{
	_body = body;
}
