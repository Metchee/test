/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Zappy3DRenderer
*/

#pragma once
#include "window.hpp"
#include "ZappyAbstractGUI.hpp"
#include "GuiMediator.hpp"
#include <raylib.h>
#include <string>
#include <vector>
#include <map>

class Zappy3DRenderer : public ZappyAbstractGUI {
public:
    Zappy3DRenderer(const std::string& ip, int port);
    ~Zappy3DRenderer() = default;
    void run() override;

protected:
    void setupWindow() override;
    bool isWindowOpen() override;
    void pollEvents() override;
    void renderFrame() override;
    void closeWindow() override;

private:
    Camera camera;
    void drawMap();
    void drawPlayers();
    void drawTileResources(const Tile& tile, int x, int y);
    void drawSelection();

    float cameraSpeed;
    float mouseSensitivity;
    bool freeCameraMode;
    Vector2 lastMousePosition;
    void updateCameraControls();
    void handleKeyboardMovement();
    void handleMouseLook();
    void resetCameraPosition();

    bool followingPlayer;
    const Player* followedPlayer;
    bool povMode;
    float followDistance;
    float followHeight;
    void updateFollowCamera();
    void startFollowingPlayer(const Player* player);
    void stopFollowingPlayer();
    void togglePOVMode();

    void teleportToCorner(int corner);
    void teleportToCenter();
    void teleportToPlayer(const Player* player);

    void drawUI();
    void drawTopBar();
    void drawTeamStatsPanel();
    void drawPlayerPanel();
    void drawTilePanel();
    void drawEventLogPanel();
    void drawControlsPanel();
    void drawHelpText();

    void handleMouseClick();
    void handleUIInput();

    const Player* selectedPlayer;
    int selectedTileX, selectedTileY;

    bool showPlayerPanel;
    bool showTilePanel;
    bool showTeamStats;
    bool showEventLog;

    float gameSpeed;
    bool isPaused;

    std::vector<std::string> eventLog;
    void addEventToLog(const std::string& event);

    Color getTileColor(const Tile& tile);
    Color getTeamColor(const std::string& teamName);
    Vector3 getDirectionVector(int orientation);
    std::string getOrientationString(int orientation);

    int getScaledSize(int baseSize);
    int getScaledFontSize(int baseFontSize);
    int getPanelWidth(float percentage);
    int getPanelHeight(float percentage);

    GuiMediator mediator;
};
