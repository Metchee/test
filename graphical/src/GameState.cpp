/*
** EPITECH PROJECT, 2025
** GameState
** File description:
** GameState
*/

#include "../lib/GameState.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>

GameState::GameState() : map_width(0), map_height(0) {}

void GameState::setMapSize(int width, int height) {
    map_width = width;
    map_height = height;

    map.clear();
    map.resize(height);
    for (int y = 0; y < height; ++y) {
        map[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map[y][x] = Tile(x, y);
            map[y][x].initResources();
        }
    }
    std::cout << "Map initialized: " << width << "x" << height << std::endl;
}

void GameState::updateTile(int x, int y, const std::vector<int>& resources) {
    if (x < 0 || y < 0 || x >= map_width || y >= map_height) {
        std::cerr << "Invalid tile coordinates: " << x << ", " << y << std::endl;
        return;
    }
    map[y][x].resources = resources;
}

const Tile& GameState::getTile(int x, int y) const {
    static Tile empty_tile;
    if (x < 0 || y < 0 || x >= map_width || y >= map_height) {
        return empty_tile;
    }
    return map[y][x];
}

void GameState::addPlayer(int id, int x, int y, int orientation, int level, const std::string& team) {
    if (player_indices.find(id) != player_indices.end()) {
        std::cout << "Player " << id << " already exists, updating position" << std::endl;
        updatePlayerPosition(id, x, y, orientation);
        return;
    }

    players.emplace_back(id, x, y, orientation, level, team);
    player_indices[id] = players.size() - 1;
    std::cout << "Added player " << id << " (" << team << ") at (" << x << ", " << y << ")" << std::endl;
}

void GameState::updatePlayerPosition(int id, int x, int y, int orientation) {
    auto it = player_indices.find(id);
    if (it != player_indices.end()) {
        Player& player = players[it->second];
        player.x = x;
        player.y = y;
        player.orientation = orientation;
        std::cout << "Updated player " << id << " position to (" << x << ", " << y << ")" << std::endl;
    }
}

void GameState::updatePlayerLevel(int id, int level) {
    auto it = player_indices.find(id);
    if (it != player_indices.end()) {
        players[it->second].level = level;
        std::cout << "Updated player " << id << " level to " << level << std::endl;
    }
}

void GameState::removePlayer(int id) {
    auto it = player_indices.find(id);
    if (it != player_indices.end()) {
        size_t index = it->second;

        players.erase(players.begin() + index);
        player_indices.erase(it);

        for (auto& pair : player_indices) {
            if (pair.second > index) {
                pair.second--;
            }
        }
        std::cout << "Removed player " << id << std::endl;
    }
}

const Player* GameState::getPlayer(int id) const {
    auto it = player_indices.find(id);
    if (it != player_indices.end())
        return &players[it->second];
    return nullptr;
}

void GameState::processServerMessage(const std::string& message) {
    std::istringstream iss(message);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.empty()) continue;

        if (line.rfind("msz", 0) == 0)
            parseMapSize(line);
        else if (line.rfind("bct", 0) == 0)
            parseTileContent(line);
        else if (line.rfind("pnw", 0) == 0)
            parseNewPlayer(line);
        else if (line.rfind("ppo", 0) == 0)
            parsePlayerPosition(line);
        else if (line.rfind("plv", 0) == 0)
            parsePlayerLevel(line);
        else if (line.rfind("pdi", 0) == 0)
            parsePlayerDeath(line);
    }
}

void GameState::parseMapSize(const std::string& line) {
    std::istringstream iss(line.substr(4));
    int w, h;
    if (iss >> w >> h) {
        setMapSize(w, h);
    }
}

void GameState::parseTileContent(const std::string& line) {
    std::istringstream iss(line.substr(4));
    int x, y;
    std::vector<int> resources(7);

    if (iss >> x >> y) {
        for (int i = 0; i < 7; ++i) {
            iss >> resources[i];
        }
        updateTile(x, y, resources);
    }
}

void GameState::parseNewPlayer(const std::string& line) {
    std::istringstream iss(line.substr(4));
    int id, x, y, orientation, level;
    std::string team_name;

    if (iss >> id >> x >> y >> orientation >> level >> team_name) {
        addPlayer(id, x, y, orientation, level, team_name);
    }
}

void GameState::parsePlayerPosition(const std::string& line) {
    std::istringstream iss(line.substr(4));
    int id, x, y, orientation;

    if (iss >> id >> x >> y >> orientation) {
        updatePlayerPosition(id, x, y, orientation);
    }
}

void GameState::parsePlayerLevel(const std::string& line) {
    std::istringstream iss(line.substr(4));
    int id, level;

    if (iss >> id >> level) {
        updatePlayerLevel(id, level);
    }
}

void GameState::parsePlayerDeath(const std::string& line) {
    std::istringstream iss(line.substr(4));
    int id;

    if (iss >> id) {
        removePlayer(id);
    }
}

void Tile::initResources() {
    std::vector<std::string> paths = {
        "../graphical/asset/steak.png",
        "../graphical/asset/linemate.png",
        "../graphical/asset/deraumere.png",
        "../graphical/asset/sibur.png",
        "../graphical/asset/mendiane.png",
        "../graphical/asset/phiras.png",
        "../graphical/asset/thystame.png"
    };

    resourceTextures.resize(paths.size());
    resourceSprites.resize(paths.size());

    for (size_t i = 0; i < paths.size(); ++i) {
        if (!resourceTextures[i].loadFromFile(paths[i])) {
            std::cerr << "Erreur chargement " << paths[i] << std::endl;
        }
        resourceSprites[i].setTexture(resourceTextures[i]);
        resourceSprites[i].setScale(0.5f, 0.5f);
    }
}

const std::vector<std::vector<Tile>>& GameState::getMap() const {
    return map;
}
