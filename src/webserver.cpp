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

int bind_socket(Config *configuration)
{
	sockaddr_in socket_info;

	socket_info.sin_family = AF_INET;
	socket_info.sin_port = htons(configuration->getPort());
	socket_info.sin_addr.s_addr = inet_addr(configuration->getAddress());
	memset(socket_info.sin_zero, 0, 8);
	if (bind(configuration->getSocket(), reinterpret_cast<sockaddr *>(&socket_info), sizeof(sockaddr)) == -1)
	{
		std::cout << "Error when try to bind socket!" << strerror(errno) << std::endl;
		return (-1);
	}
	return (0);
}

int create_socket(Config *configuration) //создаём сокет и прикрепляем к нему имя
{
    int created_socket;
	if ((created_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == EXIT_FAILURE)
    {
		std::cout << "Error when try to create socket!" << std::endl;
        return (EXIT_FAILURE);
    }
    configuration->setSocket(created_socket);
    if (rebind(configuration->getSocket()) == EXIT_FAILURE) //разлипание сокета перед его прикреплением
    {
        std::cout << "Error when try to rebind socket!" << std::endl;
		return (EXIT_FAILURE);
    }
	if (bind_socket(configuration) == EXIT_FAILURE)
    {
        std::cout << "Error when try to bind socket!" << std::endl;
		return (EXIT_FAILURE);
    }
    if (listen(configuration->getSocket(), 5) == EXIT_FAILURE)
    {
        std::cout << "Error when listen the socket" << std::endl;
        return (EXIT_FAILURE);
    }
	return (EXIT_SUCCESS);
}

int master_process(Config *configuration){
	char read_buffer[1024];
    std::string buffer;
    fd_set read_fds,write_fds;
	int new_client_socket;
	int result;
    int max_fd;
    t_client *new_client = NULL;

	if (create_socket(configuration) == EXIT_FAILURE)
		return (EXIT_FAILURE);
    fcntl(configuration->getSocket(), F_SETFL, O_NONBLOCK);
	while (true)
	{
        max_fd = configuration->getSocket();
        std::cout << "Cycle started" << max_fd << std::endl;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_SET(configuration->getSocket(), &read_fds);

        for (std::list<t_client>::iterator i = configuration->clients.begin(); i != configuration->clients.end(); i++)
        {
            std::cout << "Adding to read/write sets fd:" << (*i).socket << std::endl;
            FD_SET((*i).socket, &read_fds);
            FD_SET((*i).socket, &write_fds);
            if (new_client_socket > max_fd)
                max_fd = new_client_socket;
        }
        if ((result = select(max_fd + 1, &read_fds, NULL, NULL, NULL)) == -1)
            std::cout << "Select error: " << strerror(errno) << std::endl;
        std::cout << "After select: " << result << std::endl;
        if (FD_ISSET(configuration->getSocket(), &read_fds))
        {
            if ((new_client_socket = accept(configuration->getSocket(), NULL, NULL)) == -1)
            {
                std::cout << "Error when accept connection to the socket. " << strerror(errno) << std::endl;
                return (EXIT_FAILURE);
            }
            new_client = new t_client;
            new_client->socket = new_client_socket;
            fcntl(new_client->socket, F_SETFL, O_NONBLOCK);
            configuration->clients.push_back(*new_client);
            std::cout << "Connected new client" << std::endl;
            new_client = NULL;
        }
        for (std::list<t_client>::iterator i = configuration->clients.begin(); i != configuration->clients.end(); i++)
        {
            std::cout << "Check for request fd:" << (*i).socket << " status: " << FD_ISSET((*i).socket, &read_fds) << std::endl;
            if (FD_ISSET((*i).socket, &read_fds))
            {
                if ((result = recv((*i).socket, read_buffer, 1024, 0)) == -1)
			        std::cout << "Error when receiving  message! " << strerror(errno) << std::endl;
                std::cout << "Received request________________________" << std::endl;
                std::cout << read_buffer << std::endl;
                std::cout << "End request________________________" << std::endl;
                (*i).buffer = read_buffer;
                (*i).status = 1;
            }
        }
        for (std::list<t_client>::iterator i = configuration->clients.begin(); i != configuration->clients.end(); i++)
        {
            std::cout << "Check ready for responce fd:" << (*i).socket << std::endl;
            if (FD_ISSET((*i).socket, &write_fds) and (*i).status)
            {
                result = response((*i).socket, (*i).buffer);
                if (result == -1)
                    std::cerr << "send failed: " << strerror(errno) << "\n"; // произошла ошибка при отправке данных
                close((*i).socket);
                i = configuration->clients.erase(i);
                std::cout << "Sended responce" << std::endl;
                (*i).status = 0;
            }
        }
        std::cout << "Cycle ended" << std::endl;
	}
    return (EXIT_SUCCESS);
}