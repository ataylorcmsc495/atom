#pragma once
#include <SFML/Graphics.hpp>

class Target {
  private:
    sf::RectangleShape _box;
    int _hp;

    void updateColor() {

        switch (_hp) {
        case 0:
            _box.setFillColor(sf::Color::Black);
            break;
        case 1:
            _box.setFillColor(sf::Color::White);
            break;
        case 2:
            _box.setFillColor(sf::Color::Blue);
            break;
        case 3:
            _box.setFillColor(sf::Color::Green);
            break;
        default:
            _box.setFillColor(sf::Color::Red);
        }
    }

  public:
    Target(int hp, float x, float y, float width, float height) : _hp(hp) {
        _box.setPosition(sf::Vector2f(x, y));
        _box.setSize(sf::Vector2f(width, height));
        updateColor();
    }
    sf::RectangleShape getBoundingBox() {
        return _box;
    }
    sf::RectangleShape getDrawableShape() {
        return _box;
    }
    sf::Vector2f getPosition() {
        return _box.getPosition();
    }
    bool isAlive() {
        if (_hp > 0)
            return true;
        return false;
    }
    void damage(int dmg) {
        _hp -= dmg;
        if (_hp < 0)
            _hp = 0;
        if (_hp == 0) {
            _box.setSize(sf::Vector2f(0,0));
            _box.setPosition(sf::Vector2f(-1,-1));
        }
        updateColor();
    }
};
