//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/2/21				 //
//                                   //

#ifndef BADRESPONSE_HPP
#define BADRESPONSE_HPP

#include "AResponse.hpp"

class BadResponse : public AResponse {
public:
	BadResponse();
	BadResponse(int status, const std::string &errorPagePath);
	BadResponse(const BadResponse & rhi);
	BadResponse & operator=(const BadResponse & rhi);
	~BadResponse();

	std::string	generateResponse();
	std::string	generateHeader();
	std::string	generateBody();
};

#endif //BADRESPONSE_HPP
