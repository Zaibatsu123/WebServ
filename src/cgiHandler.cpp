//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/28/21				 //
//                                   //

#include "../inc/output.hpp"

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
	env[2] = (char *)"PATH_INFO=./root/info.php";
	env[3] = 0;
	return env;
}

void cgiChild(const std::string & cgiName) {

	char **env = generateCgiEnv();
	if (env == NULL){
		exit(2);
	}

	int in = open(CGI_INPUT_FILE, O_RDONLY, S_IRUSR | S_IWUSR);
	int out = open(CGI_OUTPUT_FILE, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

	if(in == -1 || out == -1)
		exit(2);
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		exit(2);
	execve(cgiName.c_str(), 0, env);
	exit(1);
}

std::string cgiParent(pid_t pid, Response* response){
	int					status;
	std::stringstream	str;

	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
		status = WEXITSTATUS(status);

	if (status == EXIT_SUCCESS){
		std::ifstream inputCGI(CGI_OUTPUT_FILE, std::ifstream::in);

		if (!inputCGI.is_open()){
			std::cout << "cannot write to outputMY" << std::endl;
			response->setStatus(500);
			return response->generateBody();
		}

		std::string buf;
		while (std::getline(inputCGI, buf))
			str << buf << std::endl;
	}
	else if (status == EXIT_FAILURE){
		std::cout << "execve error" << std::endl;
		response->setStatus(502);
		return response->generateBody();
	}
	else {
		std::cout << "child error" << std::endl;
		response->setStatus(500);
		return response->generateBody();
	}
	return str.str();
}



std::string cgi(const std::string & cgiName, Response* response){

	std::ofstream outMy(CGI_INPUT_FILE,std::ofstream::out);

	if (!outMy.is_open()){
		std::cout << "cannot write to outputMY" << std::endl;
		response->setStatus(500);
		return response->generateBody();
	}

	outMy << response->generateBody();
	outMy.close();

	pid_t pid = fork();
	if (pid == FAILURE){
		std::cout << "fork error" << std::endl;
		response->setStatus(500);
		return response->generateBody();
	}
	else if (pid == CHILD){
		cgiChild(cgiName);
	}
	return cgiParent(pid, response);
}