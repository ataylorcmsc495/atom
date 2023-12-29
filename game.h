#pragma once
#include "TargetManager.h"
#include "bullet.h"
#include "target.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

class Game {
  private:
    std::string bgFilename = "vga.bmp";

    sf::Vector2f _dim = sf::Vector2f(640.f, 480.f);
    sf::RenderWindow _window = sf::RenderWindow(sf::VideoMode(800, 600), "This is ATOM");
    sf::View _view = sf::View(sf::FloatRect(0.f, 0.f, _dim.x, _dim.y));
    sf::Texture _bgTexture;
    sf::Sprite _background;

    sf::Font _font;
    sf::Text _score;
    int _playerScore = 0;

    sf::RectangleShape player;
    float playerSpeed = 0.f;
    float playerMaxSpeed = 200.0f;
    float bulletMaxSpeed = 300.0f;
    sf::Vector2f _playerSize = sf::Vector2f(50.f, 10.f);
    float gameLevel = 1;

    Bullet bullet2 = Bullet(_dim, bulletMaxSpeed);

    TargetManager targets;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = now();

    std::chrono::time_point<std::chrono::high_resolution_clock> now() {
        return std::chrono::high_resolution_clock::now();
    }

    float deltaTime = 0.0;
    void setDeltaTime() {
        deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now() - lastTime).count();
    }
    void updateLastTime() {
        lastTime = now();
    }

  public:
    Game(sf::Vector2i windowStartPos) {
        _window.setPosition(windowStartPos);
        _window.setView(_view);
        _bgTexture.loadFromFile(bgFilename);
        _background.setTexture(_bgTexture);
        _background.setTextureRect(sf::IntRect(0, 0, (int)_dim.x, (int)_dim.y));
        _background.setScale(1.0f, 1.0f);
        player.setSize(_playerSize);
        player.setPosition(sf::Vector2f(_dim.x / 2 - player.getSize().x / 2, _dim.y - 10.f));
        targets.reset(gameLevel);

        _font.loadFromFile("SauceCodeProNerdFont-Regular.ttf");
        _score.setFont(_font);
        _score.setString("SCORE: 0");
        _score.setCharacterSize(24);
        _score.setFillColor(sf::Color::Magenta);
        _score.setStyle(sf::Text::Regular);
        _score.setPosition(sf::Vector2f(0.f,0.f));
    
    }
    bool isRunning() {
        return _window.isOpen();
    }
    void updateScore() {
        _score.setString("SCORE: " + std::to_string(_playerScore));
    }
    void update() {
        setDeltaTime();
        updateLastTime();
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window.close();
            }
            if (event.type == sf::Event::Resized) {
                _view.setSize(_dim.x, _dim.y);
                float viewToWindowRatioX = _dim.x / event.size.width;
                float viewToWindowRatioY = _dim.y / event.size.height;
                float letterboxX = (1 - viewToWindowRatioX) / 2;
                float letterboxY = (1 - viewToWindowRatioY) / 2;

                _view.setViewport(sf::FloatRect(letterboxX, letterboxY, viewToWindowRatioX, viewToWindowRatioY));
                _window.setView(_view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                _window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                bullet2.init();
                targets.reset(1);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    playerSpeed = -1 * playerMaxSpeed * deltaTime;
                }
                if (event.key.code == sf::Keyboard::D) {
                    playerSpeed = playerMaxSpeed * deltaTime;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::A && playerSpeed < 0) {
                    playerSpeed = 0;
                }
                if (event.key.code == sf::Keyboard::D && playerSpeed > 0) {
                    playerSpeed = 0;
                }
            }
        }

        // Zero out the player speed if at edge of screen
        if (player.getPosition().x < 0 && playerSpeed < 0) {
            playerSpeed = 0;
        } else if ((player.getPosition().x + player.getSize().x) > _dim.x && playerSpeed > 0) {
            playerSpeed = 0;
        }

        if (Game::isCollision(player, bullet2.getBoundingBox())) {
            bullet2.reboundY();
            float mult = Game::normalizedHitPosition(bullet2.getCenterX(), player.getPosition().x, player.getPosition().x + player.getSize().x);
            bullet2.setMaxVelocityX();
            bullet2.scaleVelocityX((2 * mult) - 1);
        }

        // Bullet Hit Screen Edge?
        if (bullet2.hitFloor()) {
            // bullet2.init();
            if (bullet2.getVelocityY() > 0)
                bullet2.reboundY();
        } else if (bullet2.hitCeiling() && bullet2.getVelocityY() < 0) {
            bullet2.reboundY();
        } else if (bullet2.hitLeftWall() && bullet2.getVelocityX() < 0) {
            bullet2.reboundX();
        } else if (bullet2.hitRightWall() && bullet2.getVelocityX() > 0) {
            bullet2.reboundX();
        }

        // Bullet Hit A Target?
        bool anyCollisions = false;
        for (Target target : targets.getTargets()) {
            if (Game::isCollision(bullet2.getBoundingBox(), target.getBoundingBox())) {
                anyCollisions = true;
                sf::Vector2f rebound = Game::calcReboundVector(bullet2.getBoundingBox(), target.getBoundingBox());
                if (bullet2.isArmed()) {
                    bullet2.scaleVelocityX(rebound.x);
                    bullet2.scaleVelocityY(rebound.y);
                    targets.damage(target.getPosition());
                    _playerScore++;
                    updateScore();
                    bullet2.disarm();
                }
            }
        }
        if (!anyCollisions) {
            bullet2.arm();
        }

        player.setPosition(sf::Vector2f(player.getPosition().x + playerSpeed, player.getPosition().y));
        bullet2.move(deltaTime);
    }

    void
    render() {
        _window.clear();
        // window.draw(background);
        _window.draw(player);
        for (Target target : targets.getTargets()) {
            if (target.isAlive()) {
                _window.draw(target.getDrawableShape());
            }
        }
        _window.draw(bullet2.getDrawableShape());
        _window.draw(_score);
        _window.display();
    }

    /* Static Functions */

    static bool isCollision(const sf::RectangleShape &shape1, const sf::RectangleShape &shape2) {
        if (shape1.getPosition().x + shape1.getSize().x > shape2.getPosition().x) {
            if (shape1.getPosition().x < shape2.getPosition().x + shape2.getSize().x) {
                if (shape1.getPosition().y + shape1.getSize().y > shape2.getPosition().y) {
                    if (shape1.getPosition().y < shape2.getPosition().y + shape2.getSize().y) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    static float normalizedHitPosition(const float bulletCenter, const float targetLeft, const float targetRight) {
        float x = bulletCenter - targetLeft;
        float w = targetRight - targetLeft;
        float t = x / w;
        if (t < 0.1)
            t = 0.1;
        if (t > 0.9)
            t = 0.9;
        return t;
    }

    static sf::Vector2f calcReboundVector(const sf::RectangleShape &projectile, const sf::RectangleShape &surface) {

        float px1 = projectile.getPosition().x;
        float px2 = projectile.getPosition().x + projectile.getSize().x;
        float py1 = projectile.getPosition().y;
        float py2 = projectile.getPosition().y + projectile.getSize().y;

        float sx1 = surface.getPosition().x;
        float sx2 = sx1 + surface.getSize().x;
        float sy1 = surface.getPosition().y;
        float sy2 = sy1 + surface.getSize().y;

        sf::Vector2f rebound(1, 1);
        float fudgeFactor = 2.5f;
        if (px2 < sx1 + fudgeFactor) {
            // p is on the left
            rebound.x = -1;
        } else if (px1 > sx2 - fudgeFactor) {
            // p on the right
            rebound.x = -1;
        } else if (py2 < sy1 + fudgeFactor) {
            // p is on top
            rebound.y = -1;
        } else if (py1 > sy2 - fudgeFactor) {
            // p is under
            rebound.y = -1;
        } else {
            // p is inside
        }

        return rebound;
    }
};
