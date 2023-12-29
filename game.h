#include "TargetManager.h"
#include "bullet.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Game {
  private:
    std::string bgFilename = "vga.bmp";

    sf::Vector2f dim = sf::Vector2f(640.f, 480.f);
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800, 600), "This is ATOM");
    sf::View view = sf::View(sf::FloatRect(0.f, 0.f, dim.x, dim.y));
    sf::Texture bgTexture;
    sf::Sprite background;

    sf::RectangleShape player;
    float playerSpeed = 0.f;

    Bullet bullet2 = Bullet(dim);

    TargetManager targets;

  public:
    Game(sf::Vector2i windowStartPos) {
        window.setPosition(windowStartPos); //200,200
        window.setView(view);
        bgTexture.loadFromFile(bgFilename);
        background.setTexture(bgTexture);
        background.setTextureRect(sf::IntRect(0, 0, (int)dim.x, (int)dim.y));
        background.setScale(1.0f, 1.0f);
        player.setSize(sf::Vector2f(50.f, 10.f));
        player.setPosition(sf::Vector2f(dim.x / 2 - player.getSize().x / 2, dim.y - 10.f));
        targets.reset(1);
    }
    bool isRunning() {
        return window.isOpen();
    }
    void update() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Debug::debug("-----------------------");
            //            debug(printResolution());
            //            debug(printWindowSize(&window));
            //            debug(printWindowPosition(&window));
            //            debug(printMousePosition());
            //            debug(printMousePosition(&window));
            if (event.type == sf::Event::Resized) {
                view.setSize(dim.x, dim.y);
                float viewToWindowRatioX = dim.x / event.size.width;
                float viewToWindowRatioY = dim.y / event.size.height;
                float letterboxX = (1 - viewToWindowRatioX) / 2;
                float letterboxY = (1 - viewToWindowRatioY) / 2;

                view.setViewport(sf::FloatRect(letterboxX, letterboxY, viewToWindowRatioX, viewToWindowRatioY));
                window.setView(view);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                bullet2.init();
                targets.reset(1);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    playerSpeed = -1.5f;
                }
                if (event.key.code == sf::Keyboard::D) {
                    playerSpeed = 1.5f;
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
        if (player.getPosition().x < 0 && playerSpeed < 0) {
            playerSpeed = 0;
        }
        if ((player.getPosition().x + player.getSize().x) > dim.x && playerSpeed > 0) {
            playerSpeed = 0;
        }

        if (bullet2.hitFloor()) {
            bullet2.init();
        }

        player.setPosition(sf::Vector2f(player.getPosition().x + playerSpeed, player.getPosition().y));

        if (Game::isCollision(player, bullet2.getBoundingBox())) {
            bullet2.reboundY();
            float mult = Game::normalizedHitPosition(bullet2.getCenterX(), player.getPosition().x, player.getPosition().x + player.getSize().x);
            bullet2.setMaxVelocityX();
            bullet2.scaleVelocityX((2 * mult) - 1);
        }

        // Bullet Hit Screen Edge
        if (bullet2.hitCeiling() && bullet2.getVelocityY() < 0) {
            bullet2.reboundY();
        } else if (bullet2.hitLeftWall() && bullet2.getVelocityX() < 0) {
            bullet2.reboundX();
        } else if (bullet2.hitRightWall() && bullet2.getVelocityX() > 0) {
            bullet2.reboundX();
        }


        // Bullet Hit A Target?
        for (sf::RectangleShape target : targets.getTargets()) {
            if (Game::isCollision(bullet2.getBoundingBox(), target)) {
                sf::Vector2f rebound = Game::calcReboundVector(bullet2.getBoundingBox(), target);
                bullet2.scaleVelocityX(rebound.x);
                bullet2.scaleVelocityY(rebound.y);
                targets.damage(target.getPosition());
            }
        }

        bullet2.move();

    }

    void render() {
        window.clear();
        // window.draw(background);
        window.draw(player);
        for (sf::RectangleShape target : targets.getTargets()) {
            if (target.getPosition().x > 0) {
                window.draw(target);
            }
        }
        window.draw(bullet2.getDrawableShape());
        window.display();
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
        if (t < 0 || t > 1)
            t = -1;
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
            std::cout << "p is on the left" << std::endl;
            rebound.x = -1;
        } else if (px1 > sx2 - fudgeFactor) {
            // p on the right
            std::cout << "p is on the right" << std::endl;
            rebound.x = -1;
        } else if (py2 < sy1 + fudgeFactor) {
            // p is on top
            std::cout << "p is on top" << std::endl;
            rebound.y = -1;
        } else if (py1 > sy2 - fudgeFactor) {
            // p is under
            std::cout << "p is under" << std::endl;
            rebound.y = -1;
        } else {
            std::cout << "p is inside" << std::endl;
            // p is inside
        }

        return rebound;
    }
};
