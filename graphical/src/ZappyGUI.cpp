/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Main GUI application class implementation
*/

#include "../lib/ZappyGUI.hpp"
#include "../lib/ZappyAbstractGUI.hpp"
#include <iostream>

ZappyGUI::ZappyGUI(const std::string& ip, int port)
    : ZappyAbstractGUI(ip, port), window(1980, 920, "Zappy GUI"), renderer(window.getWindow()), mediator(ip, port) {}

void ZappyGUI::setupWindow() {
}

bool ZappyGUI::isWindowOpen() {
    return window.isOpen();
}

void ZappyGUI::pollEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void ZappyGUI::renderFrame() {
    mediator.update();
    window.clear();
    renderer.render(mediator.getGameState());
    window.display();
}

void ZappyGUI::closeWindow() {
    window.close();
}

void ZappyGUI::run() {
    mediator.initialize();
    setupWindow();
    renderer.playBackgroundMusic("../graphical/asset/backgroundmusic.ogg");
    while (isWindowOpen()) {
        pollEvents();
        renderFrame();
    }
    closeWindow();
}
