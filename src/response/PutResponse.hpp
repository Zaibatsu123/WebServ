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
	PutResponse(const PutResponse & rhi);
	PutResponse & operator=(const PutResponse & rhi);
	~PutResponse();

	PutResponse(int status);

	std::string	generateResponse();
	std::string	generateHeader();
	std::string	generateBody();
};



#endif //PUTRESPONSE_HPP
