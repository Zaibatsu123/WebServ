//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/23/21				 //
//                                   //

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>

class Logger{
private:
	std::string		_fileName;
	std::ofstream	_logs;
public:
	Logger();
	Logger(const std::string & fileName);
	Logger(const Logger & rhi);
	Logger & operator=(const Logger & rhi);
	~Logger();

	std::ofstream &getLogs();
	void	addMessage(const std::string &msg);
	void	addMessage(const int &in);
	std::string timeStamp();
};

Logger &operator<<(Logger &log, int msg);
Logger &operator<<(Logger &log, const char *msg);
Logger &operator<<(Logger &log, std::string *msg);

#endif //LOGGER_HPP
