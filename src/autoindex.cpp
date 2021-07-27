#include <dirent.h>
#include <iostream>
#include <sstream>
#include <fstream>

void autoindex(char *directory)
{
    DIR                 *dir;
    struct dirent       *de;
    std::stringstream   str;

    dir = opendir(directory);
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

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    autoindex(argv[1]);
    return (0);
}