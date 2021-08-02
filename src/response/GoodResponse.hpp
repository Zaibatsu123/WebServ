//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/9/21				 //
//                                   //

#ifndef GOODRESPONSE_HPP
#define GOODRESPONSE_HPP

#include "AResponse.hpp"

class GoodResponse : public AResponse {
public:
	GoodResponse();
	GoodResponse(long long maxContent, const std::string & root, const std::string & fileName);
	~GoodResponse();

	std::string	generateResponse(int res);
	std::string	generateResponse();
	std::string	generateHeader(int status);
	std::string	generateHeader();
	std::string	generateBody();
};

#endif //GOODRESPONSE_HPP
