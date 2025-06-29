/*
** EPITECH PROJECT, 2025
** ZappyGUI
** File description:
** Main GUI application class
*/

#pragma once
#include "ZappyAbstractGUI.hpp"
#include "window.hpp"
#include "Renderer.hpp"
#include "GuiMediator.hpp"
#include "IGUI.hpp"

class ZappyGUI : public ZappyAbstractGUI {
public:
    ZappyGUI(const std::string& ip, int port);

    void run() override;

protected:
    void setupWindow();
    bool isWindowOpen();
    void pollEvents();
    void renderFrame();
    void closeWindow();

private:
    GameWindow window;
    Renderer renderer;
    GuiMediator mediator;
};
