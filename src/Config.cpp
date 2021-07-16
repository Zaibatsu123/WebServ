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

int Config::getPort()
{
    return (htons(this->port));
}

int Config::getAddress()
{
    return (htonl(this->port));
}

int Config::readConfig()
{
    return (EXIT_SUCCESS);
}

Config::~Config()
{
    if (this->fd != NULL)
        delete this->fd;
}
