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
    sf::Vector2f dim(640.f, 480.f);
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setPosition(sf::Vector2i(2000, 200));
    sf::View view(sf::FloatRect(0.f, 0.f, dim.x, dim.y));
    window.setView(view);
    sf::CircleShape shape(100.f);
    sf::Texture bgTexture;
    bgTexture.loadFromFile("vga.bmp");
    sf::Sprite background;
    background.setTexture(bgTexture);
    background.setTextureRect(sf::IntRect(0, 0, (int)dim.x, (int)dim.y));

    background.setScale(1.0f, 1.0f);

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
            if (event.type == sf::Event::Resized) {
                view.setSize(dim.x, dim.y);
                float viewToWindowRatioX = dim.x / event.size.width;
                float viewToWindowRatioY = dim.y / event.size.height;
                float letterboxX = (1 - viewToWindowRatioX) / 2;
                float letterboxY = (1 - viewToWindowRatioY) / 2;

                view.setViewport(sf::FloatRect(letterboxX, letterboxY, viewToWindowRatioX, viewToWindowRatioY));
                window.setView(view);

                // sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                // window.setView(sf::View(visibleArea));
            }
        }

        window.clear();
        window.draw(background);
        //    window.draw(shape);
        window.display();
    }

    return 0;
}
