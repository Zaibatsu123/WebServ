//                                   //
// Created by:                       //
//              Watcher Jarr         //
//         on:                       //
//              7/28/21				 //
//                                   //

#include "../../inc/output.hpp"

#define CHILD 0
#define FAILURE -1

void cgiChild(const std::string & cgiName, const std::string & pathToFile, s_client* client) {

	char** env = generateEnv(client);
	if (env == NULL){
		exit(2);
	}

	int in = open(pathToFile.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
	int out = open(CGI_OUTPUT, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

    char *argv[2];
    argv[0] = (char*)cgiName.c_str();
    argv[1] = NULL;
	if(in == -1 || out == -1)
		exit(2);
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1)
		exit(2);
	execve(cgiName.c_str(), argv, env);
	exit(1);
}

int cgiParent(pid_t pid){
	int					res;
	std::stringstream	str;

	waitpid(pid, &res, 0);
	if (WIFEXITED(res))
		res = WEXITSTATUS(res);
	if (res == EXIT_SUCCESS){
		std::ifstream inputCGI(CGI_OUTPUT, std::ifstream::in);

		if (!inputCGI.is_open()){
			return 500;
		}

		std::string buf;
		buf.reserve(inputCGI.tellg());
		inputCGI.seekg(0, std::ifstream::beg);
		buf.assign(std::istreambuf_iterator<char>(inputCGI), std::istreambuf_iterator<char>());
		inputCGI.close();
	}
	else if (res == EXIT_FAILURE){
		std::remove(CGI_OUTPUT);
		return 502;
	}
	else {
		return 500;
	}
	return 0;
}

int cgi(const std::string & cgiName, const std::string & pathToFile, s_client *client){
	pid_t pid = fork();
	if (pid == FAILURE){
		return 500;
	}
	else if (pid == CHILD){
		cgiChild(cgiName, pathToFile, client);
	}
	return cgiParent(pid);
}


void	addToEnv(char **env, const std::string & newEnvValue){
	while(*env)
		env++;
	*env = strdup(newEnvValue.c_str());
}

int toUnderscore(int c){
	if (c == '-')
		return '_';
	return c;
}

std::string generateNewEnv(const std::string & key, const std::string & value){
	std::string buf(key);
	transform(buf.begin(), buf.end(), buf.begin(), ::toupper);
	transform(buf.begin(), buf.end(), buf.begin(), ::toUnderscore);
	buf = "HTTP_" + buf + "=" + value;
	return buf;
}

char **generateEnv(s_client* client){
	char **env;
	std::map<std::string , std::string> mymap = client->request->getHeaders_();

	try{
		env = new char*[mymap.size() + 4]();
	}
	catch (std::exception & e){
		return NULL;
	}

	addToEnv(env, "REQUEST_METHOD=" + client->request->getMethod());
	addToEnv(env, "SERVER_PROTOCOL=" + client->request->getProtocol());
	addToEnv(env, "PATH_INFO=" + client->request->getPath());

	std::map<std::string, std::string>::iterator it;
	for (it = mymap.begin(); it != mymap.end(); it++)
		addToEnv(env, generateNewEnv(it->first, it->second));

	return env;
}