/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** ZappyAbstractGUI.hpp
*/

#pragma once
#include "Client_network.hpp"
#include "GameState.hpp"
#include "IGUI.hpp"
#include <string>

class ZappyAbstractGUI : public IGUI {
public:
    ZappyAbstractGUI(const std::string& ip, int port);
    virtual ~ZappyAbstractGUI() = default;

    virtual void run() override;

protected:
    ClientNetwork network;
    GameState gameState;
    int windowWidth;
    int windowHeight;

    virtual void setupWindow() = 0;
    virtual bool isWindowOpen() = 0;
    virtual void pollEvents() = 0;
    virtual void renderFrame() = 0;
    virtual void closeWindow() = 0;

private:
    bool initialize();
    void updateFromServer();
};
