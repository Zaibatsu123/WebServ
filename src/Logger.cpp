//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/23/21				 //
//                                   //

#include "Logger.hpp"

Logger::Logger() :
	_fileName("logs.txt"),
	_logs("logs.txt", std::ios::trunc){
}

Logger::Logger(const std::string &fileName) :
	_fileName(fileName),
	_logs(fileName, std::ios::trunc){
}

Logger::Logger(const Logger &rhi){
	*this = rhi;
}

Logger &Logger::operator=(const Logger &rhi){
	if (this == &rhi)
		return *this;
	return *this;
}

Logger::~Logger(){
	_logs.close();
}

const std::ofstream &Logger::getLogs() const{
	return _logs;
}

void Logger::addMessage(const std::string &msg){
	_logs << msg;
}

std::ostream& operator<<(std::ostream& os, const Logger & ){
	return os;
}
