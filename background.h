#pragma once

#include <SFML/Graphics.hpp>

class Background {
  private:
    sf::RectangleShape _background;

    std::string _filename;
    sf::Texture _texture;
    sf::Sprite _sprite;

  public:
    Background(std::string filename, int width, int height) : _filename(filename) {
        _texture.loadFromFile(_filename);
        _sprite.setTexture(_texture);
        _sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        _sprite.setScale(1.0f, 1.0f);
    }

    sf::RectangleShape getDrawableShape() {
        return _background;
    }
};
