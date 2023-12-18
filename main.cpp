#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

void debug(std::string message) { std::cout << message << std::endl; }

std::string printResolutionX() {
    return std::to_string(sf::VideoMode::getDesktopMode().width);
}

std::string printResolutionY() {
    return std::to_string(sf::VideoMode::getDesktopMode().height);
}

std::string printResolution() {
    return "Screen Resolution: " + printResolutionX() + "x" + printResolutionY();
}

std::string printWindowSize(sf::RenderWindow *window) {
    return "Window Size: " + std::to_string(window->getSize().x) + "x" + std::to_string(window->getSize().y);
}

std::string printWindowPosition(sf::RenderWindow *window) {
    return "Window Postion: " + std::to_string(window->getPosition().x) + "x" + std::to_string(window->getPosition().y);
}

std::string printMousePosition() {
    return "Mouse Position (Abs): " + std::to_string(sf::Mouse::getPosition().x) + "x" + std::to_string(sf::Mouse::getPosition().y);
}

std::string printMousePosition(sf::RenderWindow *window) {
    return "Mouse Position (Rel): " + std::to_string(sf::Mouse::getPosition(*window).x) + "x" + std::to_string(sf::Mouse::getPosition(*window).y);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    window.setPosition(sf::Vector2i(2000, 200));
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            debug("-----------------------");
            debug(printResolution());
            debug(printWindowSize(&window));
            debug(printWindowPosition(&window));
            debug(printMousePosition());
            debug(printMousePosition(&window));
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
