#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Config.hpp"

int rebind(int listen_socket);
int bind_socket(int listen_socket);
int create_socket();
int master_process(Config *configuration);

#endif