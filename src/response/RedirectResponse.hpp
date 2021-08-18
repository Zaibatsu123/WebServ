//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/18/21				 //
//                                   //

#ifndef REDIRECTRESPONSE_HPP
#define REDIRECTRESPONSE_HPP

#include "AResponse.hpp"

class RedirectResponse : public AResponse {
public:
	std::string _redirect;
public:
	RedirectResponse();
	RedirectResponse(int status, const std::string &redirect);
	RedirectResponse(const RedirectResponse & rhi);
	RedirectResponse & operator=(const RedirectResponse & rhi);
	~RedirectResponse();

	std::string	generateResponse();
	std::string	generateHeader();
	std::string	generateBody();
};


#endif //REDIRECTRESPONSE_HPP
