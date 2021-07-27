//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#include "Response.hpp"

const std::string Response::_protocol = "HTTP/1.1";

const std::string Response::_cgiInputFile = "outputMy.txt";

const std::string Response::_cgiOutputFile = "outputCGI.txt";

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

	try
	{
		env = new char* [sizeof(char*) * 4];
	}
	catch (std::exception & e){
		return 0;
	}
	env[0] = (char *)"REQUEST_METHOD=get";
	env[1] = (char *)"SERVER_PROTOCOL=HTTP/1.1";
	env[2] = (char *)"PATH_INFO=./root/info.php";
	env[3] = 0;
	return env;
}

void Response::cgiChild(const std::string & cgiName) {

	char **env = generateCgiEnv();
	if (env == NULL){
		exit(2);
	}

	int in = open(_cgiInputFile.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
	int out = open(_cgiOutputFile.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

	if(in == -1 || out == -1)
		exit(2);

	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		exit(2);
	execve(cgiName.c_str(), 0, env);
	exit(1);
}

std::string Response::cgiParent(pid_t pid){
	int					status;
	std::stringstream	str;

	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
		status = WEXITSTATUS(status);

	if (status == EXIT_SUCCESS){
		std::ifstream inputCGI(_cgiOutputFile, std::ifstream::in);

		if (!inputCGI.is_open()){
			std::cout << "cannot write to outputMY" << std::endl;
			_status = 500;
			return generateBody();
		}

		std::string buf;
		while (std::getline(inputCGI, buf))
			str << buf << std::endl;
	}
	else if (status == EXIT_FAILURE){
		std::cout << "execve error" << std::endl;
		_status = 502;
		return generateBody();
	}
	else {
		std::cout << "child error" << std::endl;
		_status = 500;
		return generateBody();
	}
	return str.str();
}



std::string Response::cgi(const std::string & cgiName){

	std::ofstream outMy(_cgiInputFile,std::ofstream::out);

	if (!outMy.is_open()){
		std::cout << "cannot write to outputMY" << std::endl;
		_status = 500;
		return generateBody();
	}

	outMy << generateBody();
	outMy.close();

	pid_t pid = fork();
	if (pid == FAILURE){
		std::cout << "fork error" << std::endl;
		_status = 500;
		return generateBody();
	}
	else if (pid == CHILD){
		cgiChild(cgiName);
	}
	return cgiParent(pid);
}

std::string Response::generateResponseCGI(){
//TODO: REWORK CGI NAME
	std::string cgiName = CGI;

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
