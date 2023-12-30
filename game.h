#pragma once
#include "TargetManager.h"
#include "background.h"
#include "bullet.h"
#include "gameclock.h"
#include "player.h"
#include "scoreboard.h"
#include "target.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

class Game {
  private:
    const bool bouncyFloor = true;

    sf::Vector2f _dim = sf::Vector2f(640.f, 480.f);
    sf::RenderWindow _window = sf::RenderWindow(sf::VideoMode(800, 600), "This is ATOM");
    sf::View _view = sf::View(sf::FloatRect(0.f, 0.f, _dim.x, _dim.y));

    Background _background = Background("vga.bmp", (int)_dim.x, (int)_dim.y);
    Scoreboard _scoreboard = Scoreboard(0, 0);
    GameClock _clock;

    Player _player = Player(_dim);
    bool _controlLeft = false;
    bool _controlRight = false;

    const float _playerMaxSpeed = 200.0f;
    const float _bulletMaxSpeed = 300.0f;
    float gameLevel = 1;

    Bullet _bullet = Bullet(_dim, _bulletMaxSpeed);
    TargetManager _targets;

  public:
    Game(sf::Vector2i windowStartPos) {
        _window.setPosition(windowStartPos);
        _window.setView(_view);
        _targets.reset(gameLevel);
    }
    bool isRunning() {
        return _window.isOpen();
    }
    void update() {
        float deltaTime = _clock.refreshDeltaTime();

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
                _bullet.init();
                _targets.reset(1);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    _controlLeft = true;
                    _controlRight = false;
                }
                if (event.key.code == sf::Keyboard::D) {
                    _controlRight = true;
                    _controlLeft = false;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::A) {
                    _controlLeft = false;
                }
                if (event.key.code == sf::Keyboard::D) {
                    _controlRight = false;
                }
            }
        }

        if (Game::isCollision(_player.getBoundingBox(), _bullet.getBoundingBox())) {
            _bullet.reboundY();
            float mult = Game::normalizedHitPosition(_bullet.getCenterX(), _player.getPlatformLeftCoord(), _player.getPlatformRightCoord());
            _bullet.setMaxVelocityX();
            _bullet.scaleVelocityX((2 * mult) - 1);
        }

        // Bullet Hit Screen Edge?
        if (_bullet.hitFloor()) {
            if (bouncyFloor) {
                if (_bullet.getVelocityY() > 0)
                    _bullet.reboundY();
            } else {
                _bullet.init();
            }
        } else if (_bullet.hitCeiling() && _bullet.getVelocityY() < 0) {
            _bullet.reboundY();
        } else if (_bullet.hitLeftWall() && _bullet.getVelocityX() < 0) {
            _bullet.reboundX();
        } else if (_bullet.hitRightWall() && _bullet.getVelocityX() > 0) {
            _bullet.reboundX();
        }

        // Bullet Hit A Target?
        bool anyCollisions = false;
        for (Target target : _targets.getTargets()) {
            if (Game::isCollision(_bullet.getBoundingBox(), target.getBoundingBox())) {
                anyCollisions = true;
                sf::Vector2f rebound = Game::calcReboundVector(_bullet.getBoundingBox(), target.getBoundingBox());
                if (_bullet.isArmed()) {
                    _bullet.scaleVelocityX(rebound.x);
                    _bullet.scaleVelocityY(rebound.y);
                    _targets.damage(target.getPosition());

                    _scoreboard.addToScore(1);
                    _scoreboard.updateScoreboardText();

                    _bullet.disarm();
                }
            }
        }
        if (!anyCollisions) {
            _bullet.arm();
        }

        if (_controlLeft) {
            _player.moveLeft(deltaTime);
        } else if (_controlRight) {
            _player.moveRight(deltaTime);
        }
        _bullet.move(deltaTime);
    }

    void render() {
        _window.clear();
        // _window.draw(_background.getDrawableShape());
        _window.draw(_player.getDrawableShape());
        for (Target target : _targets.getTargets()) {
            if (target.isAlive()) {
                _window.draw(target.getDrawableShape());
            }
        }
        _window.draw(_bullet.getDrawableShape());
        _window.draw(_scoreboard.getDrawableShape());
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
