# include <iostream>
#include <vector>
#include "Request.hpp"

int parser(Request *pars, std::vector<std::string> request)
{
	pars->strrequest(request);

	if (pars->getErr() != 0)
		return (1);	
	return (0);
}

int main(void)
{
	Request pars;
	std::string req = "";
	std::vector<std::string> request;

	while (true)
	{
		std::string str;
		getline(std::cin, str);
		if (str.empty() || !str.size())
			break;
		req += str + "\n";
	}
	request = pars.getarray(req);
	// pars.strrequest(request);
	parser(&pars, request);
	std::cout << pars.getMethod() << pars.getPath() << pars.getProtocol() << pars.getHost(); //<< std::endl;
	return (0);
}

