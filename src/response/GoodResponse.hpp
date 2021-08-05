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
	GoodResponse(const std::string & root, const std::string & fileName);
	GoodResponse(const GoodResponse & rhi);
	GoodResponse & operator=(const GoodResponse & rhi);
	~GoodResponse();

	std::string	generateResponse();
	std::string	generateHeader();
	std::string	generateBody();
};

#endif //GOODRESPONSE_HPP
