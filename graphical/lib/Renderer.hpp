/*
** EPITECH PROJECT, 2025
** Renderer
** File description:
** Renders
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <vector>
#include "GameState.hpp"

class Renderer {
public:
    Renderer(sf::RenderWindow& window);

    void render(const GameState& gameState);
    void setViewport(int x, int y, int width, int height);
    void playBackgroundMusic(const std::string& filepath);

    void updateAnimation(float deltaTime);

private:
    sf::RenderWindow& window;
    sf::View gameView;
    sf::Font font;
    float tile_size;
    sf::Vector2f offset;
    sf::Music backgroundMusic;
    bool _musicPlaying = false;
    sf::Texture mainGuyTexture;
    sf::Sprite mainGuySprite;
    int animationFrame = 0;
    float animationTimer = 0.0f;
    const int frameCount = 4;
    const float frameDuration = 0.15f;
    float radius = 12.0f;

    void renderMap(const GameState& gameState);
    void renderTile(const Tile& tile);
    void renderPlayers(const GameState& gameState);
    void renderPlayer(const Player& player);
    void renderBackground(const Tile& tile);
    void renderUI(const GameState& gameState);
    void renderResources(sf::RenderWindow& window, const Tile& tile);
    std::vector<int> computeTotalResources(const GameState& gameState);
    void drawResourceTotals(const GameState& gameState);

    sf::Color getResourceColor(const std::vector<int>& resources);
    sf::Color getTeamColor(const std::string& team_name);
    sf::Vector2f worldToScreen(int world_x, int world_y);
    void calculateTileSize(const GameState& gameState);
    sf::Clock clock;
    const int frameCols = 3;
    const int frameRows = 4;
    std::vector<sf::Color> teamColorList;
    std::map<std::string, sf::Color> teamColorMap;
    void loadTeamColors(const std::string& filename);
};
