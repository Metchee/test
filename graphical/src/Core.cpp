#include "../lib/Core.hpp"
#include <sstream>
#include <unistd.h>
#include <iostream>

Core::Core(const std::string& ip, int port)
    : network(ip, port)
{}

void Core::initialize() {
    if (!network.connectToServer()) {
        throw std::runtime_error("Failed to connect to server");
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
        throw std::runtime_error("No welcome message from server.");
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
        throw std::runtime_error("Timeout waiting for map size (msz)");
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
        gameState.processServerMessage(data);
        tiles_received += gameState.getMapWidth() * gameState.getMapHeight();
    }
}

void Core::update() {
    std::string data = network.receiveData();
    if (!data.empty())
        gameState.processServerMessage(data);
}

GameState& Core::getGameState() {
    return gameState;
}
