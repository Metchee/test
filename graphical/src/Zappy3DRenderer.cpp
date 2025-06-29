/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Zappy3DRenderer
*/

#include "../lib/Zappy3DRenderer.hpp"
#include "../lib/ZappyAbstractGUI.hpp"
#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

Zappy3DRenderer::Zappy3DRenderer(const std::string& ip, int port)
    : ZappyAbstractGUI(ip, port), mediator(ip, port), selectedPlayer(nullptr), selectedTileX(-1), selectedTileY(-1),
      showPlayerPanel(false), showTilePanel(false), showTeamStats(true),
      gameSpeed(1.0f), isPaused(false), showEventLog(true),
      cameraSpeed(0.5f), mouseSensitivity(0.003f), freeCameraMode(true),
      followingPlayer(false), followedPlayer(nullptr), povMode(false),
      followDistance(5.0f), followHeight(3.0f)
{
    camera = {0};
    eventLog.reserve(100);
    lastMousePosition = {0, 0};
}

void Zappy3DRenderer::setupWindow() {
    windowWidth = 1920;
    windowHeight = 1080;
    InitWindow(windowWidth, windowHeight, "Zappy 3D - Enhanced Interface");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(800, 600);

    camera.position = { (float)gameState.getMapWidth() / 2, 15.0f, (float)gameState.getMapHeight() / 2 + 10.0f };
    camera.target = { (float)gameState.getMapWidth() / 2, 0.0f, (float)gameState.getMapHeight() / 2 };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    lastMousePosition = GetMousePosition();

    addEventToLog("3D Interface initialized");
    addEventToLog("ZQSD: Move camera | Right Click: Look around | F: Follow player");
    addEventToLog("Numpad 1-4: Teleport to corners | Numpad 5: Center | V: POV mode");
}

bool Zappy3DRenderer::isWindowOpen() {
    return !WindowShouldClose();
}

void Zappy3DRenderer::pollEvents() {
    updateCameraControls();

    if (IsWindowResized()) {
        windowWidth = GetScreenWidth();
        windowHeight = GetScreenHeight();
        addEventToLog("Window resized to " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight));
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        handleMouseClick();
    }

    if (IsKeyPressed(KEY_SPACE)) {
        isPaused = !isPaused;
        addEventToLog(isPaused ? "Game paused" : "Game resumed");
    }

    if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD)) {
        gameSpeed = std::min(gameSpeed + 0.5f, 5.0f);
        addEventToLog("Speed: " + std::to_string(gameSpeed) + "x");
    }

    if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) {
        gameSpeed = std::max(gameSpeed - 0.5f, 0.1f);
        addEventToLog("Speed: " + std::to_string(gameSpeed) + "x");
    }

    if (IsKeyPressed(KEY_P)) {
        showPlayerPanel = !showPlayerPanel;
    }
    if (IsKeyPressed(KEY_T)) {
        showTeamStats = !showTeamStats;
    }
    if (IsKeyPressed(KEY_L)) {
        showEventLog = !showEventLog;
    }

    if (IsKeyPressed(KEY_C)) {
        freeCameraMode = !freeCameraMode;
        if (freeCameraMode) {
            stopFollowingPlayer();
        }
        addEventToLog(freeCameraMode ? "Free camera mode enabled" : "Orbital camera mode enabled");
    }

    if (IsKeyPressed(KEY_F)) {
        if (selectedPlayer && !followingPlayer) {
            startFollowingPlayer(selectedPlayer);
        } else {
            stopFollowingPlayer();
        }
    }

    if (IsKeyPressed(KEY_V) && followingPlayer) {
        togglePOVMode();
    }

    if (IsKeyPressed(KEY_KP_1)) {
        teleportToCorner(2);
        addEventToLog("Teleported to bottom-left corner");
    }
    if (IsKeyPressed(KEY_KP_2)) {
        teleportToCenter();
        addEventToLog("Teleported to map center");
    }
    if (IsKeyPressed(KEY_KP_3)) {
        teleportToCorner(3);
        addEventToLog("Teleported to bottom-right corner");
    }
    if (IsKeyPressed(KEY_KP_4)) {
        teleportToCorner(0);
        addEventToLog("Teleported to top-left corner");
    }
    if (IsKeyPressed(KEY_KP_5)) {
        teleportToCenter();
        addEventToLog("Teleported to map center");
    }
    if (IsKeyPressed(KEY_KP_6)) {
        teleportToCorner(1);
        addEventToLog("Teleported to top-right corner");
    }
    if (IsKeyPressed(KEY_KP_7)) {
        teleportToCorner(0);
        addEventToLog("Teleported to top-left corner");
    }
    if (IsKeyPressed(KEY_KP_9)) {
        teleportToCorner(1);
        addEventToLog("Teleported to top-right corner");
    }

    if (IsKeyPressed(KEY_G) && selectedPlayer) {
        teleportToPlayer(selectedPlayer);
        addEventToLog("Teleported to player " + std::to_string(selectedPlayer->id));
    }

    if (IsKeyPressed(KEY_R)) {
        resetCameraPosition();
        addEventToLog("Camera position reset");
    }

    if (IsKeyPressed(KEY_F11)) {
        ToggleFullscreen();
        addEventToLog(IsWindowFullscreen() ? "Entered fullscreen" : "Exited fullscreen");
    }
}

void Zappy3DRenderer::updateCameraControls() {
    if (followingPlayer) {
        updateFollowCamera();
    } else if (freeCameraMode) {
        handleKeyboardMovement();
        handleMouseLook();
    } else {
        UpdateCamera(&camera, CAMERA_ORBITAL);
    }
}

void Zappy3DRenderer::handleKeyboardMovement() {
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

    Vector3 movement = {0, 0, 0};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_Z)) {
        movement = Vector3Add(movement, Vector3Scale(forward, cameraSpeed));
    }
    if (IsKeyDown(KEY_S)) {
        movement = Vector3Subtract(movement, Vector3Scale(forward, cameraSpeed));
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_Q)) {
        movement = Vector3Subtract(movement, Vector3Scale(right, cameraSpeed));
    }
    if (IsKeyDown(KEY_D)) {
        movement = Vector3Add(movement, Vector3Scale(right, cameraSpeed));
    }

    if (IsKeyDown(KEY_E) || IsKeyDown(KEY_SPACE)) {
        movement.y += cameraSpeed;
    }
    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
        movement.y -= cameraSpeed;
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
        movement = Vector3Scale(movement, 0.3f);
    }
    if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) {
        movement = Vector3Scale(movement, 3.0f);
    }

    camera.position = Vector3Add(camera.position, movement);
    camera.target = Vector3Add(camera.target, movement);
}

void Zappy3DRenderer::handleMouseLook() {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 mousePosition = GetMousePosition();
        Vector2 mouseDelta = {
            mousePosition.x - lastMousePosition.x,
            mousePosition.y - lastMousePosition.y
        };

        Vector3 forward = Vector3Subtract(camera.target, camera.position);

        float yaw = -mouseDelta.x * mouseSensitivity;
        float pitch = -mouseDelta.y * mouseSensitivity;
        Matrix yawMatrix = MatrixRotateY(yaw);
        forward = Vector3Transform(forward, yawMatrix);
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
        Matrix pitchMatrix = MatrixRotate(right, pitch);
        forward = Vector3Transform(forward, pitchMatrix);
        float currentPitch = asinf(forward.y / Vector3Length(forward));
        if (currentPitch > 1.4f) forward.y = sinf(1.4f) * Vector3Length(forward);
        if (currentPitch < -1.4f) forward.y = sinf(-1.4f) * Vector3Length(forward);
        camera.target = Vector3Add(camera.position, Vector3Normalize(forward));
    }

    lastMousePosition = GetMousePosition();
}

void Zappy3DRenderer::resetCameraPosition() {
    stopFollowingPlayer();
    camera.position = { (float)gameState.getMapWidth() / 2, 15.0f, (float)gameState.getMapHeight() / 2 + 10.0f };
    camera.target = { (float)gameState.getMapWidth() / 2, 0.0f, (float)gameState.getMapHeight() / 2 };
    camera.up = { 0.0f, 1.0f, 0.0f };
}

void Zappy3DRenderer::updateFollowCamera() {
    if (!followedPlayer) {
        stopFollowingPlayer();
        return;
    }

    bool playerExists = false;
    for (const Player& player : gameState.getPlayers()) {
        if (player.id == followedPlayer->id) {
            followedPlayer = &player;
            playerExists = true;
            break;
        }
    }

    if (!playerExists) {
        addEventToLog("Followed player disconnected");
        stopFollowingPlayer();
        return;
    }

    Vector3 playerPos = {(float)followedPlayer->x, 1.0f, (float)followedPlayer->y};

    if (povMode) {
        camera.position = {playerPos.x, playerPos.y + 0.8f, playerPos.z};

        Vector3 direction = getDirectionVector(followedPlayer->orientation);
        camera.target = Vector3Add(camera.position, Vector3Scale(direction, 5.0f));
        camera.up = {0.0f, 1.0f, 0.0f};

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            handleMouseLook();
        }
    } else {
        Vector3 direction = getDirectionVector(followedPlayer->orientation);

        Vector3 offset = Vector3Scale(direction, -followDistance);
        offset.y = followHeight;

        camera.position = Vector3Add(playerPos, offset);
        camera.target = playerPos;
        camera.up = {0.0f, 1.0f, 0.0f};

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
                followDistance = std::max(2.0f, followDistance - 0.1f);
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                followDistance = std::min(15.0f, followDistance + 0.1f);
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                followHeight = std::max(1.0f, followHeight - 0.1f);
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                followHeight = std::min(20.0f, followHeight + 0.1f);
            }
        }
    }
}

void Zappy3DRenderer::startFollowingPlayer(const Player* player) {
    if (!player) return;

    followedPlayer = player;
    followingPlayer = true;
    freeCameraMode = false;

    addEventToLog("Following player " + std::to_string(player->id) +
                  " (" + player->team_name + ")");
    addEventToLog("Press V for POV mode, F to stop following");
}

void Zappy3DRenderer::stopFollowingPlayer() {
    if (followingPlayer) {
        addEventToLog("Stopped following player");
    }
    followingPlayer = false;
    followedPlayer = nullptr;
    povMode = false;
    freeCameraMode = true;
}

void Zappy3DRenderer::togglePOVMode() {
    if (!followingPlayer) return;

    povMode = !povMode;

    if (povMode) {
        addEventToLog("POV mode enabled - experiencing " + followedPlayer->team_name +
                      " player " + std::to_string(followedPlayer->id) + "'s view");
    } else {
        addEventToLog("Third-person follow mode enabled");
    }
}

void Zappy3DRenderer::teleportToCorner(int corner) {
    stopFollowingPlayer();

    float mapWidth = (float)gameState.getMapWidth();
    float mapHeight = (float)gameState.getMapHeight();

    switch (corner) {
        case 0:
            camera.position = {-5.0f, 15.0f, -5.0f};
            camera.target = {mapWidth * 0.25f, 0.0f, mapHeight * 0.25f};
            break;
        case 1:
            camera.position = {mapWidth + 5.0f, 15.0f, -5.0f};
            camera.target = {mapWidth * 0.75f, 0.0f, mapHeight * 0.25f};
            break;
        case 2:
            camera.position = {-5.0f, 15.0f, mapHeight + 5.0f};
            camera.target = {mapWidth * 0.25f, 0.0f, mapHeight * 0.75f};
            break;
        case 3:
            camera.position = {mapWidth + 5.0f, 15.0f, mapHeight + 5.0f};
            camera.target = {mapWidth * 0.75f, 0.0f, mapHeight * 0.75f};
            break;
    }
    camera.up = {0.0f, 1.0f, 0.0f};
}

void Zappy3DRenderer::teleportToCenter() {
    stopFollowingPlayer();

    float mapWidth = (float)gameState.getMapWidth();
    float mapHeight = (float)gameState.getMapHeight();

    camera.position = {mapWidth / 2, 20.0f, mapHeight / 2 + 15.0f};
    camera.target = {mapWidth / 2, 0.0f, mapHeight / 2};
    camera.up = {0.0f, 1.0f, 0.0f};
}

void Zappy3DRenderer::teleportToPlayer(const Player* player) {
    if (!player) return;

    stopFollowingPlayer();

    Vector3 playerPos = {(float)player->x, 1.0f, (float)player->y};
    Vector3 direction = getDirectionVector(player->orientation);

    Vector3 offset = Vector3Scale(direction, -8.0f);
    offset.y = 6.0f;

    camera.position = Vector3Add(playerPos, offset);
    camera.target = playerPos;
    camera.up = {0.0f, 1.0f, 0.0f};
}

void Zappy3DRenderer::renderFrame() {
    BeginDrawing();
    ClearBackground({20, 25, 40, 255});
    BeginMode3D(camera);
    drawMap();
    drawPlayers();
    drawSelection();
    EndMode3D();
    drawUI();

    EndDrawing();
}

void Zappy3DRenderer::closeWindow() {
    CloseWindow();
}

void Zappy3DRenderer::drawMap() {
    const GameState& gameState = mediator.getGameState();
    for (int y = 0; y < gameState.getMapHeight(); ++y) {
        for (int x = 0; x < gameState.getMapWidth(); ++x) {
            const Tile& tile = gameState.getTile(x, y);

            Color tileColor = getTileColor(tile);

            DrawCube({(float)x, 0.0f, (float)y}, 0.9f, 0.1f, 0.9f, tileColor);
            DrawCubeWires({(float)x, 0.0f, (float)y}, 1.0f, 0.1f, 1.0f, {100, 100, 100, 100});

            drawTileResources(tile, x, y);

            if (selectedTileX == x && selectedTileY == y) {
                DrawCubeWires({(float)x, 0.5f, (float)y}, 1.1f, 1.0f, 1.1f, YELLOW);
            }
        }
    }
}

void Zappy3DRenderer::drawPlayers() {
    for (const Player& player : mediator.getGameState().getPlayers()) {
        Vector3 position = {(float)player.x, 1.0f, (float)player.y};

        Color playerColor = getTeamColor(player.team_name);
        DrawSphere(position, 0.3f, playerColor);

        float crownHeight = 0.1f * player.level;
        DrawCube({position.x, position.y + 0.3f + crownHeight/2, position.z},
                 0.2f, crownHeight, 0.2f, GOLD);

        Vector3 direction = getDirectionVector(player.orientation);
        Vector3 arrowEnd = {
            position.x + direction.x * 0.5f,
            position.y,
            position.z + direction.z * 0.5f
        };

        DrawLine3D(position, arrowEnd, WHITE);
        DrawSphere(arrowEnd, 0.05f, WHITE);

        if (selectedPlayer && selectedPlayer->id == player.id) {
            DrawSphereWires(position, 0.4f, 8, 8, YELLOW);
        }

        Vector2 screenPos = GetWorldToScreen(position, camera);
        if (screenPos.x >= 0 && screenPos.x < GetScreenWidth() &&
            screenPos.y >= 0 && screenPos.y < GetScreenHeight()) {
            DrawText(TextFormat("P%d", player.id), (int)screenPos.x - 15, (int)screenPos.y - 50, 16, WHITE);
            DrawText(TextFormat("Lv%d", player.level), (int)screenPos.x - 20, (int)screenPos.y - 30, 14, LIGHTGRAY);
        }
    }
}

void Zappy3DRenderer::drawTileResources(const Tile& tile, int x, int y) {
    const std::vector<Color> resourceColors = {
        BROWN,
        LIGHTGRAY,
        GOLD,
        BLUE,
        PURPLE,
        ORANGE,
        RED
    };

    int resourceCount = 0;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < tile.resources[i]; ++j) {
            if (resourceCount >= 9) break;

            float offsetX = (resourceCount % 3 - 1) * 0.2f;
            float offsetZ = (resourceCount / 3 - 1) * 0.2f;

            DrawCube({(float)x + offsetX, 0.15f, (float)y + offsetZ},
                     0.1f, 0.1f, 0.1f, resourceColors[i]);
            resourceCount++;
        }
        if (resourceCount >= 9) break;
    }
}

void Zappy3DRenderer::drawSelection() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
    }
}

void Zappy3DRenderer::drawUI() {
    drawTopBar();

    if (showTeamStats) drawTeamStatsPanel();
    if (showPlayerPanel && selectedPlayer) drawPlayerPanel();
    if (showTilePanel && selectedTileX >= 0) drawTilePanel();
    if (showEventLog) drawEventLogPanel();

    drawControlsPanel();

    drawHelpText();
}

void Zappy3DRenderer::drawTopBar() {
    const GameState& gameState = mediator.getGameState();
    int barHeight = getScaledSize(80);
    DrawRectangle(0, 0, GetScreenWidth(), barHeight, {0, 0, 0, 180});

    int titleSize = getScaledFontSize(32);
    int margin = getScaledSize(30);
    DrawText("ZAPPY 3D", margin, barHeight/2 - titleSize/2, titleSize, WHITE);

    int infoSize = getScaledFontSize(20);
    std::string mapInfo = "Map: " + std::to_string(gameState.getMapWidth()) + "x" +
                         std::to_string(gameState.getMapHeight());
    DrawText(mapInfo.c_str(), getScaledSize(220), barHeight/2 - infoSize/2, infoSize, LIGHTGRAY);

    std::string playerInfo = "Players: " + std::to_string(gameState.getPlayers().size());
    DrawText(playerInfo.c_str(), getScaledSize(450), barHeight/2 - infoSize/2, infoSize, LIGHTGRAY);

    std::string cameraMode;
    if (followingPlayer) {
        cameraMode = povMode ? "POV Mode - Player " + std::to_string(followedPlayer->id) :
                               "Following Player " + std::to_string(followedPlayer->id);
    } else {
        cameraMode = freeCameraMode ? "Free Camera" : "Orbital Camera";
    }
    DrawText(cameraMode.c_str(), getScaledSize(650), barHeight/2 - infoSize/2, infoSize, YELLOW);

    std::string speedInfo = isPaused ? "PAUSED" : ("Speed: " + std::to_string(gameSpeed) + "x");
    Color speedColor = isPaused ? RED : GREEN;
    DrawText(speedInfo.c_str(), GetScreenWidth() - getScaledSize(200), barHeight/2 - infoSize/2, infoSize, speedColor);

    int fpsSize = getScaledFontSize(16);
    DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() - getScaledSize(100), 10, fpsSize, YELLOW);
}

void Zappy3DRenderer::drawTeamStatsPanel() {
    int panelWidth = getPanelWidth(0.18f);
    int panelHeight = getPanelHeight(0.37f);
    int margin = getScaledSize(20);
    int topBarHeight = getScaledSize(80);
    int x = GetScreenWidth() - panelWidth - margin;
    int y = topBarHeight + margin;

    DrawRectangle(x, y, panelWidth, panelHeight, {0, 0, 0, 200});
    DrawRectangleLines(x, y, panelWidth, panelHeight, WHITE);

    int titleSize = getScaledFontSize(20);
    int padding = getScaledSize(15);
    DrawText("TEAM STATISTICS", x + padding, y + padding, titleSize, WHITE);

    // Get the current game state from mediator
    const GameState& currentGameState = mediator.getGameState();
    
    std::map<std::string, std::vector<const Player*>> teamPlayers;
    for (const Player& player : currentGameState.getPlayers()) {
        teamPlayers[player.team_name].push_back(&player);
    }

    if (teamPlayers.empty()) {
        DrawText("No players connected", x + padding, y + padding + getScaledSize(40), 
                 getScaledFontSize(16), GRAY);
        return;
    }

    int yOffset = getScaledSize(50);
    int teamNameSize = getScaledFontSize(18);
    int statsSize = getScaledFontSize(14);
    int lineSpacing = getScaledSize(45);

    for (const auto& team : teamPlayers) {
        // Check bounds
        if (yOffset > panelHeight - getScaledSize(60)) {
            DrawText("...", x + padding, y + yOffset, statsSize, GRAY);
            break;
        }

        Color teamColor = getTeamColor(team.first);
        DrawText(team.first.c_str(), x + padding, y + yOffset, teamNameSize, teamColor);

        int totalLevel = 0;
        for (const Player* player : team.second) {
            totalLevel += player->level;
        }
        float avgLevel = team.second.empty() ? 0 : (float)totalLevel / team.second.size();

        std::string stats = TextFormat("Players: %d  Avg Lvl: %.1f",
                                     (int)team.second.size(), avgLevel);
        DrawText(stats.c_str(), x + padding + getScaledSize(5), 
                 y + yOffset + getScaledSize(20), statsSize, LIGHTGRAY);

        yOffset += lineSpacing;
    }
}

void Zappy3DRenderer::drawPlayerPanel() {
    if (!selectedPlayer) return;

    const GameState& gameState = mediator.getGameState();

    int panelWidth = 280;
    int panelHeight = 320;
    int x = 20;
    int y = 100;

    DrawRectangle(x, y, panelWidth, panelHeight, {0, 0, 0, 200});
    DrawRectangleLines(x, y, panelWidth, panelHeight, WHITE);

    DrawText("PLAYER INFO", x + 15, y + 15, 20, WHITE);

    int yOffset = 50;
    DrawText(TextFormat("ID: %d", selectedPlayer->id), x + 15, y + yOffset, 16, WHITE);
    yOffset += 25;

    Color teamColor = getTeamColor(selectedPlayer->team_name);
    DrawText(TextFormat("Team: %s", selectedPlayer->team_name.c_str()), x + 15, y + yOffset, 16, teamColor);
    yOffset += 25;

    DrawText(TextFormat("Level: %d", selectedPlayer->level), x + 15, y + yOffset, 16, WHITE);
    yOffset += 25;

    DrawText(TextFormat("Position: (%d, %d)", selectedPlayer->x, selectedPlayer->y), x + 15, y + yOffset, 16, WHITE);
    yOffset += 25;

    std::string orientationText = "Facing: " + getOrientationString(selectedPlayer->orientation);
    DrawText(orientationText.c_str(), x + 15, y + yOffset, 16, WHITE);
    yOffset += 35;

    DrawText("ACTIONS:", x + 15, y + yOffset, 16, YELLOW);
    yOffset += 25;
    DrawText("F - Follow this player", x + 15, y + yOffset, 14, LIGHTGRAY);
    yOffset += 20;
    DrawText("G - Teleport to player", x + 15, y + yOffset, 14, LIGHTGRAY);
    yOffset += 20;
    DrawText("V - POV mode (when following)", x + 15, y + yOffset, 14, LIGHTGRAY);
    yOffset += 25;

    if (followingPlayer && followedPlayer && followedPlayer->id == selectedPlayer->id) {
        std::string followStatus = povMode ? "Currently in POV mode" : "Currently following";
        DrawText(followStatus.c_str(), x + 15, y + yOffset, 14, GREEN);
    }
}

void Zappy3DRenderer::drawTilePanel() {
    if (selectedTileX < 0 || selectedTileY < 0) return;
    const GameState& gameState = mediator.getGameState();
    const Tile& tile = gameState.getTile(selectedTileX, selectedTileY);

    int panelWidth = 250;
    int panelHeight = 280;
    int x = 20;
    int y = GetScreenHeight() - panelHeight - 20;

    DrawRectangle(x, y, panelWidth, panelHeight, {0, 0, 0, 200});
    DrawRectangleLines(x, y, panelWidth, panelHeight, WHITE);

    DrawText("TILE INFO", x + 15, y + 15, 20, WHITE);
    DrawText(TextFormat("(%d, %d)", selectedTileX, selectedTileY), x + 15, y + 40, 16, LIGHTGRAY);

    const std::vector<std::string> resourceNames = {
        "Food", "Linemate", "Deraumere", "Sibur",
        "Mendiane", "Phiras", "Thystame"
    };

    int yOffset = 75;
    for (int i = 0; i < 7; ++i) {
        if (tile.resources[i] > 0) {
            DrawText(TextFormat("%s: %d", resourceNames[i].c_str(), tile.resources[i]),
                     x + 15, y + yOffset, 14, WHITE);
            yOffset += 20;
        }
    }

    if (yOffset == 75) {
        DrawText("No resources", x + 15, y + yOffset, 14, GRAY);
    }
}

void Zappy3DRenderer::drawEventLogPanel() {
    int panelWidth = 400;
    int panelHeight = 200;
    int x = GetScreenWidth() - panelWidth - 20;
    int y = GetScreenHeight() - panelHeight - 20;
    const GameState& gameState = mediator.getGameState();


    DrawRectangle(x, y, panelWidth, panelHeight, {0, 0, 0, 200});
    DrawRectangleLines(x, y, panelWidth, panelHeight, WHITE);

    DrawText("EVENT LOG", x + 15, y + 15, 18, WHITE);

    int maxEvents = 12;
    int startIdx = std::max(0, (int)eventLog.size() - maxEvents);

    for (int i = startIdx; i < eventLog.size(); ++i) {
        int yPos = y + 45 + (i - startIdx) * 14;
        DrawText(eventLog[i].c_str(), x + 15, yPos, 12, LIGHTGRAY);
    }
}

void Zappy3DRenderer::drawControlsPanel() {
    int panelHeight = getScaledSize(50);
    int y = GetScreenHeight() - panelHeight;

    DrawRectangle(0, y, GetScreenWidth(), panelHeight, {0, 0, 0, 150});

    std::string controls;

    if (followingPlayer) {
        if (povMode) {
            controls = "POV MODE: Right Click+Mouse=Look | F=Stop Following | V=Third Person | Ctrl+WASD=Adjust View";
        } else {
            controls = "FOLLOWING: F=Stop Following | V=POV Mode | Ctrl+WASD=Adjust Distance/Height | G=Teleport to Player";
        }
    } else if (freeCameraMode) {
        controls = "FREE CAMERA: ZQSD=Move | Right Click+Mouse=Look | F=Follow Selected | G=Go to Player | Numpad=Quick Teleport";
    } else {
        controls = "ORBITAL CAMERA: Mouse=Orbit | C=Free Camera | F=Follow Selected | Numpad=Quick Teleport";
    }

    int fontSize = getScaledFontSize(14);
    int margin = getScaledSize(20);
    DrawText(controls.c_str(), margin, y + panelHeight/2 - fontSize/2, fontSize, LIGHTGRAY);
}

void Zappy3DRenderer::drawHelpText() {
    if (IsKeyDown(KEY_H)) {
        int panelWidth = getPanelWidth(0.35f);
        int panelHeight = getPanelHeight(0.45f);
        int x = (GetScreenWidth() - panelWidth) / 2;
        int y = (GetScreenHeight() - panelHeight) / 2;

        DrawRectangle(x, y, panelWidth, panelHeight, {0, 0, 0, 220});
        DrawRectangleLines(x, y, panelWidth, panelHeight, WHITE);

        int padding = getScaledSize(30);
        int titleSize = getScaledFontSize(28);
        int textSize = getScaledFontSize(16);
        int smallSize = getScaledFontSize(14);
        int lineSpacing = getScaledSize(25);

        DrawText("HELP - CAMERA & CONTROLS", x + padding, y + padding, titleSize, WHITE);

        int yOffset = padding + lineSpacing * 1.5f;

        DrawText("CAMERA MOVEMENT:", x + padding, y + yOffset, textSize, YELLOW);
        yOffset += lineSpacing;
        DrawText("Z/W: Move forward", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.7f;
        DrawText("S: Move backward", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.7f;
        DrawText("Q/A: Move left", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.7f;
        DrawText("D: Move right", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.7f;
        DrawText("E/Space: Move up", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.7f;
        DrawText("Shift: Move down", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing;

        DrawText("PLAYER FOLLOWING:", x + padding, y + yOffset, textSize, YELLOW);
        yOffset += lineSpacing;
        DrawText("F: Follow selected player", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.7f;
        DrawText("V: Toggle POV mode", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.7f;
        DrawText("G: Teleport to player", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing;

        DrawText("QUICK TELEPORT:", x + padding, y + yOffset, textSize, YELLOW);
        yOffset += lineSpacing;
        DrawText("Numpad 1-3-7-9: Corners", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.7f;
        DrawText("Numpad 2-5: Center", x + padding + 20, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing;

        DrawText("Right Click + Mouse: Look around", x + padding, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.8f;
        DrawText("C: Toggle camera mode", x + padding, y + yOffset, smallSize, LIGHTGRAY);
        yOffset += lineSpacing * 0.8f;
        DrawText("R: Reset camera position", x + padding, y + yOffset, smallSize, LIGHTGRAY);

        DrawText("Hold H to show this help", x + padding, y + panelHeight - getScaledSize(45), smallSize, YELLOW);
    } else {
        int fontSize = getScaledFontSize(14);
        DrawText("Hold H for help", getScaledSize(20), 10, fontSize, GRAY);
    }
}

void Zappy3DRenderer::handleMouseClick() {
    Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
    const GameState& gameState = mediator.getGameState();

    selectedPlayer = nullptr;
    float closestPlayerDist = 1000000.0f;

    for (const Player& player : gameState.getPlayers()) {
        Vector3 playerPos = {(float)player.x, 1.0f, (float)player.y};
        RayCollision collision = GetRayCollisionSphere(mouseRay, playerPos, 0.4f);

        if (collision.hit && collision.distance < closestPlayerDist) {
            selectedPlayer = &player;
            closestPlayerDist = collision.distance;
            showPlayerPanel = true;
            showTilePanel = false;

            addEventToLog("Selected player " + std::to_string(player.id) +
                         " (" + player.team_name + ") - Press F to follow, G to teleport");
        }
    }

    if (!selectedPlayer) {
        float t = -mouseRay.position.y / mouseRay.direction.y;
        if (t > 0) {
            Vector3 hitPoint = {
                mouseRay.position.x + mouseRay.direction.x * t,
                0,
                mouseRay.position.z + mouseRay.direction.z * t
            };

            int tileX = (int)round(hitPoint.x);
            int tileZ = (int)round(hitPoint.z);

            if (tileX >= 0 && tileX < gameState.getMapWidth() &&
                tileZ >= 0 && tileZ < gameState.getMapHeight()) {
                selectedTileX = tileX;
                selectedTileY = tileZ;
                showTilePanel = true;
                showPlayerPanel = false;

                addEventToLog("Selected tile (" + std::to_string(tileX) +
                             ", " + std::to_string(tileZ) + ")");
            }
        }
    }
}

Color Zappy3DRenderer::getTileColor(const Tile& tile) {
    int maxResource = 0;
    int dominantResource = -1;

    for (int i = 0; i < 7; ++i) {
        if (tile.resources[i] > maxResource) {
            maxResource = tile.resources[i];
            dominantResource = i;
        }
    }

    if (maxResource == 0) return {40, 40, 40, 255};

    const std::vector<Color> resourceColors = {
        {101, 67, 33, 255},
        {192, 192, 192, 255},
        {255, 215, 0, 255},
        {0, 0, 255, 255},
        {128, 0, 128, 255},
        {255, 165, 0, 255},
        {255, 0, 0, 255}
    };

    Color baseColor = resourceColors[dominantResource];
    float intensity = std::min(1.0f, maxResource / 5.0f);
    return {
        (unsigned char)(baseColor.r * intensity),
        (unsigned char)(baseColor.g * intensity),
        (unsigned char)(baseColor.b * intensity),
        200
    };
}

Color Zappy3DRenderer::getTeamColor(const std::string& teamName) {
    unsigned int hash = 0;
    for (char c : teamName) {
        hash = hash * 31 + c;
    }

    return {
        (unsigned char)(100 + (hash % 155)),
        (unsigned char)(100 + ((hash >> 8) % 155)),
        (unsigned char)(100 + ((hash >> 16) % 155)),
        255
    };
}

Vector3 Zappy3DRenderer::getDirectionVector(int orientation) {
    switch (orientation) {
        case 1: return {0, 0, -1};
        case 2: return {1, 0, 0};
        case 3: return {0, 0, 1};
        case 4: return {-1, 0, 0};
        default: return {0, 0, -1};
    }
}

std::string Zappy3DRenderer::getOrientationString(int orientation) {
    switch (orientation) {
        case 1: return "North";
        case 2: return "East";
        case 3: return "South";
        case 4: return "West";
        default: return "Unknown";
    }
}

void Zappy3DRenderer::addEventToLog(const std::string& event) {
    eventLog.push_back(event);
    if (eventLog.size() > 50) {
        eventLog.erase(eventLog.begin());
    }
}

void Zappy3DRenderer::handleUIInput() {
}

int Zappy3DRenderer::getScaledSize(int baseSize) {
    float scale = (float)GetScreenWidth() / 1920.0f;
    return (int)(baseSize * scale);
}

int Zappy3DRenderer::getScaledFontSize(int baseFontSize) {
    float scale = (float)GetScreenWidth() / 1920.0f;
    int scaledSize = (int)(baseFontSize * scale);
    return std::max(scaledSize, 8);
}

int Zappy3DRenderer::getPanelWidth(float percentage) {
    return (int)(GetScreenWidth() * percentage);
}

int Zappy3DRenderer::getPanelHeight(float percentage) {
    return (int)(GetScreenHeight() * percentage);
}

void Zappy3DRenderer::run() {
    mediator.initialize();
    setupWindow();
    while (isWindowOpen()) {
        pollEvents();
        mediator.update();
        renderFrame();
    }
    closeWindow();
}
