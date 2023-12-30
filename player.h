#pragma once
#include <SFML/Graphics.hpp>

class Player {
  private:
    sf::RectangleShape _player;
    const float _playerMaxSpeed = 200.0f;
    const sf::Vector2f _playerSize = sf::Vector2f(50.f, 10.f);
    sf::Vector2f _startPos;
    float _maxPosX;

  public:
    Player(sf::Vector2f dim) {
        _player.setSize(_playerSize);
        _startPos.x = (dim.x / 2) - (_playerSize.x / 2);
        _startPos.y = dim.y - 10.f;
        _maxPosX = dim.x;
        _player.setPosition(_startPos);
    }
    sf::RectangleShape getBoundingBox() {
        return _player;
    }
    sf::RectangleShape getDrawableShape() {
        return _player;
    }
    void moveLeft(float deltaTime) {
        if (_player.getPosition().x > 0) {
            const float x = _player.getPosition().x - (_playerMaxSpeed * deltaTime);
            const float y = _startPos.y;
            _player.setPosition(sf::Vector2f(x, y));
        }
    }
    void moveRight(float deltaTime) {
        if (_player.getPosition().x < _maxPosX) {
            const float x = _player.getPosition().x + (_playerMaxSpeed * deltaTime);
            const float y = _startPos.y;
            _player.setPosition(sf::Vector2f(x,y));
        }
    }
    float getPlatformLeftCoord() {
        return getBoundingBox().getPosition().x;
    }
    float getPlatformRightCoord() {
        return getPlatformLeftCoord() + _player.getSize().x;
    }
};
