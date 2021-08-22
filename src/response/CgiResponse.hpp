//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#ifndef CGIRESPONSE_HPP
#define CGIRESPONSE_HPP

#include "AResponse.hpp"
#include "../../inc/output.hpp"

class CgiResponse : public AResponse {
public:
	CgiResponse();
	CgiResponse(const std::string & fileName);
	CgiResponse(const CgiResponse & rhi);
	CgiResponse & operator=(const CgiResponse & rhi);
	~CgiResponse();

	std::string	generateResponse();
	std::string	generateHeader();
	std::string	generateBody();
};


#endif //CGIRESPONSE_HPP
