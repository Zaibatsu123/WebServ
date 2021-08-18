#include "webserver.hpp"

Request *start(std::string str_req);

WebServer::WebServer()
{
    request_count = 0;
    __max_fd    =   0;
    __delay_client_disconnect_sec = 10;
}

void    WebServer::setServer(std::vector<Server*> *servers){
    this->__servers = servers;
}

void WebServer::announceServerSettings(){
	std::cout << COLOR_GREY << "Loading complete." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_GREEN << "Welcome to Equal Rights WebServer." << COLOR_DEFAULT << std::endl;
	std::cout << COLOR_RED << "WARNING: This is development server. Do not use it in a production deployment." << COLOR_DEFAULT << std::endl;
	std::cout << "Use a production WSGI server instead." << COLOR_DEFAULT << std::endl;
    for (std::vector<Server*>::iterator i = __servers->begin(); i != __servers->end(); i++)
    {
        for (std::vector<t_socket *>::iterator j = (*i)->sockets.begin(); j != (*i)->sockets.end(); j++)
        {
            std::cout << "Running on http://" << (*j)->address << ":" << (*j)->port << "/ (Press CTRL+C to quit)" << std::endl;
        }
    }
}

int rebind(int listen_socket) //устраняем залипание сокета после некорректного завершения работы сервака
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

int create_socket(t_socket *server_socket) //создаём сокет, устраняем залипание и прикрепляем к нему имя
{
    int created_socket = 0;
	if ((created_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
		std::cout << "Error when try to create socket!" << std::endl;
        return (-1);
    }
    if (rebind(created_socket) == EXIT_FAILURE) //разлипание сокета перед его прикреплением
    {
        std::cout << "Error when try to rebind socket! Socket number = " << created_socket << ", address = " << server_socket->address << ", port = " << server_socket->port << ", error: " << strerror(errno) << std::endl;
		return (-1);
    }
	if (bind_socket(server_socket, created_socket) == EXIT_FAILURE)
    {
        std::cout << "Error when try to bind socket! Socket number = " << created_socket << ", address = " << server_socket->address << ", port = " << server_socket->port << ", error: " << strerror(errno) << std::endl;
		return (-1);
    }
    if (listen(created_socket, 5) == EXIT_FAILURE)
    {
        std::cout << "Error when change socket mode to listen" << created_socket << ", address = " << server_socket->address << ", port = " << server_socket->port << ", error: " << strerror(errno) << std::endl;
        return (-1);
    }
	return (created_socket);
}

int WebServer::return_same_socket(t_socket *socket)
{
    for (std::list<t_socket *>::iterator i = __binded_sockets.begin(); i != __binded_sockets.end(); i++)
        if (socket->port == (*i)->socket && !strcmp(socket->address, (*i)->address))
            return ((*i)->socket);
    return (0);
}

void WebServer::bindServersToSockets()
{
    for (std::list<t_socket *>::iterator i = __binded_sockets.begin(); i != __binded_sockets.end(); i++){
        for (std::vector<Server*>::iterator j = __servers->begin(); j != __servers->end(); j++){
            for (std::list<t_socket *>::iterator k = __binded_sockets.begin(); k != __binded_sockets.end(); k++){
                if ((*i)->socket == (*k)->socket){
                    (*i)->servers.push_back(*j);
                }
            }
        }
    }
}

int WebServer::creatingSocketServers()
{
    int same_socket = 0;
    for (std::vector<Server*>::iterator i = __servers->begin(); i != __servers->end(); i++){
        for (std::vector<t_socket *>::iterator j = (*i)->sockets.begin(); j != (*i)->sockets.end(); j++){
            same_socket = this->return_same_socket(*j);
            if (!same_socket) {
                if (((*j)->socket = create_socket(*j)) == -1){
                    return (EXIT_FAILURE);
                }
                fcntl((*j)->socket, F_SETFL, O_NONBLOCK);
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
    logs << "        ---> Connected new client with socket number:|" << new_client->socket << "|" << std::endl;
    return (new_client);
}

int WebServer::connectingNewClients()
{
    int         new_client_socket = -1;

    logs << "    -->|CONNECTING CLIENTS|" << std::endl;
    for (std::list<t_socket*>::iterator i = __binded_sockets.begin(); i != __binded_sockets.end(); i++) {
        if (FD_ISSET((*i)->socket, &__read_fds)) {
            if ((new_client_socket = accept((*i)->socket, NULL, NULL)) == -1) {
                std::cout << "Error when accept connection to the socket. " << strerror(errno) << std::endl;
                return (EXIT_FAILURE);
            }
            __clients.push_back(initClient(new_client_socket, *i));
        }
    }
    return (EXIT_SUCCESS);
}

std::string readRequest(s_client* client, ssize_t *status){
	std::stringstream	str;
	ssize_t				result = 0;
	char				read_buffer[MiB];
	std::string			buffer;

	std::memset(read_buffer, 0, MiB);
	result = recv(client->socket, read_buffer, MiB, 0);
	*status = result;
	std::cout << "Bytes read:|" << result << "|" << std::endl;
	if (static_cast<int>(result) == -1)
	{
		perror("Something goes wrong, when receiving message");
		return "";
	}
	read_buffer[result] = '\0';
    std::cout << "LEN OF READED BYTES STRING:|" << strlen(read_buffer) << "|" << std::endl;
    int a = 0;
    while (a < result)
        buffer.push_back(read_buffer[a++]);
    std::cout << "A:|" << a << "|" << std::endl;
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
    if (client->request->getHost().size() == 0)
        return (client->parent->servers[def]);
    for (std::vector<Server*>::iterator i = client->parent->servers.begin(); i != client->parent->servers.end(); i++){
        if (checkServerName(client->request->getHost(), *i)){
            return (client->parent->servers[def]);
        }
        def++;
    }
    return (client->parent->servers[def]);
}

void WebServer::proccessRequestHead(std::list<t_client *>::iterator i)
{
    logs << "            ---> Read Head" << std::endl;
    size_t pos = (*i)->buffer.find("\15\12\15\12");
    if (pos == std::string::npos)
        return ;
    logs << "            ---> Got full head" << std::endl;
    request_count += 1;
    (*i)->head = (*i)->buffer.substr(0, pos + 3); // TODO: substr direct to start()
    logs << "================== Request HEAD =====================" << std::endl;
    logs << (*i)->head;
    logs << "================== Request HEAD =====================" << std::endl << std::endl;
    (*i)->request = start((*i)->head);
    if ((*i)->request->getMethod() == "POST" || (*i)->request->getMethod() == "PUT"){
        logs << "            ---> Request have body" << std::endl;
        (*i)->getRequestHead = 1;
        if ((*i)->request->getBoundary().empty())
            (*i)->needle = "0\15\12\15\12";
        else
            (*i)->needle = (*i)->request->getBoundary() + "--";
    }
    else{
        logs << "            ---> Request have not body" << std::endl;
        (*i)->getRequestHead = 0;
        (*i)->status = 1;
    }
    std::cout << "BODY_SIZE:|" << (*i)->body.size() << "|" << std::endl;
	if ((*i)->buffer.size() > pos + 3)
		(*i)->buffer = (*i)->buffer.substr(pos + 4);
	else
		(*i)->buffer.clear();
    (*i)->request->setServer(findDefaultServer(*i));
    (*i)->head.clear();
}

void    WebServer::proccessRequestBody(std::list<t_client *>::iterator i)
{
    logs << "            ---> Read Body" << std::endl;
    size_t pos = (*i)->buffer.find((*i)->needle);
    std::cout << "Content length:" << (*i)->request->getCLength() << "BODY SIZE: " << strlen((*i)->body.c_str()) << std::endl;
    if (pos == std::string::npos)
        return ;
    logs << "Got full body" << std::endl;
    (*i)->getRequestHead = 0;
    (*i)->status = 1;
    (*i)->body = (*i)->buffer.substr(0, pos + (*i)->needle.length());
    logs << "========================== Request BODY ==============================2" << std::endl;
    logs << (*i)->body;
    logs << "========================== Request BODY ==============================" << std::endl << std::endl;
    logs << "STATUS:|" << (*i)->request->getTransferCode() << "|" << std::endl;
    logs << "METHOD:|" << (*i)->request->getMethod() << "|" << std::endl;
    std::cout << "!!!" << (*i)->body.size() << std::endl;
    if ((*i)->request->getTransferCode() == "chunked")
        (*i)->request->body_chunk((*i)->body);
    else
        (*i)->request->postbody((*i)->body);
    if ((*i)->buffer.size() > pos + (*i)->needle.length())
        (*i)->buffer = (*i)->buffer.substr(pos + (*i)->needle.length() + 1);
    (*i)->body.clear();
}

std::list<t_client *>::iterator WebServer::clientDelete(std::list<t_client *>::iterator i)
{
    if ((*i)->request)
        delete (*i)->request;
    delete (*i);
    return (--__clients.erase(i));
}

int WebServer::checkIncomingRequests()
{
    std::string buffer;
    ssize_t     result = 0;
    std::ofstream outf;                                  // DELETE AFTER DEBUG
    outf.open( "output_request.txt", std::ios_base::app);    // DELETE AFTER DEBUG

    logs << "    -->|RECEIVING REQUESTS|" << std::endl;
    for (std::list<t_client *>::iterator i = __clients.begin(); i != __clients.end(); i++)
    {
        logs << "        ---> |Check for request socket with number:|" << (*i)->socket << "|, status:|" << FD_ISSET((*i)->socket, &__read_fds) << "|" << std::endl;
        if (FD_ISSET((*i)->socket, &__read_fds))
        {
			(*i)->buffer += readRequest(*i, &result);
            std::cout << "BUFFER_SIZE:|" << (*i)->buffer.size() << "|" << std::endl;
			if (result <= 0){
                logs << "RESULT:" << result << "Error occured when receive message from client!" << strerror(errno) << "Errno:" << errno << std::endl;
                close((*i)->socket);
                i = clientDelete(i);
                continue;
            }
            gettimeofday(&((*i)->time), NULL);
			if ((*i)->getRequestHead == 0)
                proccessRequestHead(i);
			if ((*i)->getRequestHead == 1)
                proccessRequestBody(i);
        }
    }
    outf.close();  // DELETE AFTER DEBUG
    return (EXIT_SUCCESS);
}

int WebServer::checkOutcomingResponces()
{
    ssize_t result = 0;
    int exit_status = EXIT_SUCCESS;
    logs << "    --> Check outcoming responses" <<std::endl;
    for (std::list<t_client *>::iterator i = __clients.begin(); i != __clients.end(); i++)
    {
        logs << "        ---> Check ready for responce socket number:" << "|" << (*i)->socket << "|" << std::endl;
		logs << "        ---> Checking FDSet: " << FD_ISSET((*i)->socket, &__write_fds) << " with status: " << (*i)->status << std::endl;
        if (FD_ISSET((*i)->socket, &__write_fds) && (*i)->status == 1)
        {
            result = response(*i, &logs);
            if (result <= 0)
            {
                close((*i)->socket);
                i = clientDelete(i);
                std::cerr << "send failed: " << strerror(errno) << std::endl;
                exit_status = EXIT_FAILURE;
				continue;
            }
            gettimeofday(&((*i)->time), NULL);
            (*i)->buffer.clear();
            logs << "            ----> Response sended for socket:|" << (*i)->socket << "|" << std::endl;
        }
    }
    return (exit_status);
}

int WebServer::addingSocketsToSets()
{
    int max_fd = -1;
    FD_ZERO(&__read_fds);
    FD_ZERO(&__write_fds);
    for (std::vector<Server*>::iterator i = __servers->begin(); i != __servers->end(); i++){
        for (std::vector<t_socket *>::iterator j = (*i)->sockets.begin(); j != (*i)->sockets.end(); j++)
        {
            FD_SET((*j)->socket, &__read_fds);
            if ((*j)->socket > max_fd)
                max_fd = (*j)->socket;
        }
    }
    for (std::list<t_client *>::iterator i = __clients.begin(); i != __clients.end(); i++){
        logs << "    --> Adding to read/write sets fd socket with number:|" << (*i)->socket << "|" << std::endl;
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
    for (std::list<t_client *>::iterator i = __clients.begin(); i != __clients.end(); i++)
    {
        delay = now_time.tv_sec - (*i)->time.tv_sec;
        if (delay > __delay_client_disconnect_sec && (*i)->time.tv_sec != -1)
        {
            std::cout << "    --> Delete client with delay = :|" << delay << "|" << std::endl;
            close((*i)->socket);
            i = clientDelete(i);
        }
    } 
}

int WebServer::startServer(){
    logs.open("logs", std::ios::trunc);
    struct timeval now_time;
    now_time.tv_sec = 0;
    now_time.tv_usec = 100000;
    if (creatingSocketServers() == EXIT_FAILURE)
        return (EXIT_FAILURE);
    bindServersToSockets();
	while (true)
	{
        logs << "-> Cycle started" << std::endl;
        if ((__max_fd = addingSocketsToSets()) == -1)
            std::cerr << "Something wrong when work with sets!" << std::endl;
        logs << "-> Waiting for select..." << std::endl;
        if (select(__max_fd + 1, &__read_fds, &__write_fds, NULL, &now_time) == -1)
            std::cerr << "Select error: " << strerror(errno) << std::endl;
//        std::cout << "*time out*" << std::endl;
        if (connectingNewClients() == EXIT_FAILURE)
            std::cerr << "Something wrong when new client accepting!" << std::endl;
        if (checkIncomingRequests() == EXIT_FAILURE)
            std::cerr << "Something wrong when request receiving!" << std::endl;
        if (checkOutcomingResponces() == EXIT_FAILURE)
            std::cerr << "Something wrong when responce sending!" << std::endl;
        deleteOldClients();
        logs << "-> Cycle ended" << std::endl;
        logs << "-> Count of incoming requests:|" << request_count << "|" << std::endl;
	}
    return (EXIT_SUCCESS);
}