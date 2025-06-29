/*
** EPITECH PROJECT, 2025
** main
** File description:
** zappy GUI main
*/

#include "../lib/ZappyGUI.hpp"
#include "../lib/Zappy3DRenderer.hpp"
#include "../lib/ZappyAbstractGUI.hpp"
#include <iostream>
#include <string>

void print_usage(const char* program_name) {
    std::cout << "USAGE: " << program_name << " -p port [-h machine] [-r 2d|3d]" << std::endl;
    std::cout << "DESCRIPTION:" << std::endl;
    std::cout << "\tport\t\tis the port number" << std::endl;
    std::cout << "\tmachine\t\tis the name of the machine; localhost by default" << std::endl;
    std::cout << "\t-r\t\tchoose render mode: 2d (default) or 3d" << std::endl;
}

int main(int argc, char** argv) {
    std::string ip = "localhost";
    int port = 4242;
    bool port_specified = false;
    std::string render_mode = "2d";

    for (int i = 1; i < argc - 1; i++) {
        if (std::string(argv[i]) == "-p") {
            port = std::atoi(argv[i + 1]);
            port_specified = true;
            i++;
        } else if (std::string(argv[i]) == "-h") {
            ip = argv[i + 1];
            i++;
        } else if (std::string(argv[i]) == "-r") {
            render_mode = argv[i + 1];
            i++;
        }
    }
    if (argc > 1) {
        if (std::string(argv[1]) == "--help") {
            print_usage(argv[0]);
            return 0;
        }
        if (!port_specified && argc > 1) {
            std::cerr << "Error: Port is required when using arguments" << std::endl;
            print_usage(argv[0]);
            return 84;
        }
    }

    std::cout << "Starting Zappy GUI..." << std::endl;
    std::cout << "Connecting to " << ip << ":" << port << std::endl;

    try {
        if (render_mode == "3d") {
            Zappy3DRenderer gui(ip, port);
            gui.run();
        } else {
            ZappyGUI gui(ip, port);
            gui.run();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
