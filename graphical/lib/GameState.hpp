/*
** EPITECH PROJECT, 2025
** GameState
** File description:
** Manages state of the game
*/

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

struct Player {
    int id;
    int x, y;
    int orientation;
    int level;
    std::string team_name;

    Player(int id, int x, int y, int orientation, int level, const std::string& team)
        : id(id), x(x), y(y), orientation(orientation), level(level), team_name(team) {}
};

struct Tile {
    int x, y;
    std::vector<int> resources;
    std::vector<sf::Texture> resourceTextures;
    std::vector<sf::Sprite> resourceSprites;

    void initResources();

    Tile() : x(0), y(0), resources(7, 0) {}
    Tile(int x, int y) : x(x), y(y), resources(7, 0) {}
};

class GameState {
public:
    GameState();

    void setMapSize(int width, int height);
    void updateTile(int x, int y, const std::vector<int>& resources);
    void addPlayer(int id, int x, int y, int orientation, int level, const std::string& team);
    void updatePlayerPosition(int id, int x, int y, int orientation);
    void updatePlayerLevel(int id, int level);
    void removePlayer(int id);
    int getMapWidth() const { return map_width; }
    int getMapHeight() const { return map_height; }
    const Tile& getTile(int x, int y) const;
    const std::vector<Player>& getPlayers() const { return players; }
    const Player* getPlayer(int id) const;
    void processServerMessage(const std::string& message);
    const std::vector<std::vector<Tile>>& getMap() const;


private:
    int map_width, map_height;
    std::vector<std::vector<Tile>> map;
    std::vector<Player> players;
    std::unordered_map<int, size_t> player_indices;
    void parseMapSize(const std::string& line);
    void parseTileContent(const std::string& line);
    void parseNewPlayer(const std::string& line);
    void parsePlayerPosition(const std::string& line);
    void parsePlayerLevel(const std::string& line);
    void parsePlayerDeath(const std::string& line);
};
