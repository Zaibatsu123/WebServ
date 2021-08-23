//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              8/23/21				 //
//                                   //

#include "Logger.hpp"

Logger::Logger(){
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
}
