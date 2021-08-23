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
	std::fstream logs;
public:
	Logger();
	Logger(const Logger & rhi);
	Logger & operator=(const Logger & rhi);
	~Logger();
};


#endif //LOGGER_HPP
