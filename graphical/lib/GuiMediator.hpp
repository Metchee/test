/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** GuiMediator.hpp
*/

#pragma once
#include "Core.hpp"

class GuiMediator {
public:
    GuiMediator(const std::string& ip, int port);

    void initialize();
    void update();
    GameState& getGameState();

private:
    Core core;
};
