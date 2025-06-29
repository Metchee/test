/*
** EPITECH PROJECT, 2025
** client handler
** File description:
** zappy
*/

#pragma once
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <queue>

class ClientNetwork {
    public :
        ClientNetwork(const std::string& ip, int port);
        ~ClientNetwork();
        bool connectToServer();
        std::string receiveData();
        void sendData(const std::string& data);
    private :
        int m_socket;
        std::string m_ip;
        int m_port;
};
