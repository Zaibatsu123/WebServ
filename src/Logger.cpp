//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/23/21				 //
//                                   //

#include "Logger.hpp"
#include <sstream>

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

std::ofstream &Logger::getLogs(){
	return _logs;
}

std::string Logger::timeStamp(){
	time_t time;
	std::time(&time);
	std::stringstream str;
	std::string timeStamp(std::asctime(std::localtime(&time)));
	timeStamp.pop_back();
	str << "[" << timeStamp << "]";
	return str.str();
}

void Logger::addMessage(const std::string &msg){
	_logs << msg;
	_logs.flush();
}

void Logger::addMessage(const int &in){
	_logs << in;
	_logs.flush();
}

Logger &operator<<(Logger &log, int msg)
{
	log.addMessage(msg);
	return log;
}

Logger &operator<<(Logger &log, const char *msg)
{
	log.addMessage(msg);
	return log;
}

Logger &operator<<(Logger &log, const std::string *msg)
{
	log.addMessage(msg->c_str());
	return log;
}