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
	AutoIndexResponse(const std::string & content);
	AutoIndexResponse(const AutoIndexResponse & rhi);
	AutoIndexResponse & operator=(const AutoIndexResponse & rhi);
	~AutoIndexResponse();

	std::string	generateResponse();
	std::string	generateHeader();
	std::string	generateBody();
};

#endif //AUTOINDEXRESPONSE_HPP
