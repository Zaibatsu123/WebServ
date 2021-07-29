#include <dirent.h>
#include <iostream>
#include <sstream>
#include <fstream>

void autoindex(char *directory)
{
    DIR                 *dir = NULL;
    struct dirent       *de;
    std::stringstream   str;

    if ((dir = opendir(directory)) == NULL)
    {
        std::cout << strerror(errno) << std::endl;
        exit(1);
    }
    str << "<!DOCTYPE html><html><head><title>Вы находитесь в директории: " << directory << "</title></head><body><H1>Autoindex</H1>";
    while(dir)
    {
        de = readdir(dir);
        if (!de)
            break;
        str << "<p><a href=\"" << directory << "/" << de->d_name << "\">" << de->d_name << "</a></p>" << std::endl;
    }
    str << "</body></html>";

    std::ofstream file("answer.html");
    if (!file)
        std::cout << "File opening error!" << std::endl;
    file << str.str();
    closedir(dir);
}

int file_or_directory_existing(std::string path, Server *server)
{
	std::ifstream		file;
	DIR                 *dir = NULL;
    struct dirent       *de;
    std::stringstream   str;

	if (server->autoindex == 1)
	{
		file.open(path);
		if (file.is_open()){
			file.close();
			return (1);
		}
		else if (dir = opendir(path.c_str()))
		{
			closedir(dir);
			return (2);
		}
		else
	}
	else
	{

	}
}

// void autodirectory(char *directory)
// {
//     std::ifstream	srcFile;

// 	srcFile.open((_root + _fileName).c_str(), std::ifstream::in);

// 	if (!srcFile.is_open()){
// 		_status = 404;
// }

int main(int argc, char **argv)
{
    (void)argc;
    std::cout << "directory:" << argv[1] << std::endl;
    autoindex(argv[1]);
    return (0);
}