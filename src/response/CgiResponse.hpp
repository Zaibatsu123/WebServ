//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#ifndef CGIRESPONSE_HPP
#define CGIRESPONSE_HPP

#include "AResponse.hpp"

class CgiResponse : public AResponse {
public:
	CgiResponse();
	CgiResponse(const std::string & root, const std::string & fileName);
	~CgiResponse();

	std::string	generateResponse();
	std::string	generateHeader();
	std::string	generateBody();
};


#endif //CGIRESPONSE_HPP
