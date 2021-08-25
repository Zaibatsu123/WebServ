#include "../../inc/output.hpp"

std::string utrim_end(std::string old_string, std::string elems)
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

t_location *get_location(std::string root, std::map<std::string, t_location *> *locations)
{
    std::string haystack = "no directory";
    std::map<std::string, t_location *>::iterator i;
    if (root.empty() || root.find("/") == std::string::npos)
        return (NULL);
    do
    {
        if ((i = locations->find(root)) != locations->end())
        {
            return (i->second);
        }
        root = rrepeats_from_end(alter_trim_end(root, "/"));
    }
    while (root.size() != 1);
    if ((i = locations->find(root)) != locations->end())
    {
        return (i->second);
    }
    return (NULL);
}

std::string autoindex(const char *directory, t_client *client)
{
	DIR                 *dir = NULL;
	struct dirent       *de;
	std::stringstream   str;
	(void)client;

	dir = opendir(directory);
	std::string prefix = "/";
	if (client->request->getPath()[client->request->getPath().size() - 1] == '/')
		prefix = "";
	str << "<!DOCTYPE html><html><head><title>Вы находитесь в директории: " << directory << "</title></head><body><H1>Autoindex</H1>";
	while(dir)
	{
		de = readdir(dir);
		if (!de)
			break;
		if (!strcmp(de->d_name, "."))
			continue;
		str << "<p><a href=\""  << client->request->getPath() << prefix << de->d_name << "\">" << de->d_name << "</a></p>" << std::endl;
	}
	str << "</body></html>";
	closedir(dir);
	return (str.str());
}

AResponse* file_or_directory_existing(t_client *client)
{
	std::ifstream		file;
    DIR                 *dir = NULL;
	t_location 			*location = NULL;
	std::string			path = client->request->getPath();

	if ((location = get_location(path, &client->request->getServer()->locations)) == NULL)
		return new BadResponse(404, client->request->getServer()->error_pages[404]);
	size_t pos = path.find(location->location);
	if (pos != std::string::npos && location->location != "/")
		path = path.substr(pos + location->location.length());
	std::string directory = location->root;
	std::string fullpath = directory + path;
    if ((dir = opendir(fullpath.c_str())) != NULL)
    {
        closedir(dir);
        if (location->index.size() != 0) {
            file.open(fullpath + "/" + location->index);
            if (file.is_open()) {
                return new GoodResponse(fullpath + "/" + location->index);
            }
        }
        if (location->autoindex == 1)
        {
            std::string str = autoindex(fullpath.c_str(), client);
            return new AutoIndexResponse(str);
        }
        return new BadResponse(404, client->request->getServer()->error_pages[404]);
    }
    file.open(fullpath);
    if (file.is_open())
    {
         return new GoodResponse(fullpath);
    }
    return new BadResponse(404, client->request->getServer()->error_pages[404]);
}
