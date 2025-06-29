/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ZappyAbstractGUI.cpp
*/

#include "../lib/ZappyAbstractGUI.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unistd.h>

ZappyAbstractGUI::ZappyAbstractGUI(const std::string& ip, int port)
    : network(ip, port), windowWidth(1280), windowHeight(720) {}

bool ZappyAbstractGUI::initialize() {
    if (!network.connectToServer()) {
        std::cerr << "Failed to connect to server" << std::endl;
        return false;
    }

    std::string welcome;
    int tries = 0;
    while (welcome.empty() && tries < 100) {
        welcome = network.receiveData();
        if (welcome.empty()) {
            usleep(10000);
            tries++;
        }
    }
    if (welcome.empty()) {
        std::cerr << "No welcome message from server." << std::endl;
        return false;
    }
    std::cout << "Server: " << welcome << std::endl;
    network.sendData("GRAPHIC\n");

    int map_w = 0, map_h = 0;
    bool got_size = false;
    tries = 0;
    while (!got_size && tries < 1000) {
        std::string data = network.receiveData();
        if (data.empty()) {
            usleep(10000);
            tries++;
            continue;
        }
        std::istringstream iss(data);
        std::string line;
        while (std::getline(iss, line)) {
            gameState.processServerMessage(line);
            if (line.rfind("msz", 0) == 0) {
                std::istringstream(line.substr(4)) >> map_w >> map_h;
                got_size = true;
            }
        }
    }
    if (!got_size) {
        std::cerr << "Timeout waiting for map size (msz)" << std::endl;
        return false;
    }
    int tiles_received = 0;
    tries = 0;
    while (tiles_received < map_w * map_h && tries < 1000) {
        std::string data = network.receiveData();
        if (data.empty()) {
            usleep(10000);
            tries++;
            continue;
        }
        std::istringstream iss(data);
        std::string line;
        while (std::getline(iss, line)) {
            gameState.processServerMessage(line);
            if (line.rfind("bct", 0) == 0)
                tiles_received++;
        }
    }
    if (tiles_received < map_w * map_h) {
        std::cerr << "Timeout waiting for all bct tiles" << std::endl;
        return false;
    }
    windowWidth = std::max(800, map_w * 64);
    windowHeight = std::max(600, map_h * 64);
    return true;
}

void ZappyAbstractGUI::updateFromServer() {
    std::string data = network.receiveData();
    if (!data.empty()) {
        if (data == "SERVER_SHUTDOWN\n") {
            std::cout << "Server closed the connection." << std::endl;
            closeWindow();
            return;
        }
        gameState.processServerMessage(data);
    }
}

void ZappyAbstractGUI::run() {
    if (!initialize())
        return;
    setupWindow();
    while (isWindowOpen()) {
        pollEvents();
        updateFromServer();
        renderFrame();
    }
    closeWindow();
}
