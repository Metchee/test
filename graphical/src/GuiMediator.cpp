/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** GuiMediator.cpp
*/

#include "../lib/GuiMediator.hpp"

GuiMediator::GuiMediator(const std::string& ip, int port)
    : core(ip, port)
{}

void GuiMediator::initialize() {
    core.initialize();
}

void GuiMediator::update() {
    core.update();
}

GameState& GuiMediator::getGameState() {
    return core.getGameState();
}
