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

	const	std::ofstream &getLogs() const;
	void	addMessage(const std::string & msg);
};

std::ostream& operator<<(std::ostream& os, const Logger & logs);

#endif //LOGGER_HPP
