#pragma once
#include <SFML/Graphics.hpp>

class Scoreboard {
  private:
    sf::Font _font;
    sf::Text _scoreText;

    const std::string _fontFile = "SauceCodeProNerdFont-Regular.ttf";
    const std::string _leader = "SCORE: ";
    const int _fontSize = 24;
    const sf::Color _fontColor = sf::Color::Magenta;

    int _score = 0;

  public:
    Scoreboard(float xPos, float yPos) {
        _font.loadFromFile(_fontFile);
        _scoreText.setFont(_font);
        _scoreText.setCharacterSize(_fontSize);
        _scoreText.setFillColor(_fontColor);
        _scoreText.setStyle(sf::Text::Regular);

        setPosition(xPos, yPos);
        updateScoreboardText();
    }

    void setScore(int score) {
        _score = score;
    }

    void addToScore(int points) {
        _score += points;
    }

    void updateScoreboardText() {
        _scoreText.setString(_leader + std::to_string(_score));
    }

    void setPosition(float xPos, float yPos) {
        _scoreText.setPosition(sf::Vector2f(xPos, yPos));
    }
    sf::Text getDrawableShape() {
        return _scoreText;
    }
};
