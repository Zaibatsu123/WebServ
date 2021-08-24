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

std::string trim_begin(std::string old_string)
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
		if (req.find("\n", index2 + 1) == std::string::npos && req.find("\n", index2 + 1) == std::string::npos)
			break ; 
		index2 = req.find("\n", index2 + 1);
		if (index1 > int(req.size()) || index2 - index1 > int(req.size()))
			break;
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

int count_str(std::string input_str, std::string str)
{
	int cnt = 0;
	size_t foundIndex = 0;

	std::string strFinal = input_str;
	if (input_str.find(str) == std::string::npos || alnumsymb(input_str))
		return(cnt);
	while (foundIndex != std::string::npos)
	{
		strFinal = strFinal.substr(foundIndex + 1);
		if (alnumsymb(strFinal))
			break;
		foundIndex = strFinal.find(str);
		cnt += 1;
	}
	return(cnt);
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
		if ((old_vector[i].compare(0, str.size(), str) == 0) && i + 1 < old_vector.size())
		{
			size_t begin = i + 1;
			for (size_t j = i + 1; j < old_vector.size(); ++j)
			{
				if (old_vector[j].size() >= str.size() && (old_vector[j].compare(0, str.size(), str)) == 0)
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

std::string content(std::string string, std::string boundary)
{
	std::string content;
	size_t pos1 = string.find("\15\12\15\12") + 4;
	size_t pos2 = string.find(boundary, pos1);
	if (pos1 == std::string::npos || pos2 == std::string::npos)
		return("");
	content = string.substr(pos1, pos2 - pos1 - 2);
	std::cout << "|" << content << "|" << std::endl;
	return(content);
}

std::string rslash_from_end(std::string string)
{
    if (string.size() <= 1)
        return (string);
    char symbol = '/';
    int i = string.size() - 1;
    while (i > 1)
    {
        if (string[i] != symbol)
            break;
        i--;
    }
    return (string.substr(0, i + 1));
}

std::string rduplicate_slashes(std::string string)
{
    std::string temp_string = rslash_from_end(string);
    if (temp_string.size() <= 1)
        return (temp_string);
    std::cout << "remove sup slashes" << temp_string << std::endl;
    std::string clear_str = "/";
    char temp = '/';
    for (unsigned int i = 1; i < temp_string.size(); i++)
        if (temp_string[i] != temp || temp_string[i-1] != temp )
            clear_str.push_back(temp_string[i]);
    return (clear_str);
}

std::vector<std::string> std_split(std::string str)
{
	std::vector<std::string> massive;
	std::string buffer = "";
	str += " ";
	for (int i = 0; i < (int)str.length();)
	{
		if (str[i] != ' ')
		{
			buffer += str[i];    
			i++;
		} 
		else
		{
			while (str[i] == ' ')
				i++;
			massive.push_back(buffer);
			buffer = "";
		}
	}
	// for (size_t j = 0; j < massive.size(); j++)
	// 	std::cout << "MAS|" << massive[j] << "|" << std::endl;
	return (massive);
}
