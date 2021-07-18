#include "Config.hpp"

Config::ConfigException::ConfigException(std::string error)
{
    this->__error = error;
}

const char* Config::ConfigException::what() const throw()
{
    return (this->__error.c_str());
}

Config::Config()
{
    if ((this->fd = new int[1024]) == NULL)
    {
        this->~Config();
        throw ConfigException("Malloc error");
    }
    memset(this->fd, 0, 1024);
}

std::vector<std::string> readFile(char *config_name)
{
    std::ifstream               config_file(config_name);
    std::string                 temp;
    std::vector<std::string>    configuration;

    std::cout << "Start config reading" << std::endl;
    if (!config_file)
    {
        std::cerr << "Configuration file not found!"<< std::endl;
        return (configuration);
    }
    while (config_file)
    {
        std::getline(config_file, temp);
        if (config_file.eof())
        {
            configuration.push_back(temp + '\0');
            break;
        }
        else
            configuration.push_back(temp + '\0');
    }
    config_file.close();
    return (configuration);
}

int Config::parsingConfiguration(char *config_name)
{
    std::vector<std::string>    configuration = readFile(config_name);
    std::string                 next_string;

    if (configuration.size() == 0)
    {
        std::cout << "Configuration file is empty!" << std::endl;
        return (EXIT_FAILURE);
    }
    for (std::vector<std::string>::iterator iter = configuration.begin(); iter != configuration.end(); iter++)
    {
        std::cout << (*iter) << "|" << std::endl;
        if ((*iter).find("ip") != std::string::npos)
        {
            this->address = new char[strlen((*iter).substr((*iter).find("ip") + 3).c_str()) + 1];
            memcpy(this->address, (*iter).substr((*iter).find("ip") + 3).c_str(), strlen((*iter).substr((*iter).find("ip") + 3).c_str()));
            this->address[strlen((*iter).substr((*iter).find("ip") + 3).c_str())] = '\0';
            std::cout << "strlen:|" << strlen((*iter).substr((*iter).find("ip") + 3).c_str()) << "|" << std::endl;
        }
        else if((*iter).find("port") != std::string::npos)
        {
            this->port = std::stoi((*iter).substr((*iter).find("port") + 5));
            std::cout << this->port << std::endl;
        }
    }
    std::cout << "PARSED CONFIG:" << std::endl;
    std::cout << "IP:" << this->getAddress() << "len" << strlen(this->getAddress()) << std::endl;
    std::cout << "PORT:" << this->getPort() << std::endl;
    std::cout << "___________________________" << std::endl;
    return (EXIT_SUCCESS);
}

int Config::getPort()
{
    return (this->port);
}

char *Config::getAddress()
{
    return (this->address);
}

Config::~Config()
{
    if (this->fd != NULL)
        delete this->fd;
}
