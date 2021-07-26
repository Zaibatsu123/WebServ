//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/26/21				 //
//                                   //

#include <iostream>

int main(){
	size_t bufSize = 1024;
	char* buf = new char[bufSize];
	while (std::fread(buf, 1, bufSize, stdin))
		std::cout << buf << std::endl;
	delete[] buf;
	return 0;
}