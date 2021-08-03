//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/28/21				 //
//                                   //

#include "../../inc/output.hpp"

#define CHILD 0
#define FAILURE -1
#define CGI_INPUT_FILE "outputMy.txt"
#define CGI_OUTPUT_FILE "outputCGI.txt"

char** generateCgiEnv(){
	char **env;

	try
	{
		env = new char* [sizeof(char*) * 4];
	}
	catch (std::exception & e){
		return 0;
	}
	env[0] = (char *)"REQUEST_METHOD=get";
	env[1] = (char *)"SERVER_PROTOCOL=HTTP/1.1";
	env[2] = (char *)"PATH_INFO=./root/cgi_tester";
	env[3] = 0;
	return env;
}

void cgiChild(const std::string & cgiName, const std::string & pathToFile) {

	char **env = generateCgiEnv();
	if (env == NULL){
		exit(2);
	}

	int in = open(pathToFile.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
	int out = open(CGI_OUTPUT_FILE, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

	if(in == -1 || out == -1)
		exit(2);
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		exit(2);
	execve(cgiName.c_str(), 0, env);
	exit(1);
}

int cgiParent(pid_t pid){
	int					res;
	std::stringstream	str;

	waitpid(pid, &res, 0);

	if (WIFEXITED(res))
		res = WEXITSTATUS(res);

	if (res == EXIT_SUCCESS){
		std::ifstream inputCGI(CGI_OUTPUT_FILE, std::ifstream::in);

		if (!inputCGI.is_open()){
			std::cout << "cannot write to outputMY" << std::endl;
			return 500;
		}

		std::string buf;
		/*
		** Skip Cgi-Header
		*/
		while (std::getline(inputCGI, buf)){
			if (buf.length() == 0 ||  buf.at(0) == 13)
				break;
		}
		while (std::getline(inputCGI, buf))
			str << buf << std::endl;
		inputCGI.close();
	}
	else if (res == EXIT_FAILURE){
		std::cout << "execve error" << std::endl;
		return 502;
	}
	else {
		std::cout << "child error" << std::endl;
		return 500;
	}
	return 0;
}



int cgi(const std::string & cgiName, const std::string & pathToFile){

	std::ofstream outMy(CGI_INPUT_FILE,std::ofstream::out);

	if (!outMy.is_open()){
		std::cout << "cannot write to outputMY" << std::endl;
		return 500;
	}

	pid_t pid = fork();
	if (pid == FAILURE){
		std::cout << "fork error" << std::endl;
		return 500;
	}
	else if (pid == CHILD){
		cgiChild(cgiName, pathToFile);
	}
	return cgiParent(pid);
}