//                                          ┏┻━━┻┓        //
// Created by:                        ╭┓┏╮  ┃▕▏▕▏┃  ╭┓┏╮  //
//        Jolene Radioactive          ┃┗┛┃  ┃┏┳┳┓┃  ┃┗┛┃  //
//         on:                        ╰┳┳╯  ┃┗┻┻┛┃  ╰┳┳╯  //
//              07/21				   ┃┃ ┏━┻━━━━┻━┓ ┃┃   //
//                                     ┃╰━┫╭━━━━━━╮┣━╯┃   //
//                                     ╰━━┫┃╱╲╱╲╱╲┃┣━━╯   //

#include "Request.hpp"
#include <algorithm>
#include <vector>

std::vector<std::string> getarray(std::string req);
std::string trim(std::string old_string);
std::string trim_end(std::string old_string);

Request::Request(){
	_method = "";
	_content_length = "";
	_content_type = "";
	_path = "";
	_body = "";
	_protocol = "";
	_conection = "keep-alive";
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
std::string  Request::getBody() const	{return _body; };

void Request::methodpath(std::string method, std::string path)
{
	_path = path;
	_method = method;
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
		this->methodpath("GET", trim(request[0].substr(3, request[0].length() - 11)));
	else if (request[0].compare(0, 4, "POST") == 0)
	{
		this->methodpath("POST", trim(request[0].substr(4, request[0].length() - 12)));
		postrequest(request);
	}
	else if (request[0].compare(0, 6, "DELETE") == 0)
		this->methodpath("DELETE", trim(request[0].substr(6, request[0].length() - 14)));
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
		_host = trim(str.substr(5, str.length() - 5));
}

// void Request::getrequest(std::vector<std::string> request){
// }


void Request::postrequest(std::vector<std::string> request) 
{
	std::string str;
	if (request.size() > 1)
		for (size_t i = 0; i < request.size(); ++i)
		{
			str = request[i];
			transform(str.begin(), str.end(), str.begin(), ::tolower);
			if (str.compare(0, 16, "content-length: ") == 0)
				_content_length = trim(str.substr(16, str.length() - 16));
			else if (str.compare(0, 14, "content-type: ") == 0)
				_content_type = trim(str.substr(14, str.length() - 14));
			else if (str.empty() && i + 1 < request.size())
				for (size_t j = i + 1; j < request.size(); ++j)
				{
					if (j != request.size() - 1)
						_body += request[j] + "\n";
					else 
						_body += request[j];
				}
		}
}

// void Request::deleterequest(std::vector<std::string> request){

// }
