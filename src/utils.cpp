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
#include "../inc/output.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Request.hpp"
#include "../inc/output.hpp"

std::string trim(std::string old_string)
{
	int i = 0;
	while (old_string[i] == ' ' || old_string[i] == '\t' || old_string[i] == 13)
		i++;
	int j = old_string.size() - 1;
	while (old_string[j] == ' ' || old_string[j] == '\t' || old_string[j] == 13)
		j--;
	std::string new_string = old_string.substr(i, j - i + 1);
	return (new_string);
}

std::string trim_end(std::string old_string)
{
	int j = old_string.size() - 1;
	while (old_string[j] == ' ' || old_string[j] == '\t' || old_string[j] == 13)
		j--;
	std::string new_string = old_string.substr(0, j + 1);
	return (new_string);
}

int alnumsymb(std::string str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] > 32 && str[i] < 127 )
			return (0);
	}
	return (1);
}	

std::vector<std::string> getarray(std::string req)
{
	std::vector<std::string> request;
	int index1 = 0;
	int index2 = -1;
	req = trim(req);
	if (req.empty() or alnumsymb(req))
	{
		throw std::exception();
	}
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
	std::cout  << str_req << str_req.length()<< std::endl;
	try 
	{
		vect_req = getarray(str_req);
	}
	catch (std::exception(&e))
	{
		request->setErr(400);
		return (request);
	}
	if (vect_req.size() == 0)
	{
		request->setErr(400);
		return (request);
	}
	request->strrequest(vect_req);
	std::cout << "\033[1;46m0\033[0m" << std::endl;
	return (request);
}


std::vector<std::string> getneedvector(std::vector<std::string> old_vector, size_t begin, size_t end)
{
	std::vector<std::string> new_vector;
	for (size_t i = begin; i < end; ++i)
		new_vector.push_back(old_vector[i]);
	return (new_vector);
}

std::vector<std::string> splitvector(std::vector<std::string> old_vector, std::string str)
{
	std::vector<std::string> new_vector;
	for (size_t i = 0; i < old_vector.size(); ++i)
	{
		if (old_vector[i] == str && i + 1 < old_vector.size())
		{
			size_t begin = i + 1;
			for (size_t j = i + 1; j < old_vector.size(); ++j)
			{
				if (old_vector[j] == str)
				{
					size_t end = j;
					new_vector = getneedvector(old_vector, begin, end);
					break;
				}
			}
			break;
		}
	}
	return (new_vector);
}

// std::string rslash_from_end(std::string string)
// {
//     if (string.size() <= 1)
//         return (string);
//     char symbol = '/';
//     int i = string.size() - 1;
//     while (i > 1)
//     {
//         if (string[i] != symbol)
//             break;
//         i--;
//     }
//     return (string.substr(0, i + 1));
// }

// std::string rduplicate_slashes(std::string string)
// {
//     std::string temp_string = rslash_from_end(string);
//     if (temp_string.size() <= 1)
//         return (temp_string);
//     std::cout << temp_string << std::endl;
//     std::string clear_str = "/";
//     char temp = '/';
//     for (unsigned int i = 1; i < temp_string.size(); i++)
//         if (temp_string[i] != temp || temp_string[i-1] != temp )
//             clear_str.push_back(temp_string[i]);
//     return (clear_str);
// }
