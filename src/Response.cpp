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
	m.insert(std::pair<int, std::string>(404, "not found"));
	return m;
}

std::map<int, std::string> Response::_createErrorPage() {
	std::map<int, std::string> m;
	m.insert(std::pair<int, std::string>(404, _errorPageFolder + "404.html"));

	return m;
}

		Response::Response() : _status(0), _method("get"), _root(""), _fileName(""), _uplRoot(""), _uplFileName(""), _buffer(""){
}

Response::~Response(){
}

char** Response::generateCgiEnv(){
	char **env;

	env = (char **)malloc(sizeof(char*) * 4);
	env[0] = (char *)"REQUEST_METHOD=get";
	env[1] = (char *)"SERVER_PROTOCOL=HTTP/1.1";
	env[2] = (char *)"PATH_INFO=./root/info.php";
	env[3] = 0;
	return env;
}

std::string Response::cgiParent(int *fds1, int *fds2, pid_t pid){
	int					status;
	char				buffer[1024];
	std::stringstream	str;

	close(fds1[0]);
	close(fds2[1]);

	std::string fileData = this->generateBody(_fileName);

	write(fds1[1], fileData.c_str(), fileData.length());
	close(fds1[1]);

	waitpid(pid, &status, 0);
	status = 1;
	if (status >= 0){
		std::memset(buffer, 0, 1024);
		while (read(fds2[0], buffer, 1024) > 0)
			str << buffer;
		close(fds2[0]);
	}
	else{
		close(fds2[0]);
		str << this->generateBody(this->getRoot() + "/502.html");
	}
	return str.str();
}

void Response::cgiChild(int fds1[2], int fds2[2], const std::string & cgiName) {
	dup2(fds1[0], 0);
	dup2(fds2[1], 1);

	close(fds1[1]);
	close(fds2[0]);

	char **env = this->generateCgiEnv();
	int res = execve(cgiName.c_str(), 0, env);

	if (res < 0){
		this->setStatus(502);
	}

	exit(1);
}

std::string Response::cgi(const std::string & cgiName){
	std::string	str;
	pid_t		pid;
	int			fds1[2];
	int			fds2[2];

	pipe(fds1);
	pipe(fds2);
	pid = fork();
	if (pid == -1){
		std::cout << "fork error" << std::endl;
		this->setStatus(500);
		return this->generateBody(this->getRoot() + "/500.html");
	}
	else if (pid == 0){
		this->cgiChild(fds1, fds2, cgiName);
	}
	else
		str = this->cgiParent(fds1, fds2, pid);
	return str;
}

std::string Response::generateResponse(const std::string &fileName) {
	std::ifstream	srcFile;

	srcFile.open(fileName.c_str(), std::ifstream::in);

	// if file cannot fine -> response 404
	if (!srcFile.is_open()){
		_status = 404;
		srcFile.open(_errorPage[_status], std::ifstream::in);
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
//		<< "Content-Type: " << "text/html" << "\n"
		<< "Content-Length: " << _fileSize << "\n"
		<< "\n";
	return str.str();
}

std::string Response::generateBody(const std::string & fileName) {
	std::string			buffer;
	std::ifstream		file;
	std::stringstream	str;

	if (_status != 200)
		file.open(_errorPage[_status], std::ifstream::in);
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
