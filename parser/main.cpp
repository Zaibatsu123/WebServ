//                                          ┏┻━━┻┓        //
// Created by:                        ╭┓┏╮  ┃▕▏▕▏┃  ╭┓┏╮  //
//        Jolene Radioactive          ┃┗┛┃  ┃┏┳┳┓┃  ┃┗┛┃  //
//         on:                        ╰┳┳╯  ┃┗┻┻┛┃  ╰┳┳╯  //
//              07/21				   ┃┃ ┏━┻━━━━┻━┓ ┃┃   //
//                                     ┃╰━┫╭━━━━━━╮┣━╯┃   //
//                                     ╰━━┫┃╱╲╱╲╱╲┃┣━━╯   //

#include <iostream>
#include <vector>
#include "Request.hpp"

std::vector<std::string> getarray(std::string req);
Request start(std::string str_req);


int main(void)
{
	Request pars;
	std::string req = "";
	std::vector<std::string> request;

	while (true)
	{
		std::string str;
		getline(std::cin, str);
		if (str == "END")
			break;
		req += str + "\n";
	}
	if (req.empty())
		return (1);

	pars = start(req);

	std::cout << pars.getMethod() << pars.getPath() << pars.getProtocol() << pars.getHost() << "|||||"  << std::endl;
	std::cout << "CL: " << pars.getCLength() << "|\nCT: " << pars.getCType() << "|||||" << std::endl;
	std::cout << "ERR: " << pars.getErr() << std::endl;
	std::cout << "BODY: " << pars.getBody() << std::endl;

	return (0);
}

