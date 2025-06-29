/*
** EPITECH PROJECT, 2025
** Window implementation
** File description:
** zappy
*/

#include "../lib/window.hpp"

GameWindow::GameWindow(unsigned int width, unsigned int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title)
{
    m_window.create(sf::VideoMode(width, height), title);
    m_window.setFramerateLimit(60);
}

bool GameWindow::pollEvent(sf::Event& event) {
    return m_window.pollEvent(event);
}

bool GameWindow::isOpen() const {
    return m_window.isOpen();
}

void GameWindow::close() {
    m_window.close();
}

void GameWindow::clear(sf::Color color) {
    m_window.clear(color);
}

void GameWindow::display() {
    m_window.display();
}