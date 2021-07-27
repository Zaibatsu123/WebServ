#include "../inc/output.hpp"
#include "Response.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "Server.hpp"

Request *start(std::string str_req);

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

int creating_socket_servers(std::vector<Server> *servers)
{
    for (std::vector<Server>::iterator i = servers->begin(); i != servers->end(); i++)
        for (std::vector<t_socket>::iterator j = (*i).sockets.begin(); j != (*i).sockets.end(); j++)
        {
            if ((j->socket = create_socket(&(*j))) == -1)
                return (EXIT_FAILURE);
            fcntl((*j).socket, F_SETFL, O_NONBLOCK);
        }
    return (EXIT_SUCCESS);
}

int connecting_new_clients(fd_set *read_fds, std::vector<Server> *servers, std::list<t_client> *clients)
{
    int         new_client_socket = -1;
    t_client    *new_client = NULL;

    for (std::vector<Server>::iterator i = servers->begin(); i != servers->end(); i++)
        for (std::vector<t_socket>::iterator j = (*i).sockets.begin(); j != (*i).sockets.end(); j++)
            if (FD_ISSET((*j).socket, read_fds))
            {
                if ((new_client_socket = accept((*j).socket, NULL, NULL)) == -1)
                {
                    std::cout << "Error when accept connection to the socket. " << strerror(errno) << std::endl;
                    return (EXIT_FAILURE);
                }
                new_client = new t_client;
                new_client->socket = new_client_socket;
                new_client->status = 0;
                new_client->request = NULL;
                fcntl(new_client->socket, F_SETFL, O_NONBLOCK);
                clients->push_back(*new_client);
                std::cout << "Connected new client" << std::endl;
                new_client = NULL;
            }
    return (EXIT_SUCCESS);
}

int check_incoming_requests(fd_set *read_fds, std::list<t_client> *clients)
{
	char        read_buffer[1024];
    std::string buffer;
    ssize_t     result = 0;

    for (std::list<t_client>::iterator i = clients->begin(); i != clients->end(); i++)
    {
        std::cout << "Check for request fd:" << (*i).socket << " status: " << FD_ISSET((*i).socket, read_fds) << std::endl;
        if (FD_ISSET((*i).socket, read_fds))
        {
            std::stringstream str;
            do {
                std::memset(read_buffer, 0, 1024);
                result = recv((*i).socket, read_buffer, 1024, 0);
                if (static_cast<int>(result) == -1)
                {
                    perror("Somthing goes wrong, when receiving message");
                    break;
                }
                std::cout << result << std::endl;
                str << read_buffer;
                if (result < 1024)
                    break;
            } while (result > 0);
            if (result > 0)
            {
                std::cout << "Received request________________________" << std::endl;
                std::cout << str.str() << std::endl;
                std::cout << "End request________________________" << std::endl;
                (*i).buffer = str.str();
                (*i).request = start((*i).buffer);
                (*i).status = 1;
            }
            else if (result <= 0)
            {
                i = clients->erase(i);
                std::cout << "Error occured when receive message from client!" << strerror(errno) << std::endl;
            }
        }
    }
    return (EXIT_SUCCESS);
}

int check_outcoming_responces(fd_set *write_fds, std::list<t_client> *clients)
{
    ssize_t result = 0;
    int exit_status = EXIT_SUCCESS;
    for (std::list<t_client>::iterator i = clients->begin(); i != clients->end(); i++)
    {
        std::cout << "Check ready for responce fd:" << (*i).socket << std::endl;
        if (FD_ISSET((*i).socket, write_fds))
        {
            result = response(&(*i));
            if (result == -1)
            {
                std::cerr << "send failed: " << strerror(errno) << std::endl;
                exit_status = EXIT_FAILURE;
            }
            // close((*i).socket);
            // i = clients->erase(i);
            (*i).status = 0;
            std::cout << "Sended responce" << std::endl;
        }
    }
    return (exit_status);
}

int adding_sockets_to_sets(std::vector<Server> *servers, std::list<t_client> *clients, fd_set *read_fds, fd_set *write_fds)
{
    int max_fd = -1;
    FD_ZERO(read_fds);
    FD_ZERO(write_fds);
    for (std::vector<Server>::iterator i = servers->begin(); i != servers->end(); i++)
        for (std::vector<t_socket>::iterator j = (*i).sockets.begin(); j != (*i).sockets.end(); j++)
        {
            FD_SET((*j).socket, read_fds);
            if ((*j).socket > max_fd)
                max_fd = (*j).socket;
        }
    for (std::list<t_client>::iterator i = clients->begin(); i != clients->end(); i++)
    {
        std::cout << "Adding to read/write sets fd:" << (*i).socket << std::endl;
        FD_SET((*i).socket, read_fds);
        if ((*i).status == 1)
            FD_SET((*i).socket, write_fds);
        if ((*i).socket > max_fd)
            max_fd = (*i).socket;
    }
    return (max_fd);
}

int master_process(std::vector<Server> *servers){
    int result = 0;
    int max_fd = 0;
    fd_set read_fds,write_fds;
    std::list<t_client> clients;

    if (creating_socket_servers(servers) == EXIT_FAILURE)
        return (EXIT_FAILURE);
	while (true)
	{
        std::cout << "Cycle started" << max_fd << std::endl;
        if ((max_fd = adding_sockets_to_sets(servers, &clients, &read_fds, &write_fds)) == -1)
            std::cout << "Something wrong when work with sets!" << std::endl;
        if ((result = select(max_fd + 1, &read_fds, NULL, NULL, NULL)) == -1)
            std::cout << "Select error: " << strerror(errno) << std::endl;
        std::cout << "After select:" << result << std::endl;
        if (connecting_new_clients(&read_fds, servers, &clients) == EXIT_FAILURE)
            std::cout << "Something wrong when new client accepting!" << std::endl;
        if (check_incoming_requests(&read_fds, &clients) == EXIT_FAILURE)
            std::cout << "Something wrong when request receiving!" << std::endl;
        if (check_outcoming_responces(&read_fds, &clients) == EXIT_FAILURE)
            std::cout << "Something wrong when responce sending!" << std::endl;
        std::cout << "Cycle ended" << std::endl;
	}
    return (EXIT_SUCCESS);
}