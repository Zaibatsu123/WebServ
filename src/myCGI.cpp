//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/26/21				 //
//                                   //

#include <iostream>

int main(){
	std::string buf;
	while (std::getline(std::cin, buf))
		std::cout << buf << std::endl;
	return 0;
}