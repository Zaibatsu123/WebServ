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
    this->__address = NULL;
    this->__port = -1;
}

std::vector<std::string> Config::readFile(char *config_name)
{
    std::ifstream               config_file(config_name);
    std::string                 temp;
    std::vector<std::string>    configuration;

    std::cout << "Start configuration reading" << std::endl;
    if (!config_file)
    {
        std::cerr << "Configuration file not found!" << std::endl;
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

int Config::checkigConfiguration()
{
    if (!this->__address || this->__port == -1)
    {
        std::cerr << "Configuration file is not valid!" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

void    Config::configurationPrint()
{
    std::cout << "___________________________" << std::endl;
    std::cout << "PARSED CONFIG:" << std::endl;
    std::cout << "IP:" << "|" << this->__address << "|" << std::endl;
    std::cout << "PORT:" << "|" << this->__port << "|" << std::endl;
    std::cout << "___________________________" << std::endl;
}

int Config::parsingConfiguration(char *config_name)
{
    std::vector<std::string>    configuration = readFile(config_name);
    std::string                 next_string;

    if (configuration.size() == 0)
        return (EXIT_FAILURE);
    for (std::vector<std::string>::iterator iter = configuration.begin(); iter != configuration.end(); iter++)
    {
        if ((*iter).find("ip") != std::string::npos)
        {
            this->__address = new char[strlen((*iter).substr((*iter).find("ip") + 3).c_str()) + 1];
            memcpy(this->__address, (*iter).substr((*iter).find("ip") + 3).c_str(), strlen((*iter).substr((*iter).find("ip") + 3).c_str()));
            this->__address[strlen((*iter).substr((*iter).find("ip") + 3).c_str())] = '\0';
        }
        else if((*iter).find("port") != std::string::npos)
            this->__port = std::stoi((*iter).substr((*iter).find("port") + 5));
    }
    if (checkigConfiguration() == EXIT_FAILURE)
        return (EXIT_FAILURE);
    configurationPrint();
    return (EXIT_SUCCESS);
}

int Config::getPort()
{
    return (this->__port);
}

int Config::getSocket()
{
    return (this->__socket);
}

void Config::setSocket(int socket)
{
    this->__socket = socket;
}

char *Config::getAddress()
{
    return (this->__address);
}
