//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/3/21				 //
//                                   //

#ifndef PUTRESPONSE_HPP
#define PUTRESPONSE_HPP

#include "AResponse.hpp"

class PutResponse : public AResponse {
public:
	PutResponse();
	PutResponse(long long maxContent, const std::string & root, const std::string & fileName);
	~PutResponse();

	std::string	generateResponse(int res);
	std::string	generateResponse();
	std::string	generateHeader(int status);
	std::string	generateHeader();
	std::string	generateBody();
};



#endif //PUTRESPONSE_HPP
