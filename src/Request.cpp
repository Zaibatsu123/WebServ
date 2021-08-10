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

// std::vector<std::string> getarray(std::string req);
// std::string trim(std::string old_string);
// std::string trim_end(std::string old_string);

Request::Request(){
	_method = "";
	_content_length = "";
	_content_type = "";
	_path = "";
	_body = "";
	_protocol = "";
	_body_content = "";
	_filename = "uploadFilename";
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
std::string Request::getBodyCnt() const	{return _body_content; };
std::string Request::getFilename() const 	{return _filename; };

void Request::methodpath(std::string method, std::string path)
{
	_path = rduplicate_slashes(path);
	_method = method;
}

void Request::getheaders(std::vector<std::string> request)
{
	std::string str;
	// if (trim(request[1]).empty())
	// {
	// 	_err = 400;
	// 	return ;
	// }	
	str = request[1];
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str.compare(0, 5, "host:") == 0)
		_host = trim(str.substr(5, str.length() - 5));
}


void Request::postbody(std::string body_request) 
{
	
	std::vector<std::string> body;
	std::vector<std::string> request;

	std::cout << "BODYPARSSIZE:|" << body_request.size() << std::endl;
	std::cout << body_request << std::endl;
	body = getarray(body_request);
	request = splitvector(body, _boundary);
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
		if (request[j].empty() && j + 1 < request.size())
		{
			for (size_t k = j + 1; k < request.size(); ++k)
			{
				if (k != request.size() - 1)
					_body_content += request[k] + "\n";
				else 
					_body_content += request[k];
			}
			break;
		}
	}
}

void Request::postheaders(std::vector<std::string> request) 
{
	// std::ofstream outf;                                  // DELETE AFTER DEBUG
    // outf.open( "hhh.txt", std::ios_base::app);
	
	std::string str;
	std::cout << "\033[1;46m1\033[0m" << std::endl;
	if (request.size() > 1)
		for (size_t i = 0; i < request.size(); ++i)
		{
			str = request[i];
			transform(str.begin(), str.end(), str.begin(), ::tolower);
			if (!str.empty() && str.length() > 16 && str.compare(0, 16, "content-length: ") == 0)
				_content_length = trim(str.substr(16, str.length() - 16));
			else if (!str.empty() &&  str.length() > 14 && str.compare(0, 14, "content-type: ") == 0)
			{
				if (request[i].find("boundary=") == std::string::npos)
					break;
				int bn = request[i].find("boundary=");
				_content_type = trim(str.substr(14, str.length() - 14));
				_boundary = "--" + request[i ].substr(bn + 9, request[i].length() - bn - 9);
			}
		}
	// outf << "BODY CONTENT\n" << _body_content << std::endl;
	// outf << "FILENAME: " << _filename << std::endl;
	// outf << "BODY\n" << _body << std::endl;
	// outf << "CONTENT TYPE: " << _content_type << std::endl;
	// outf << "BOUNDARY: " << _boundary << std::endl;
	// outf.close();
}

/*
 START
*/
void Request::strrequest(std::vector<std::string> request)
{
	if (request.size() == 0)
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
		if (trim(request[i]).compare(request[i].length() - 8, 8, "HTTP/1.1") == 0)
			_protocol = "HTTP/1.1";
		else
		{
			_err = 505;
			return ;
		}
		size_t n = std::count(request[i].begin(), request[i].end(), '/');
		if (n == 0)
		{
			_err = 400;
			return ;
		}
		if (request[i].compare(0, 3, "GET") == 0)
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
			this->methodpath("PUT", trim(request[i].substr(3, request[i].length() - 12)));
		else
		{
			_err = 501;
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
