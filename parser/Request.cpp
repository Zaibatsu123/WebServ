#include "Request.hpp"
#include <algorithm>
#include <vector>





Request::Request(){
	_err = 0;
}

Request::~Request(){
}

void Request::setErr(int err)			{ _err = err;}
int Request::getErr() const			{return _err; }

std::string Request::getMethod() const    {return _method; }
std::string Request::getPath() const    {return _path; }
std::string Request::getProtocol() const    {return _protocol; }
std::string Request::getHost() const    {return _host; }
std::string Request::getCType() const    {return _content_type; }
std::string Request::getCLength() const    {return _content_length; }

std::vector<std::string> Request::getarray(std::string req)
{
	std::vector<std::string> request;
	int index1 = -1;
	int index2 = 0;

	size_t n = std::count(req.begin(), req.end(), '\n');
	std::string str;
	for (int i = 0; i < n; i++)
	{
		index2 = req.find("\n");
		str = req.substr(index1 + 1, index2);
		index1 = index2;
		request.push_back(str);
	}
	return (request);
}

void Request::strrequest(std::vector<std::string> request){
	if (request[0].compare(request[0].length() - 8, 8, "HTTP/1.1") == 0)
		_protocol = "HTTP/1.1";
	else
	{
		_err = 2;
		return ;
	}
	size_t n = std::count(request[0].begin(), request[0].end(), '/');
	if (n == 0)
	{
		_err = 3;
		return ;
	}
	if (request[0].compare(0, 3, "GET") == 0)
	{
		_method = "GET";
		_path = request[0].substr(3, request[0].length() - 11);
	}
	else if (request[0].compare(0, 4, "POST") == 0)
	{
		_method = "POST";
		_path = request[0].substr(4, request[0].length() - 12);
	}
	else if (request[0].compare(0, 6, "DELETE") == 0)
	{
		_method = "DELETE";
		_path = request[0].substr(6, request[0].length() - 14);
	}
	else
	{
		_err = 1;
		return ;
	}
	getheaders(request);
}


void Request::getheaders(std::vector<std::string> request)
{
	std::string str;
	str = request[1];
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str.compare(0, 5, "host:") == 0)
		_host = str.substr(5, str.length() - 6);
}

void Request::getrequest(std::vector<std::string> request){
}


void Request::postrequest(std::vector<std::string> request) {

}

void Request::deleterequest(std::vector<std::string> request){

}