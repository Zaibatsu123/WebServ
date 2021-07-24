//                                          ┏┻━━┻┓        //
// Created by:                        ╭┓┏╮  ┃▕▏▕▏┃  ╭┓┏╮  //
//        Jolene Radioactive          ┃┗┛┃  ┃┏┳┳┓┃  ┃┗┛┃  //
//         on:                        ╰┳┳╯  ┃┗┻┻┛┃  ╰┳┳╯  //
//              07/21				   ┃┃ ┏━┻━━━━┻━┓ ┃┃   //
//                                     ┃╰━┫╭━━━━━━╮┣━╯┃   //
//                                     ╰━━┫┃╱╲╱╲╱╲┃┣━━╯   //

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Request.hpp"

std::string trim(std::string old_string)
{
	int i = 0;
	while (old_string[i] == ' ' || old_string[i] == '\t' || old_string[i] == 10  || old_string[i] == 13)
		i++;
	int j = old_string.size() - 1;
	while (old_string[j] == ' ' || old_string[j] == '\t' || old_string[j] == 10  || old_string[j] == 13)
		j--;
	std::string new_string = old_string.substr(i, j - i + 1);
	return (new_string);
}

std::string trim_end(std::string old_string)
{
	int j = old_string.size() - 1;
	while (old_string[j] == ' ' || old_string[j] == '\t')
		j--;
	std::string new_string = old_string.substr(0, j + 1);
	return (new_string);
}

std::vector<std::string> getarray(std::string req)
{
	std::vector<std::string> request;
	int index1 = 0;
	int index2 = -1;
	size_t n = std::count(req.begin(), req.end(), '\n');
	std::string str;

	for (size_t i = 0; i < n; i++)
	{
		index2 = req.find("\n", index2 + 1);
		str = trim(req.substr(index1, index2 - index1));
		index1 = index2 + 1;
		request.push_back(str);
	}
	return (request);
}

Request *start(std::string str_req)
{
	Request *request = new Request;
	std::vector<std::string> vect_req;
	vect_req = getarray(str_req);
	request->strrequest(vect_req);
	return (request);
}
