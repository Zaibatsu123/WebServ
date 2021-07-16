#include "../inc/output.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int rebind(int listen_socket) //устраняем залипание сокета после некорректного завершения работы сервака
{
	int opt = 1;

	if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int bind_socket(int listen_socket)
{
	sockaddr_in socket_info;
	char *address_name = new char[16];
	char ip_address[] = "127.0.0.1";

	strcpy(address_name, ip_address);
	socket_info.sin_family = AF_INET;
	socket_info.sin_port = htons(80);
	socket_info.sin_addr.s_addr = inet_addr(address_name);
	memset(socket_info.sin_zero, 0, 8);
	if (bind(listen_socket, reinterpret_cast<sockaddr *>(&socket_info), sizeof(sockaddr)) == -1)
	{
		std::cout << "Error when try to bind socket!" << strerror(errno) << std::endl;
		return (-1);
	}
	return (0);
}

int create_socket() //создаём сокет и прикрепляем к нему имя
{
	int listen_socket;

	if ((listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == EXIT_FAILURE)
    {
		std::cout << "Error when try to create socket!" << std::endl;
        return (EXIT_FAILURE);
    }
    if (rebind(listen_socket) == EXIT_FAILURE) //разлипание сокета перед его прикреплением
    {
        std::cout << "Error when try to rebind socket!" << std::endl;
		return (EXIT_FAILURE);
    }
	if (bind_socket(listen_socket) == EXIT_FAILURE)
    {
        std::cout << "Error when try to bind socket!" << std::endl;
		return (EXIT_FAILURE);
    }
    if (listen(listen_socket, 5) == EXIT_FAILURE)
    {
        std::cout << "Error when listen the socket" << std::endl;
        return (EXIT_FAILURE);
    }
	return (listen_socket);
}

int master_process(){
	char read_buffer[1024];
    std::string buffer;
    Config configuration;
    fd_set read_fds,write_fds;
	int new_client_socket;
	int result;
    int max_fd;
    t_client *new_client = NULL;

	if ((configuration.socket = create_socket()) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    fcntl(configuration.socket, F_SETFL, O_NONBLOCK);
	while (true)
	{
        std::cout << "Cycle started" << std::endl;
        max_fd = configuration.socket;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_SET(configuration.socket, &read_fds);

        for (std::list<t_client>::iterator i = configuration.clients.begin(); i != configuration.clients.end(); i++)
        {
            std::cout << "Adding to read/write sets fd:" << (*i).socket << std::endl;
            FD_SET((*i).socket, &read_fds);
            FD_SET((*i).socket, &write_fds);
        }
        result = select(max_fd + 1, &read_fds, &write_fds, NULL, NULL);
        std::cout << "After select:" << result << std::endl;
        if (FD_ISSET(configuration.socket, &read_fds))
        {
            if ((new_client_socket = accept(configuration.socket, NULL, NULL)) == -1)
            {
                std::cout << "Error when accept connection to the socket" << std::endl;
                return (EXIT_FAILURE);
            }
            new_client = new t_client;
            new_client->socket = new_client_socket;
            fcntl(new_client->socket, F_SETFL, O_NONBLOCK);
            if (new_client_socket > max_fd)
                max_fd = new_client_socket;
            configuration.clients.push_back(*new_client);
            std::cout << "Connected new client" << std::endl;
            new_client = NULL;
        }
        for (std::list<t_client>::iterator i = configuration.clients.begin(); i != configuration.clients.end(); i++)
        {
            std::cout << "Check for request fd:" << (*i).socket << FD_ISSET((*i).socket, &read_fds) << std::endl;
            if (FD_ISSET((*i).socket, &read_fds))
            {
                if ((result = recv((*i).socket, read_buffer, 1024, 0)) == -1)
			        std::cout << "Error when receiving  message! " << strerror(errno) << std::endl;
                std::cout << "Received request________________________" << std::endl;
                std::cout << read_buffer << std::endl;
                std::cout << "End request________________________" << std::endl;
                (*i).buffer = read_buffer;
            }
        }
        for (std::list<t_client>::iterator i = configuration.clients.begin(); i != configuration.clients.end(); i++)
        {
            std::cout << "Check ready for responce fd:" << (*i).socket << std::endl;
            if (FD_ISSET((*i).socket, &write_fds))
            {
                result = response((*i).socket, buffer);
                if (result == -1)
                    std::cerr << "send failed: " << strerror(errno) << "\n"; // произошла ошибка при отправке данных
                std::cout << "Sended responce" << std::endl;
                // close(configuration.fd[i]);
            }
        }
        std::cout << "Cycle ended" << std::endl;
	}
    return (EXIT_SUCCESS);
}