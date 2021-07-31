#include "Server.hpp"

#include <string>
#include <iostream>

std::string trim_end(std::string old_string, std::string elems)
{
    if (old_string.size() == 0)
        return old_string;
	int j = old_string.size() - 1;
	while (j >= 0)
    {
        if (elems.find(old_string[j]) != std::string::npos)
		    j--;
        else
            break;
    }
	std::string new_string = old_string.substr(0, j + 1);
	return (new_string);
}

std::string alter_trim_end(std::string old_string, std::string elems)
{
    if (old_string.size() == 0)
        return old_string;
	int j = old_string.size() - 1;
	while (j >= 0)
    {
        if (elems.find(old_string[j]) != std::string::npos)
		    break;
        else
            j--;
    }
	std::string new_string = old_string.substr(0, j + 1);
	return (new_string);
}

std::string rrepeats_from_end(std::string string)
{
    if (string.size() <= 1)
        return (string);
    char symbol = string[string.size() - 1];
    int i = string.size() - 1;
    while (i > 1)
    {
        if (string[i] != symbol)
            break;
        i--;
    }
    return (string.substr(0, i + 1));
}


int is_symbol(std::string string, std::string symbols)
{
    for (unsigned int i = 0; i < string.size(); i++)
        if (symbols.find(string[i]) == std::string::npos)
            return (0);
    return (1);
}

std::string get_location(std::string root, std::map<std::string, t_location *> *locations)
{
    std::string needle = is_symbol(root, "/") ? rrepeats_from_end(root) : trim_end(root, "/");
    std::string haystack = "no directory";
    std::map<std::string, t_location *>::iterator i;
    std::cout << "1 needle:|" << needle << "|" << std::endl;
    if (root.size() == 0 || root.find("/") == std::string::npos)
        return (haystack);
    do
    {
        if ((i = locations->find(needle)) != locations->end())
            return (i->second->location);
        needle = rrepeats_from_end(alter_trim_end(needle, "/"));
        std::cout << "2 needle:|" << needle << "|" << std::endl;
    }
    while (needle.size() != 1);
    if ((i = locations->find(needle)) != locations->end())
        return (i->second->location);
    std::cout << haystack << std::endl;
    return (haystack);
}

std::string rduplicate_slashes(std::string string)
{
    if (string.size() <= 1)
        return (string);
    char symbol = string[string.size() - 1];
    int i = string.size() - 1;
    while (i > 1)
    {
        if (string[i] != symbol)
            break;
        i--;
    }
    return (string.substr(0, i + 1));
}

int main(int argc, char **argv)
{
    Server          server;
    t_location      *location;

    if (argc < 2)
        return (1);
    (void)argv;
    location = new t_location;
    location->location = "/projects";
    server.locations.insert(std::make_pair("/", location));
    location = new t_location;
    location->location = "/images";
    server.locations.insert(std::make_pair("/images", location));
    location = new t_location;
    location->location = "/images/secret";
    server.locations.insert(std::make_pair("/images/poop", location));
    location = new t_location;
    location->location = "/FBI";
    server.locations.insert(std::make_pair("/images/poop/secret", location));

    std::cout << get_location(argv[1], &(server.locations)) << std::endl;
    return (0);
}