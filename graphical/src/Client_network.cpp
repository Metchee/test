/*
** EPITECH PROJECT, 2025
** client handler
** File description:
** zappy
*/

#include "../lib/Client_network.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

ClientNetwork::ClientNetwork(const std::string& ip, int port)
: m_ip(ip), m_port(port), m_socket(-1)
{

}

ClientNetwork::~ClientNetwork()
{
    if (m_socket != -1) {
        close(m_socket);
    }
}

bool ClientNetwork::connectToServer()
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        perror("Socket");
        return false;
    }
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ip.c_str(), &serverAddr.sin_addr);
    if (connect(m_socket, (sockaddr *)&serverAddr, sizeof(serverAddr)) != 0) {
        perror("Connect");
        return false;
    }
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return false;
    }
    if (fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        return false;
    }
    return true;
}


std::string ClientNetwork::receiveData()
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesReceived = recv(m_socket, buffer, sizeof(buffer) -1, 0);
    if (bytesReceived <= 0)
        return "";
    buffer[bytesReceived] = '\0';
    return std::string(buffer);
}

void ClientNetwork::sendData(const std::string& data)
{
    send(m_socket, data.c_str(), data.size(), 0);
}
