//                                   //
// Created by:                       //
//              Equal rights team    //
//         on:                       //
//              7/07/21              //
//                                   //

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>
#include <sstream>

int rebind(int listen_socket) //устраняем залипание сокета после некорректного завершения работы сервака
{
    int opt = 1;

    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 
    return (0);
}

int bind_socket(int listen_socket)
{
    sockaddr_in socket_info;
    char *address_name = new char[16];
    char ip_address[] = "127.0.0.1";

    strcpy(address_name, ip_address);
    socket_info.sin_family = AF_INET;
    socket_info.sin_port = htons(9909);
    socket_info.sin_addr.s_addr = inet_addr(address_name);
    memset(socket_info.sin_zero, 0, 8);
    if (rebind(listen_socket) == -1) //разлипание сокета перед его прикреплением
        return (-1);
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

    if ((listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        std::cout << "Error when try to create socket!" << std::endl;
    if (bind_socket(listen_socket) == -1)
        return (-1);
    return (listen_socket);
}

int	main(void){
    char read_buffer[1024];
    std::string buffer;
    int created_socket;
    int client_socket;
    int result;
    
    if ((created_socket = create_socket()) == 1)
        return (-1);
    if (listen(created_socket, 5) == 1)
    {
        std::cout << "Error when listen the socket" << std::endl;
        return (-1);
    }
    if ((client_socket = accept(created_socket, NULL, NULL)) == -1)
    {
        std::cout << "Error when accept connection to the socket" << std::endl;
        return (-1);
    }
    if ((result = recv(client_socket, read_buffer, 1024, 0)) == -1)
        std::cout << "Error when receiving  message! " << strerror(errno) << std::endl;
    std::cout << "Received request" << std::endl;
    buffer = read_buffer;
    std::cout << buffer << std::endl;
	return 0;
}