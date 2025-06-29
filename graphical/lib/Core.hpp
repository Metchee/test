/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Core.hpp
*/

#pragma once
#include "Client_network.hpp"
#include "GameState.hpp"

class Core {
public:
    Core(const std::string& ip, int port);

    void initialize();
    void update();
    GameState& getGameState();

private:
    ClientNetwork network;
    GameState gameState;
};
