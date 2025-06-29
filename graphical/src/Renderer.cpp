/*
** EPITECH PROJECT, 2025
** Renderer
** File description:
** Implementation of Renderer
*/

#include "../lib/Renderer.hpp"
#include <iostream>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <fstream>
#include <algorithm> // pour std::transform

Renderer::Renderer(sf::RenderWindow& win) : window(win), tile_size(32.0f), offset(50, 50) {
    if (!font.loadFromFile("../graphical/asset/dejavu-fonts-ttf-2.37/ttf/DejaVuSans.ttf")) {
        std::cout << "Warning: Could not load font, using default" << std::endl;
    }
    gameView = window.getDefaultView();
    if (!mainGuyTexture.loadFromFile("../graphical/asset/MainGuySpriteSheet.png")) {
        std::cerr << "Erreur : impossible de charger MainGuySpriteSheet.png" << std::endl;
    }
    mainGuySprite.setTexture(mainGuyTexture);
    loadTeamColors("../graphical/asset/team_colors.txt");
}

void Renderer::render(const GameState& gameState) {
    // std::cout << "2D Map size: " << gameState.getMapWidth() << "x" << gameState.getMapHeight() << std::endl;

    window.clear(sf::Color(30, 30, 30));

    if (gameState.getMapWidth() > 0 && gameState.getMapHeight() > 0) {
        calculateTileSize(gameState);
        renderMap(gameState);
        renderPlayers(gameState);
    }

    renderUI(gameState);
}

void Renderer::calculateTileSize(const GameState& gameState) {
    float window_width = static_cast<float>(window.getSize().x) - 2 * offset.x;
    float window_height = static_cast<float>(window.getSize().y) - 2 * offset.y - 100;

    float tile_width = window_width / gameState.getMapWidth();
    float tile_height = window_height / gameState.getMapHeight();

    tile_size = std::min(tile_width, tile_height);
    tile_size = std::max(tile_size, 8.0f);
}

void Renderer::renderMap(const GameState& gameState) {

    for (int y = 0; y < gameState.getMapHeight(); ++y) {
        for (int x = 0; x < gameState.getMapWidth(); ++x) {
            renderTile(gameState.getTile(x, y));
            drawResourceTotals(gameState);
        }
    }
}

void Renderer::renderTile(const Tile& tile) {
    sf::Vector2f pos = worldToScreen(tile.x, tile.y);
    sf::RectangleShape tileRect(sf::Vector2f(tile_size - 1, tile_size - 1));
    tileRect.setPosition(pos);
    tileRect.setFillColor(getResourceColor(tile.resources));
    tileRect.setOutlineThickness(1);
    tileRect.setOutlineColor(sf::Color(100, 100, 100));

    window.draw(tileRect);

    int total_resources = 0;
    for (int res : tile.resources) {
        total_resources += res;
    }
        renderBackground(tile);
    if (total_resources > 0 && tile_size > 15) {
        renderResources(window, tile);
    }
}

void Renderer::renderPlayer(const Player& player) {
    sf::Vector2f pos = worldToScreen(player.x, player.y);

    int frameWidth = mainGuyTexture.getSize().x / frameCols;
    int frameHeight = mainGuyTexture.getSize().y / frameRows;

    int row = 0;
    switch (player.orientation) {
        case 1:
            row = 2;
            break;
        case 2:
            row = 1;
            break;
        case 3:
            row = 0;
            break;
        case 4:
            row = 3;
            break;
        default:
            row = 0;
            break;
    }

    sf::IntRect textureRect(animationFrame * frameWidth, row * frameHeight, frameWidth, frameHeight);
    mainGuySprite.setTextureRect(textureRect);

    float scaleX = tile_size / static_cast<float>(frameWidth);
    float scaleY = tile_size / static_cast<float>(frameHeight);
    mainGuySprite.setScale(scaleX, scaleY);

    mainGuySprite.setColor(getTeamColor(player.team_name));

    mainGuySprite.setPosition(
        pos.x + tile_size / 2.f - (frameWidth * scaleX) / 2.f,
        pos.y + tile_size / 2.f - (frameHeight * scaleY) / 2.f
    );
    window.draw(mainGuySprite);

    sf::Vector2f center(pos.x + tile_size/2, pos.y + tile_size/2);
    sf::Vector2f direction;

    switch (player.orientation) {
        case 1: direction = sf::Vector2f(0, -1); break;
        case 2: direction = sf::Vector2f(1, 0); break;
        case 3: direction = sf::Vector2f(0, 1); break;
        case 4: direction = sf::Vector2f(-1, 0); break;
        default: direction = sf::Vector2f(0, -1); break;
    }
    if (tile_size > 20) {
        sf::Text idText;
        idText.setFont(font);
        idText.setString(std::to_string(player.id) + "\nLv" + std::to_string(player.level));
        idText.setCharacterSize(static_cast<unsigned int>(tile_size * 0.2f));
        idText.setFillColor(sf::Color::Black);
        idText.setPosition(pos.x + 2, pos.y + 2);

        window.draw(idText);
    }
}

void Renderer::renderUI(const GameState& gameState) {
    sf::RectangleShape infoPanel(sf::Vector2f(static_cast<float>(window.getSize().x), 80));
    infoPanel.setPosition(0, static_cast<float>(window.getSize().y) - 80);
    infoPanel.setFillColor(sf::Color(50, 50, 50, 200));
    window.draw(infoPanel);

    sf::Text mapInfo;
    mapInfo.setFont(font);
    mapInfo.setString("Map: " + std::to_string(gameState.getMapWidth()) + "x" +
                     std::to_string(gameState.getMapHeight()) +
                     "  Players: " + std::to_string(gameState.getPlayers().size()));
    mapInfo.setCharacterSize(16);
    mapInfo.setFillColor(sf::Color::White);
    mapInfo.setPosition(10, static_cast<float>(window.getSize().y) - 70);

    window.draw(mapInfo);
    sf::Text playerList;
    playerList.setFont(font);
    std::string playerText = "Teams: ";

    std::map<std::string, int> teamCounts;
    for (const Player& player : gameState.getPlayers()) {
        teamCounts[player.team_name]++;
    }

    for (const auto& team : teamCounts) {
        playerText += team.first + "(" + std::to_string(team.second) + ") ";
    }

    playerList.setString(playerText);
    playerList.setCharacterSize(14);
    playerList.setFillColor(sf::Color::White);
    playerList.setPosition(10, static_cast<float>(window.getSize().y) - 45);

    window.draw(playerList);
}

sf::Color Renderer::getResourceColor(const std::vector<int>& resources) {
    int maxRes = 0;
    int maxIndex = 0;

    for (size_t i = 0; i < resources.size(); ++i) {
        if (resources[i] > maxRes) {
            maxRes = resources[i];
            maxIndex = i;
        }
    }
    if (maxRes == 0) {
        return sf::Color(80, 80, 80);
    }
    switch (maxIndex) {
        case 0: return sf::Color(139, 69, 19);
        case 1: return sf::Color(192, 192, 192);
        case 2: return sf::Color(255, 215, 0);
        case 3: return sf::Color(0, 0, 255);
        case 4: return sf::Color(128, 0, 128);
        case 5: return sf::Color(255, 165, 0);
        case 6: return sf::Color(255, 0, 0);
        default: return sf::Color(100, 100, 100);
    }
}

static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

void Renderer::loadTeamColors(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#')
            continue;
        auto sep = line.find(':');
        if (sep == std::string::npos) continue;
        std::string rgb = line.substr(sep + 1);
        int r, g, b;
        if (sscanf(rgb.c_str(), "%d,%d,%d", &r, &g, &b) == 3) {
            teamColorList.push_back(sf::Color(r, g, b));
        }
    }
}

sf::Color Renderer::getTeamColor(const std::string& team_name) {
    static size_t nextColor = 0;
    std::string key = toLower(team_name);
    key.erase(0, key.find_first_not_of(" \t\n\r"));
    key.erase(key.find_last_not_of(" \t\n\r") + 1);
    auto it = teamColorMap.find(key);
    if (it != teamColorMap.end())
        return it->second;
    sf::Color color = teamColorList.empty() ? sf::Color(128,128,128)
        : teamColorList[nextColor % teamColorList.size()];
    teamColorMap[key] = color;
    nextColor++;
    return color;
}

sf::Vector2f Renderer::worldToScreen(int world_x, int world_y) {
    return sf::Vector2f(
        offset.x + world_x * tile_size,
        offset.y + world_y * tile_size
    );
}

void Renderer::renderBackground(const Tile& tile) {

    static sf::Texture Texture;
    static bool isLoaded = false;

    if (!isLoaded) {
        if (!Texture.loadFromFile("../graphical/asset/coolgrass.png")) {
            std::cerr << "Erreur : impossible de charger steak.png" << std::endl;
            return;
        }
        isLoaded = true;
    }

    sf::Sprite Sprite;
    Sprite.setTexture(Texture);

    float scale = tile_size / Texture.getSize().x;
    Sprite.setScale(scale, scale);
    Sprite.setPosition(tile.x * tile_size + offset.x, tile.y * tile_size + offset.y);

    window.draw(Sprite);
}

void Renderer::renderResources(sf::RenderWindow& window, const Tile& tile) {
    if (tile.resources.size() != tile.resourceSprites.size() ||
        tile.resources.size() != tile.resourceTextures.size()) {
        std::cerr << "Erreur: tile mal initialisé — resources vs sprites mismatch\n";
        return;
    }

    const int gridSize = 3;
    const float spriteSize = tile_size / gridSize;

    std::vector<std::pair<sf::Sprite, int>> spritesToDraw;
    for (size_t i = 0; i < tile.resources.size(); ++i) {
        if (tile.resources[i] > 0) {
            sf::Sprite sprite = tile.resourceSprites[i];
            float texSizeX = static_cast<float>(tile.resourceTextures[i].getSize().x);
            float scale = spriteSize / texSizeX;
            sprite.setScale(scale, scale);
            spritesToDraw.emplace_back(sprite, i);
        }
    }

    const float baseX = tile.x * tile_size + offset.x;
    const float baseY = tile.y * tile_size + offset.y;

    for (size_t idx = 0; idx < spritesToDraw.size(); ++idx) {
        if (idx >= 9) break;

        int row = idx / gridSize;
        int col = idx % gridSize;

        float posX = baseX + col * spriteSize;
        float posY = baseY + row * spriteSize;

        sf::Sprite& sprite = spritesToDraw[idx].first;
        sprite.setPosition(posX, posY);
        window.draw(sprite);

        int resourceIndex = spritesToDraw[idx].second;
        int quantity = tile.resources[resourceIndex];
        // std::cout << quantity <<std::endl;
        sf::Text quantityText;
        quantityText.setFont(font);
        quantityText.setCharacterSize(10);
        quantityText.setFillColor(sf::Color::White);
        quantityText.setString(std::to_string(quantity));
        quantityText.setPosition(posX + 2, posY + 2);

        window.draw(quantityText);
    }
}

std::vector<int> Renderer::computeTotalResources(const GameState& gameState)
{
    const auto& map = gameState.getMap();
    std::vector<int> totals(7, 0);

    for (const auto& row : map) {
        for (const auto& tile : row) {
            for (size_t i = 0; i < tile.resources.size(); ++i) {
                totals[i] += tile.resources[i];
            }
        }
    }
    return totals;
}

void Renderer::drawResourceTotals(const GameState& gameState)
{
    static const std::vector<std::string> resourceNames = {
        "FOOD", "LINEMATE", "DERAUMERE", "SIBUR", "MENDIANE", "PHIRAS", "THYSTAME"
    };

    static const std::vector<sf::Color> resourceColors = {
        sf::Color::Yellow,
        sf::Color(100, 255, 100),
        sf::Color::Cyan,
        sf::Color::Red,
        sf::Color(150, 255, 150),
        sf::Color(100, 100, 255),
        sf::Color(255, 100, 255)
    };

    std::vector<int> totals = computeTotalResources(gameState);

    float startX = gameState.getMapWidth() * tile_size + offset.x + 20.0f;
    float startY = offset.y;
    float lineSpacing = 24.0f;

    for (size_t i = 0; i < resourceNames.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(resourceColors[i]);
        text.setString(resourceNames[i] + ": " + std::to_string(totals[i]));
        text.setPosition(startX, startY + i * lineSpacing);
        window.draw(text);
    }
}

void Renderer::playBackgroundMusic(const std::string& filepath)
{
    if (_musicPlaying) return;

    if (!backgroundMusic.openFromFile(filepath)) {
        std::cerr << "Erreur : impossible de charger la musique : " << filepath << std::endl;
        return;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(100.0f);
    backgroundMusic.play();
    _musicPlaying = true;
}

void Renderer::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= frameDuration) {
        animationFrame = (animationFrame + 1) % frameCols;
        animationTimer = 0.0f;
    }
}

void Renderer::renderPlayers(const GameState& gameState) {
    for (const Player& player : gameState.getPlayers()) {
        renderPlayer(player);
    }
}
