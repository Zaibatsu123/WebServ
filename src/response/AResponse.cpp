//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "AResponse.hpp"

const std::string AResponse::_protocol = "HTTP/1.1";

std::map<std::string, std::string> AResponse::_fileTypes = _createTypesMap();

std::map<std::string, std::string> AResponse::_createTypesMap() {
	std::map<std::string, std::string> m;
	m.insert(std::pair<std::string, std::string>(".json", "application/json"));
	m.insert(std::pair<std::string, std::string>(".zip", "application/zip"));
	m.insert(std::pair<std::string, std::string>(".pdf", "application/pdf"));
	m.insert(std::pair<std::string, std::string>(".html", "text/html"));
	m.insert(std::pair<std::string, std::string>(".xml", "text/xml"));
	m.insert(std::pair<std::string, std::string>(".css", "text/css"));
	m.insert(std::pair<std::string, std::string>(".js", "text/javascript"));
	m.insert(std::pair<std::string, std::string>(".jpg", "image/jpeg"));
	m.insert(std::pair<std::string, std::string>(".jpeg", "image/jpeg"));
	m.insert(std::pair<std::string, std::string>(".png", "image/png"));
	m.insert(std::pair<std::string, std::string>(".ico", "image/x-icon"));
	m.insert(std::pair<std::string, std::string>(".gif", "image/gif"));
	m.insert(std::pair<std::string, std::string>(".mpeg", "audio/mpeg"));
	m.insert(std::pair<std::string, std::string>(".mp4", "video/mp4"));
	m.insert(std::pair<std::string, std::string>(".webm", "video/webm"));
	return m;
}

std::map<int, std::string> AResponse::_code = AResponse::_createMap();

std::map<int, std::string> AResponse::_createMap() {
	std::map<int, std::string> m;
	m.insert(std::pair<int, std::string>(200, "OK"));
	m.insert(std::pair<int, std::string>(201, "Created"));
	m.insert(std::pair<int, std::string>(204, "No Content"));
	m.insert(std::pair<int, std::string>(301, "Moved Permanently"));
	m.insert(std::pair<int, std::string>(400, "Bad Request"));
	m.insert(std::pair<int, std::string>(403, "Forbidden"));
	m.insert(std::pair<int, std::string>(404, "Not Found"));
	m.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	m.insert(std::pair<int, std::string>(413, "Request Entity Too Large"));
	m.insert(std::pair<int, std::string>(500, "Internal Server error"));
	m.insert(std::pair<int, std::string>(501, "Not Implemented"));
	m.insert(std::pair<int, std::string>(502, "Bad Gateway"));
	m.insert(std::pair<int, std::string>(503, "Service Unavailable"));
	m.insert(std::pair<int, std::string>(505, "HTTP Version Not Supported"));
	return m;
}

AResponse::AResponse() :
		_status(200),
		_head(0),
		_root(""),
		_fileName(""),
		_uplRoot(""),
		_uplFileName(""){
}

AResponse::AResponse(const std::string & root, const std::string & fileName) :
		_status(200),
		_head(0),
		_root(root),
		_fileName(fileName),
		_uplRoot(root + "/tmp/"),
		_uplFileName(""){
}

AResponse::AResponse(const AResponse &rhi){
	*this = rhi;
}

AResponse & AResponse::operator=(const AResponse &rhi){
	if (this == &rhi)
		return *this;
	_status = rhi._status;
	_head = rhi._head;
	_root = rhi._root;
	_fileName = rhi._fileName;
	_uplRoot = rhi._uplRoot;
	_uplFileName = rhi._uplFileName;
	return *this;
}

AResponse::~AResponse(){
}

std::string AResponse::_indicateFileType() const{
	std::map<std::string, std::string>::iterator it;
	for (it = _fileTypes.begin(); it != _fileTypes.end(); ++it){
		if (_fileName.find(it->first) != std::string::npos)
			return it->second;
	}
	return "text/html";
}

std::streamsize AResponse::_calculateFileSize() const{
	std::ifstream	srcFile;

	if (_status != 200)
		srcFile.open(_fileName, std::ifstream::in);
	else
		srcFile.open((_root + _fileName).c_str(), std::ifstream::in);

	if (!srcFile.is_open()){
		return 0;
	}

	srcFile.seekg (0, srcFile.end);
	std::streamsize size = srcFile.tellg();
	srcFile.seekg (0, srcFile.beg);
	srcFile.close();

	if (static_cast<int>(size) == -1)
		return 0;
	return size;
}

void AResponse::setStatus(int n) {
	_status = n;
}

int AResponse::getStatus() const {
	return _status;
}

void AResponse::setRoot(const std::string & root) {
	_root = root;
}

const std::string & AResponse::getRoot() const {
	return _root;
}

void AResponse::setFileName(const std::string & fileName) {
	_fileName = fileName;
}

const std::string & AResponse::getFileName() const {
	return _fileName;
}

void AResponse::setUplRoot(const std::string & root) {
	_uplRoot = root;
}

const std::string & AResponse::getUplRoot() const {
	return _uplRoot;
}

void AResponse::setUplFileName(const std::string & uplFileName) {
	_uplFileName = _uplRoot + uplFileName;
}

const std::string & AResponse::getUplFileName() const {
	return _uplFileName;
}

size_t AResponse::getFileSize() const
{
	return _fileSize;
}

void AResponse::setFileSize(size_t fileSize)
{
	_fileSize = fileSize;
}

const std::string &AResponse::getBody() const
{
	return _body;
}

void AResponse::setBody(const std::string &body)
{
	_body = body;
}

const std::list<std::string> &AResponse::getAllowedMethods() const
{
	return _allowedMethods;
}

void AResponse::setAllowedMethods(const std::list<std::string> &allowedMethods)
{
	_allowedMethods = allowedMethods;
}

int AResponse::getHead() const
{
	return _head;
}

void AResponse::setHead(int head)
{
	_head = head;
}
