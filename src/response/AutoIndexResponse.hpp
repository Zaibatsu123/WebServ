//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#ifndef AUTOINDEXRESPONSE_HPP
#define AUTOINDEXRESPONSE_HPP

#include "AResponse.hpp"

class AutoIndexResponse : public AResponse {
public:
	AutoIndexResponse();
	AutoIndexResponse(long long maxContent, const std::string & root, const std::string & fileName);
	~AutoIndexResponse();

	std::string	generateResponse(int res);
	std::string	generateResponse();
	std::string	generateHeader(int status);
	std::string	generateHeader();
	std::string	generateBody();
};

#endif //AUTOINDEXRESPONSE_HPP
