//                                          ┏┻━━┻┓        //
// Created by:                        ╭┓┏╮  ┃▕▏▕▏┃  ╭┓┏╮  //
//        Jolene Radioactive          ┃┗┛┃  ┃┏┳┳┓┃  ┃┗┛┃  //
//         on:                        ╰┳┳╯  ┃┗┻┻┛┃  ╰┳┳╯  //
//              07/21				   ┃┃ ┏━┻━━━━┻━┓ ┃┃   //
//                                     ┃╰━┫╭━━━━━━╮┣━╯┃   //
//                                     ╰━━┫┃╱╲╱╲╱╲┃┣━━╯   //

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

class Request{
public:
	std::string _method;
	std::string _path;
	std::string _protocol;
	std::string _host;
	std::string _port;
	std::string _body;
	std::string _body_content;
	std::string _filename;
	std::string _conection;
	std::string _content_type;
	std::string _content_length;
	std::string _boundary;
	
	int			_err;

public:
	Request();
	~Request();
	
	void setErr(int err);
	int getErr() const;

	std::string getMethod() const;
	std::string getPath() const;
	std::string getProtocol() const;
	std::string getHost() const;
	std::string getCType() const;
	std::string getCLength() const;
	std::string getBody() const;
	std::string getBodyCnt() const;
	std::string getFilename() const;

	void strrequest(std::vector<std::string> request);
	void getheaders(std::vector<std::string> request);
	void methodpath(std::string method, std::string path);
	void postheaders(std::vector<std::string> request);
	void postbody(std::string body_request);
	void post_fname_body(std::vector<std::string> request);

};

#endif