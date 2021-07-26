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

Response::Response() : _status(200), _method("get"), _root(""), _fileName(""), _uplRoot(""), _uplFileName(""), _buffer(""){
}

Response::~Response(){
}

char** Response::generateCgiEnv(){
	char **env;

	env = (char **)malloc(sizeof(char*) * 4);
	if (env == NULL)
		return 0;
	env[0] = (char *)"REQUEST_METHOD=get";
	env[1] = (char *)"SERVER_PROTOCOL=HTTP/1.1";
	env[2] = (char *)"PATH_INFO=./root/info.php";
	env[3] = 0;
	return env;
}

std::string Response::cgiParent(int pipeIn[2], int pipeOut[2], pid_t pid){
	int					status;
	char*				buffer = new char [BUFFER_SIZE];
	std::stringstream	str;

	close(pipeIn[0]);
	close(pipeOut[1]);

	std::string fileData = generateBody();

//	const char* bufferr = fileData.c_str();
//	std::cout << generateBody() << std::endl;

	int out = dup(1);
	dup2(pipeIn[1], 1);
	close(pipeIn[1]);



//	size_t totalData = fileData.length();
//	size_t sendData = 0;
//	while (sendData < totalData){
//
//		sendData += write(1, bufferr + sendData, BUFFER_SIZE);
//		std::cout << std::flush << std::endl;
//
//		std::memset(buffer, 0, BUFFER_SIZE);
//		read(pipeOut[0], buffer, BUFFER_SIZE);
//		std::cout << buffer << std::endl;
//		str << buffer;
//
//	}


	std::cout << generateBody() << std::endl;

	dup2(out, 1);
	close(out);

//	int in = dup(0);
//	dup2(pipeOut[0], 0);
//	close(pipeIn[1]);
	std::cout << "write done" << std::endl;
	waitpid(pid, &status, 0);
	std::cout << "wait done" << std::endl;
	if (status == EXIT_SUCCESS){
//		std::string a;
//		int i = 0;
//		while (std::cin.good()) {
//			std::cout << "it: " << i++ << std::endl;
//			std::getline(std::cin, a);
//			if (std::cin.bad())
//				std::cout << "BAD" << std::endl;
//			str << a;
//			str << "\n";
//		}
		std::memset(buffer, 0, BUFFER_SIZE);
		while (read(pipeOut[0], buffer, BUFFER_SIZE) > 0){
			std::cout << buffer << std::endl;
			str << buffer;
		}
		std::cout << "read done" << std::endl;
	}
	else {
		std::cout << "execve error" << std::endl;
		_status = 502;
		return generateBody();
	}
	close(pipeOut[0]);
//	delete[] buffer;
//	dup2(in, 0);
	return str.str();
}

void Response::cgiChild(int pipeIn[2], int pipeOut[2], const std::string & cgiName) {

	dup2(pipeIn[0], 0);
	close(pipeIn[0]);

	dup2(pipeOut[1], 1);
	close(pipeOut[1]);

	close(pipeOut[0]);
	close(pipeIn[1]);


//TODO: DELETE MALLOC CGIENV
	char **env = generateCgiEnv();
	if (env == NULL){
		exit(1);
	}
	execve(cgiName.c_str(), 0, env);

	exit(1);
}

std::string Response::cgi(const std::string & cgiName){
	std::string	str;
	pid_t		pid;
	int			pipeIn[2];
	int			pipeOut[2];

	if (pipe(pipeIn) || pipe(pipeOut)){
		std::cout << "Pipe error" << std::endl;
		_status = 500;
		return generateBody();
	}
	pid = fork();
	if (pid == FAILURE){
		std::cout << "fork error" << std::endl;
		_status = 500;
		return generateBody();
	}
	else if (pid == CHILD){
		cgiChild(pipeIn, pipeOut, cgiName);
	}
	return cgiParent(pipeIn, pipeOut, pid);
}

std::string Response::generateResponseCGI(){
//TODO: REWORK CGI NAME
//	std::string cgiName = "./root/cgi_tester";
	std::string cgiName = "./root/myCGI";

	std::string cgiString = cgi(cgiName);
	_fileSize = cgiString.length();

	return generateHeader() + cgiString;
}

std::string Response::generateResponse() {
	std::ifstream	srcFile;

	srcFile.open((_root + _fileName).c_str(), std::ifstream::in);

	// if file cannot fine -> response 404
	if (!srcFile.is_open()){
		_status = 404;
	//TODO: KOSTYL!
		_fileSize =  409;
	}
	else {
		// Get file size:
		srcFile.seekg (0, srcFile.end);
		_fileSize = srcFile.tellg();
		srcFile.seekg (0, srcFile.beg);
	}


	return generateHeader() + generateBody();
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

std::string Response::upload(const char *data) {
	std::ofstream	dstFile;

	std::string fileName = "/uploadFile.txt";
	std::string		tmp = _uplRoot + fileName;

	std::cout << "upload: " << tmp << std::endl;
	dstFile.open(tmp.c_str(), std::ofstream::out);
	dstFile << std::string(data);
	dstFile.close();
	//TODO: KOSTYL!
	_fileName = "/uploadSuccess.html";
	_fileSize =	282;
	return generateHeader() + generateBody();
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

size_t Response::getFileSize() const
{
	return _fileSize;
}
