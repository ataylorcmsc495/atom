#include <SFML/Graphics.hpp>
#include <iostream>

class Debug {
  public:
    static void debug(std::string message) { std::cout << message << std::endl; }

    static std::string printResolutionX() {
        return std::to_string(sf::VideoMode::getDesktopMode().width);
    }

    static std::string printResolutionY() {
        return std::to_string(sf::VideoMode::getDesktopMode().height);
    }

    static std::string printResolution() {
        return "Screen Resolution: " + printResolutionX() + "x" + printResolutionY();
    }

    static std::string printWindowSize(sf::RenderWindow *window) {
        return "Window Size: " + std::to_string(window->getSize().x) + "x" + std::to_string(window->getSize().y);
    }

    static std::string printWindowPosition(sf::RenderWindow *window) {
        return "Window Postion: " + std::to_string(window->getPosition().x) + "x" + std::to_string(window->getPosition().y);
    }

    static std::string printMousePosition() {
        return "Mouse Position (Abs): " + std::to_string(sf::Mouse::getPosition().x) + "x" + std::to_string(sf::Mouse::getPosition().y);
    }

    static std::string printMousePosition(sf::RenderWindow *window) {
        return "Mouse Position (Rel): " + std::to_string(sf::Mouse::getPosition(*window).x) + "x" + std::to_string(sf::Mouse::getPosition(*window).y);
    }
};
