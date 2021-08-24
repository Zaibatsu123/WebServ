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
#include "../inc/output.hpp"

Request::Request(){
	_method = "";
	_host = "";
	_content_length = "";
	_content_type = "";
	_path = "";
	_body = "";
	_protocol = "";
	_body_content = "";
	_filename = "";
	_conection = "keep-alive";
	_err = 0;
	_transfer_code = "";
	_accept_code = "";
	_server = NULL;
}

Request::~Request(){
}

void Request::setServer(Server *server) {_server = server;}
Server *Request::getServer() 			{return _server;}
void Request::setErr(int err)			{ _err = err;}
int Request::getErr() const				{return _err; }

std::string Request::getMethod() const    {return _method; }
std::string Request::getPath() const    {return _path; }
std::string Request::getProtocol() const    {return _protocol; }
std::string Request::getHost() const    {return _host; }
std::string Request::getCType() const    {return _content_type; }
std::string Request::getCLength() const    {return _content_length; }
std::string  Request::getBody() const	{return _body; }
std::string Request::getBodyCnt() const	{return _body_content; }
std::string Request::getFilename() const 	{return _filename; }
std::string Request::getTransferCode() const 	{return _transfer_code; }
std::string Request::getAcceptCode() const 	{return _accept_code; }
std::string Request::getBoundary() const {return _boundary;}
std::map<std::string, std::string> Request::getHeaders_() const {return _headers;}


std::vector<std::string> Request::methods = array_methods();

std::vector<std::string> Request::array_methods()
{
	std::vector<std::string> m;
	m.push_back("CONNECT");
	m.push_back("DELETE");
	m.push_back("GET");
	m.push_back("HEAD");
	m.push_back("OPTIONS");
	m.push_back("PATCH");
	m.push_back("POST");
	m.push_back("PUT");
	m.push_back("TRACE");
	return(m);
}

std::vector<std::string> Request::protocols = array_protocols();

std::vector<std::string> Request::array_protocols()
{
	std::vector<std::string> m;
	m.push_back("HTTP/1.1");
	m.push_back("HTTP/1.0");
	m.push_back("HTTP/2.0");
	m.push_back("HTTP/3.0");
	m.push_back("HTTP/0.9");
	return(m);
}

void Request::methodpath(std::string method, std::string path)
{
	_path = rduplicate_slashes(path);
	_method = method;
}


void Request::getheaders(std::vector<std::string> request)
{
	std::string str;
	std::vector<std::string> host = std_split(request[1]);
	if (host.size() != 2)
	{
		_err = 400;
		std::cout << "\033[1;46mloloo\033[0m\n" << std::endl;
		return ;
	}		
	str = host[0];
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str.compare(0, 5, "host:") == 0)
		_host = host[1];
	else 
	{
		_err = 400;
		std::cout << "\033[1;46mELALLAK\033[0m\n" << std::endl;
		return ;
	}
	for (size_t j = 1; j < request.size(); ++j)
	{
		if (request[j].find(":") != std::string::npos)
		{
			int pos = request[j].find(":");
			std::string key = request[j].substr(0, pos);
			_headers.insert(std::pair<std::string, std::string>(key, request[j].substr(pos + 2, request[j].size() - pos - 2)));
		}
	}
}

void Request::body_chunk(std::string body_request)
{
	std::vector<std::string> body;

	std::cout << "\033[1;46mENTER IN BODY_CHUNK\033[0m\n" << std::endl;
	body = getarray(body_request);
	for (size_t j = 0; j < body.size(); ++j)
	{
		if (j % 2 == 1)
			_body_content += body[j];
	}
}

void Request::postbody(std::string body_request) 
{
	
	std::vector<std::string> body;
	std::vector<std::string> request;

	std::cout << "\033[1;46mFR4G-TP is born\033[0m\n" << std::endl;

	body = getarray(body_request);
	size_t n = count_str(body_request, _boundary);
	if (n > 1)
		request = splitvector(body, _boundary);
	else 
		request = body;
	if (request.size() == 0)
		return;
	if (request[0].find("filename=") == std::string::npos )
		return;
	int fn = request[0].find("filename=");
	_filename = request[0].substr(fn + 10, request[0].length() - fn - 11);
	for (size_t j = 0; j < request.size(); ++j)
	{
		if (!request[j].empty() && request[j].length() > 14 && request[j].compare(0, 14, "Content-Type: ") == 0)
			_content_type = trim(request[j].substr(14, request[j].length() - 14));
	}
	_body_content = content(body_request, _boundary);
}

void Request::postheaders(std::vector<std::string> request) 
{	
	std::string str;
	if (request.size() > 1)
		for (size_t i = 0; i < request.size(); ++i)
		{
			str = request[i];
			transform(str.begin(), str.end(), str.begin(), ::tolower);
			if (!str.empty() && str.length() > 16 && str.compare(0, 16, "content-length: ") == 0)
				_content_length = trim(str.substr(16, str.length() - 16));
			else if (!str.empty() && str.length() > 14 && str.compare(0, 14, "content-type: ") == 0)
			{
				if (request[i].find("boundary=") == std::string::npos)
					break;
				int bn = request[i].find("boundary=");
				_content_type = trim(str.substr(14, str.length() - 14));
				_boundary = "--" + request[i].substr(bn + 9, request[i].length() - bn - 9);
			}
			else if (!str.empty() && str.length() > 17 && str.compare(0, 17, "accept-encoding: ") == 0)
				_accept_code = trim(str.substr(17, str.length() - 17));
			else if (!str.empty() && str.length() > 19 && str.compare(0, 19, "transfer-encoding: ") == 0)
				_transfer_code = trim(str.substr(19, str.length() - 19));
		}
}

/*
 START
*/
void Request::strrequest(std::vector<std::string> request)
{
	std::cout << "\033[1;42mFR4G-TP\033[0m\n" << std::endl;
	if (request.size() <= 1)
	{
		_err = 400;
		return ;
	}
	size_t i = 0; 
	while (i < request.size())
	{
		if ((trim(request[i]).empty()))
			i++;
		else 
			break;
	}
	try 
	{
		size_t n = std::count(request[i].begin(), request[i].end(), '/');
		if (n == 0)
		{
			_err = 400;
			return ;
		}
		std::stringstream str(request[i]);
		std::string req;
		std::getline(str, req, ' ');
		std::cout << "req" << request[i]<< "STRRRRRRR|" << req << "|" <<std::endl;
		size_t k = 0;
		for (;k < methods.size(); k++)
			if (req == methods[k])
				break;
		if (k == methods.size())
		{
			_err = 400;
			std::cout << "RESP|" << _err <<std::endl;
			return ;
		}
		else if (request[i].compare(0, 3, "GET") == 0)
			this->methodpath("GET", trim(request[i].substr(3, request[i].length() - 12)));
		else if (request[i].compare(0, 4, "POST") == 0)
		{
			this->methodpath("POST", trim(request[i].substr(4, request[i].length() - 13)));
			postheaders(request);
		}
		else if (request[i].compare(0, 6, "DELETE") == 0)
			this->methodpath("DELETE", trim(request[i].substr(6, request[i].length() - 15)));
		else if (request[i].compare(0, 4, "HEAD") == 0)
			this->methodpath("HEAD", trim(request[i].substr(4, request[i].length() - 13)));
		else if (request[i].compare(0, 3, "PUT") == 0)
		{
			this->methodpath("PUT", trim(request[i].substr(3, request[i].length() - 12)));
			postheaders(request);
		}
		else
		{
			_err = 501;
			std::cout << "RESP|" << _err <<std::endl;
			return ;
		}
		std::vector<std::string> first = std_split(request[i]);
		size_t l = 0;
		if (first.size() >= 3)
			for (;l < protocols.size(); l++)
				if (first[2] == protocols[l])
					break;
		if (l == protocols.size() or first.size() != 3 )
		{
			_err = 400;
			std::cout << "RESPkk|" << _err <<std::endl;
			return ;
		}
		else if (first[2] == "HTTP/1.1")		
			_protocol = "HTTP/1.1";
		else
		{
			_err = 505;
			std::cout << "RESP|" << _err <<std::endl;
			return ;
		}
	}
	catch(const std::exception& e)
	{
		_err = 400;
		return ;
	}
	getheaders(request);
}
