#include "webserver.hpp"
extern Logger logs;

#define CLIENT_DELAY_BEFORE_DELETING 0 // значение timeout, если в течении данного количества секунд не будет происходить никаких действий, то данный клиентский сокет будет закрыт (если == 0, то функция будет отключена)
#define DEBUG_MODE 0

WebServer::WebServer()
{
    this->request_count = 0;
    this->__max_fd    =   0;
    this->__delay_client_disconnect_sec = CLIENT_DELAY_BEFORE_DELETING;
    this->__select_delay_time.tv_sec = 100000;
    this->__select_delay_time.tv_usec = 0;
}

void    WebServer::setServer(std::vector<Server*> *servers) {this->__servers = servers;}

void    WebServer::announceServerSettings(){
    unsigned int k = 1;
	std::cout << COLOR_GREY << "Loading complete." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_GREEN << "Welcome to Equal Rights WebServer." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_RED << "WARNING: This is development server. Do not use it in a production deployment." << COLOR_DEFAULT << std::endl;
	std::cout << "Use a production WSGI server instead." << COLOR_DEFAULT << std::endl;
    for (std::vector<Server*>::iterator i = __servers->begin(); i != __servers->end(); i++, k++){
        std::cout << "_______________________________________________________" << std::endl;
        std::cout << "Server №" << k << " running on next host/ports" << std::endl;
        for (std::vector<t_socket *>::iterator j = (*i)->sockets.begin(); j != (*i)->sockets.end(); j++){
            std::cout << "Running on http://" << (*j)->address << ":" << (*j)->port  << std::endl;
        }
    }
    std::cout << "---------------> (Press CTRL+C to quit) <--------------" << std::endl;
}

int rebind(int listen_socket) //устраняем залипание сокета после некорректного завершения работы сервера
{
	int opt = 1;

	if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int bind_socket(t_socket *server_socket, int created_socket)
{
	sockaddr_in socket_info;

	socket_info.sin_family = AF_INET;
	socket_info.sin_port = htons(server_socket->port);
	socket_info.sin_addr.s_addr = inet_addr(server_socket->address);
	memset(socket_info.sin_zero, 0, 8);
	if (bind(created_socket, reinterpret_cast<sockaddr *>(&socket_info), sizeof(sockaddr)) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int create_socket(t_socket *server_socket) //создаём серверный сокет, устраняем залипание, биндим его, переводим в неблокирующий режим и оставляем слушать
{
    int created_socket = 0;
	if ((created_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){
		logs << logs.timeStamp().c_str() << " Error when try to create socket!\n";
        return (-1);
    }
    if (rebind(created_socket) == EXIT_FAILURE){ //разлипание сокета перед его прикреплением
        logs << logs.timeStamp().c_str() << " Error when try to rebind socket! Socket number = " << created_socket << ", address = " << server_socket->address << ", port = " << server_socket->port << ", error: " << strerror(errno) << "\n";
		return (-1);
    }
	if (bind_socket(server_socket, created_socket) == EXIT_FAILURE){
        logs << logs.timeStamp().c_str() << " Error when try to bind socket! Socket number = " << created_socket << ", address = " << server_socket->address << ", port = " << server_socket->port << ", error: " << strerror(errno) << "\n";
		return (-1);
    }
    if (listen(created_socket, 1024) == EXIT_FAILURE){ // второй параметр - доступная очередь сокетов к нашему серверному, если желающих будет больше, они просто автоматически дисконектятся
        logs << logs.timeStamp().c_str() << " Error when change socket mode to listen" << created_socket << ", address = " << server_socket->address << ", port = " << server_socket->port << ", error: " << strerror(errno) << "\n";
        return (-1);
    }
    if (fcntl(created_socket, F_SETFL, O_NONBLOCK == -1)){
        logs << logs.timeStamp().c_str() << " Error when try to change socket I/O mode to unblocking!" << "\n";
        return (-1);
    }
	return (created_socket);
}

int WebServer::returnSameSocket(t_socket *socket) 
{
    for (std::list<t_socket *>::iterator i = __binded_sockets.begin(); i != __binded_sockets.end(); i++) {
        if (socket->port == (*i)->port && !strcmp(socket->address, (*i)->address))
            return ((*i)->socket);
    }
    return (0);
}

void WebServer::bindServersToSockets()
{
    for (std::list<t_socket *>::iterator i = __binded_sockets.begin(); i != __binded_sockets.end(); i++){
        for (std::vector<Server*>::iterator j = __servers->begin(); j != __servers->end(); j++){
            for (std::vector<t_socket *>::iterator k = (*j)->sockets.begin(); k != (*j)->sockets.end(); k++){
                if ((*i)->socket == (*k)->socket){
                    (*i)->servers.push_back(*j);
                }
            }
        }
    }
}

int WebServer::checkSameSocketOnServers()
{
    std::vector<std::string> socket_list;
    std::string temp;
    for (std::vector<Server*>::iterator i = __servers->begin(); i != __servers->end(); i++){
        for (std::vector<t_socket *>::iterator j = (*i)->sockets.begin(); j != (*i)->sockets.end(); j++){
            temp = (*j)->address;
            temp += ":";
            temp += std::to_string((*j)->port);
            for (std::vector<std::string>::iterator k = socket_list.begin(); k != socket_list.end(); k++){
                if (temp == (*k)){
                    return (EXIT_FAILURE);
                }
            }
            socket_list.push_back(temp);
        }
        socket_list.clear();
    }
    return (EXIT_SUCCESS);
}



int WebServer::creatingSocketServers()
{
    int same_socket = 0;
    for (std::vector<Server*>::iterator i = __servers->begin(); i != __servers->end(); i++){
        for (std::vector<t_socket *>::iterator j = (*i)->sockets.begin(); j != (*i)->sockets.end(); j++){
            same_socket = this->returnSameSocket(*j);
            if (!same_socket) {
                if (((*j)->socket = create_socket(*j)) == -1){
                    return (EXIT_FAILURE);
                }
                __binded_sockets.push_back(*j);
            }
            else
                (*j)->socket = same_socket;
        }
    }
    return (EXIT_SUCCESS);
}

t_client    *WebServer::initClient(int new_client_socket, t_socket *parent)
{
    t_client    *new_client = NULL;
    new_client = new t_client;
    new_client->socket = new_client_socket;
    new_client->status = 0;
    new_client->getRequestHead = 0;
    new_client->responseNotSend = false;
    new_client->request = NULL;
    new_client->time.tv_sec = -1;
    new_client->time.tv_usec = -1;
    new_client->parent = parent;
    fcntl(new_client->socket, F_SETFL, O_NONBLOCK);
    return (new_client);
}

int WebServer::connectingNewClients()
{
    int         new_client_socket = -1;

    for (std::list<t_socket*>::iterator i = __binded_sockets.begin(); i != __binded_sockets.end(); i++) {
        if (FD_ISSET((*i)->socket, &__read_fds)) {
            if ((new_client_socket = accept((*i)->socket, NULL, NULL)) == -1) {
                logs << logs.timeStamp().c_str() << " Error occured, when trying accepting new client:" << strerror(errno) << "\n";
                return (EXIT_FAILURE);
            }
            logs << logs.timeStamp().c_str() << " New client succesfuly connected." << "\n";
            __clients.push_back(initClient(new_client_socket, *i));
        }
    }
    return (EXIT_SUCCESS);
}

std::string readRequest(s_client* client, ssize_t *status){
	std::stringstream	str;
	ssize_t				result = 0;
	char		        read_buffer[MiB];
	std::string			buffer;

	std::memset(read_buffer, 0, MiB);
	result = recv(client->socket, read_buffer, MiB, 0);
	*status = result;
	if (std::string(read_buffer) == "\xFF\xF4\xFF\xFD\x06"){
		*status = 0;
		logs << logs.timeStamp().c_str() << " Peer close connection" << "\n";
		return "";
	}
	if (static_cast<int>(result) == -1){
		logs << logs.timeStamp().c_str() << " Something goes wrong, when receiving message" << "\n";
		return "";
	}
	read_buffer[result] = '\0';
    int a = 0;
    while (a < result)
        buffer.push_back(read_buffer[a++]);
	return buffer;
}

int WebServer::checkServerName(std::string host, Server* server){
    for (std::vector<std::string>::iterator i = server->server_name.begin(); i != server->server_name.end(); i++){
        if (host == *i){
            return (1);
        }
    }
    return (0);
}

Server  *WebServer::findDefaultServer(t_client *client)
{
    int def = 0;
    if (client->request->getHost().size() == 0){
        return (client->parent->servers[0]);
    }
    for (std::vector<Server*>::iterator i = client->parent->servers.begin(); i != client->parent->servers.end(); i++) {
        if (checkServerName(client->request->getHost(), *i)){
            return (client->parent->servers[def]);
        }
        def++;
    }
    return (client->parent->servers[0]);
}

void WebServer::proccessRequestHead(std::list<t_client *>::iterator i)
{
    size_t pos = (*i)->buffer.find("\15\12\15\12");
    if (pos == std::string::npos)
        return ;

    request_count += 1;

    (*i)->head = (*i)->buffer.substr(0, pos + 3);
    (*i)->request = start((*i)->head);
    if ((*i)->request->getMethod() == "POST" || (*i)->request->getMethod() == "PUT") {
        (*i)->getRequestHead = 1;
        if ((*i)->request->getHeaders_().find("transfer-encoding")->second == "chunked")// != (*i)->request->getHeaders_().end())
            (*i)->needle = "0\15\12\15\12";
        else if ((*i)->request->getBoundary().length() > 0)
            (*i)->needle = (*i)->request->getBoundary() + "--";
        else
            (*i)->needle = "";
    }
    else {
        (*i)->getRequestHead = 0;
        (*i)->status = 1;
    }
	if ((*i)->buffer.size() > pos + 3)
		(*i)->buffer = (*i)->buffer.substr(pos + 4);
	else
		(*i)->buffer.clear();
    (*i)->request->setServer(findDefaultServer(*i));
    (*i)->head.clear();
}

void    WebServer::proccessRequestBody(std::list<t_client *>::iterator i)
{
    size_t pos = (*i)->buffer.find((*i)->needle);

    try{
		if ((*i)->needle.size() != 0){
			if (pos == std::string::npos)
				return ;
			(*i)->body = (*i)->buffer.substr(0, pos + (*i)->needle.length());
		}
		else{
			unsigned long size = std::stoul((*i)->request->getHeaders_()["content-length"]);
			if ((*i)->buffer.length() < size)
				return;
			(*i)->body = (*i)->buffer.substr(0, size);
		}
    }
    catch (std::exception & e){
    }

    (*i)->getRequestHead = 0;
    (*i)->status = 1;

	unsigned long flag = 0;
	try {
		flag = std::stoi((*i)->request->getHeaders_()["content-length"]);
	}
	catch (std::exception & e){
		flag = 0;
	}
    if ((*i)->request->getTransferCode() == "chunked")
        (*i)->request->body_chunk((*i)->body);
    else if (flag == (*i)->body.length()){
        (*i)->request->simplebody((*i)->body);
    }
    else
        (*i)->request->postbody((*i)->body);
    if ((*i)->buffer.size() > pos + (*i)->needle.length())
        (*i)->buffer = (*i)->buffer.substr(pos + (*i)->needle.length() + 1);
    (*i)->body.clear();
}

std::list<t_client *>::iterator WebServer::clientDelete(std::list<t_client *>::iterator i)
{
    close((*i)->socket);
    if ((*i)->request)
        delete (*i)->request;
    delete (*i);
    return (__clients.erase(i));
}

int WebServer::checkIncomingRequests()
{
    ssize_t     result = 0;

    for (std::list<t_client *>::iterator i = __clients.begin(); i != __clients.end(); ){
        if (FD_ISSET((*i)->socket, &__read_fds)){
			(*i)->buffer += readRequest(*i, &result);
			if (result <= 0){
                i = clientDelete(i);
                continue;
            }
            #if (CLIENT_DELAY_BEFORE_DELETING != 0)
            gettimeofday(&((*i)->time), NULL);
            #endif
			if ((*i)->getRequestHead == 0)
                proccessRequestHead(i);
			if ((*i)->getRequestHead == 1)
                proccessRequestBody(i);
        }
        i++;
    }
    return (EXIT_SUCCESS);
}

int WebServer::checkOutcomingResponces()
{
    ssize_t result = 0;

    int exit_status = EXIT_SUCCESS;
    for (std::list<t_client *>::iterator i = __clients.begin(); i != __clients.end();){
        if (FD_ISSET((*i)->socket, &__write_fds) && (*i)->status == 1){
            result = response(*i);
            if (result <= 0)
            {
                i = clientDelete(i);
                exit_status = EXIT_FAILURE;
				continue;
            }
            #if (CLIENT_DELAY_BEFORE_DELETING != 0)
            gettimeofday(&((*i)->time), NULL);
            #endif
            (*i)->buffer.clear();
        }
        i++;
    }
    return (exit_status);
}

int WebServer::addingSocketsToSets()
{
    int max_fd = -1;
    FD_ZERO(&__read_fds);
    FD_ZERO(&__write_fds);
    for (std::vector<Server*>::iterator i = __servers->begin(); i != __servers->end(); i++){
        for (std::vector<t_socket *>::iterator j = (*i)->sockets.begin(); j != (*i)->sockets.end(); j++){
            FD_SET((*j)->socket, &__read_fds);
            if ((*j)->socket > max_fd)
                max_fd = (*j)->socket;
        }
    }
    for (std::list<t_client *>::iterator i = __clients.begin(); i != __clients.end(); i++){
        FD_SET((*i)->socket, &__read_fds);
        if ((*i)->status == 1)
            FD_SET((*i)->socket, &__write_fds);
        if ((*i)->socket > max_fd)
            max_fd = (*i)->socket;
    }
    return (max_fd);
}

void WebServer::deleteOldClients()
{
    struct timeval now_time;
    int delay = 0;

    gettimeofday(&now_time, NULL);
    for (std::list<t_client *>::iterator i = __clients.begin(); i != __clients.end(); ){
        delay = now_time.tv_sec - (*i)->time.tv_sec;
        if (delay > static_cast<int>(__delay_client_disconnect_sec) && (*i)->time.tv_sec != -1){
            i = clientDelete(i);
            continue;
        }
        i++;
    }
}

int WebServer::startServer(){
    if (checkSameSocketOnServers() == EXIT_FAILURE) {
        logs << logs.timeStamp().c_str() << " Critical error! Same sockets for one server!\n";
        return (EXIT_FAILURE);
    }
    if (creatingSocketServers() == EXIT_FAILURE)
        return (EXIT_FAILURE);
    logs << logs.timeStamp().c_str() << " All server sockets created\n";
    bindServersToSockets();
    logs << logs.timeStamp().c_str() << " Webserver start working\n";
    this->announceServerSettings();
	while (true)
	{
        if ((__max_fd = addingSocketsToSets()) == -1)
            logs << logs.timeStamp().c_str() << " Error when trying add sockets to sets:" << strerror(errno) << "\n";
        if (select(__max_fd + 1, &__read_fds, &__write_fds, NULL, &this->__select_delay_time) == -1)
            logs << logs.timeStamp().c_str() << " Select error:" << strerror(errno) << "\n";
        if (connectingNewClients() == EXIT_FAILURE)
            logs << logs.timeStamp().c_str() << " Something wrong when new client accepting:" << strerror(errno) << "\n";
        if (checkIncomingRequests() == EXIT_FAILURE)
            logs << logs.timeStamp().c_str() << " Something wrong when request receiving:" << strerror(errno) << "\n";
        if (checkOutcomingResponces() == EXIT_FAILURE)
            logs << logs.timeStamp().c_str() << " Something wrong when responce sending:" << strerror(errno) << "\n";
        #if (CLIENT_DELAY_BEFORE_DELETING != 0)
        deleteOldClients();
        #endif
	}
    return (EXIT_SUCCESS);
}