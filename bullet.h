#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
  private:
    sf::RectangleShape _shape;
    const float _speedDefault = 0.f;
    const float _sizeDefault = 5.0f;
    sf::Vector2f _velocity = sf::Vector2f(0.f, 0.f);
    sf::Vector2f _viewSize;
    bool _armed;

  public:
    Bullet(sf::Vector2f viewSize, float maxSpeed)
        : _viewSize(viewSize), _speedDefault(maxSpeed) {
        init();
    }
    void init() {
        _shape.setSize(sf::Vector2f(_sizeDefault, _sizeDefault));
        resetPosition();
        resetVelocity();
    }
    void disarm() {
        _armed = false;
    }
    void arm() {
        _armed = true;
    }
    bool isArmed() {
        return _armed;
    }

    bool notInView(sf::Vector2f dim) {
        if (_shape.getPosition().y < 0)
            return true;
        if ((_shape.getPosition().y + _shape.getSize().y) > dim.y)
            return true;
        if (_shape.getPosition().x < 0)
            return true;
        if ((_shape.getPosition().x + _shape.getSize().x) > dim.x)
            return true;
        return false;
    }

    void resetPosition() {
        _shape.setPosition(sf::Vector2f(_viewSize.x / 2, _viewSize.y / 2));
    }
    void resetVelocity() {
        _velocity.x = 0;
        _velocity.y = _speedDefault;
    }

    void setMaxVelocityX() {
        _velocity.x = _speedDefault;
    }

    void scaleVelocityX(float multiplier) {
        _velocity.x *= multiplier;
    }
    void scaleVelocityY(float multiplier) {
        _velocity.y *= multiplier;
    }
    void reboundX() {
        scaleVelocityX(-1);
    }
    void reboundY() {
        scaleVelocityY(-1);
    }
    float getVelocityX() {
        return _velocity.x;
    }
    float getVelocityY() {
        return _velocity.y;
    }
    void move(float delta) {
        float targetHypSqd = _speedDefault * _speedDefault;
        float currentHypSqd = (_velocity.x * _velocity.x) + (_velocity.y * _velocity.y);
        float scaleFactor = 0;
        if (currentHypSqd != 0) {
            scaleFactor = targetHypSqd / currentHypSqd;
        }
        _velocity.x *= scaleFactor;
        _velocity.y *= scaleFactor;

        _shape.setPosition(_shape.getPosition().x + (_velocity.x * delta), _shape.getPosition().y + (_velocity.y * delta));
    }
    bool hitCeiling() {
        return (_shape.getPosition().y <= 0);
    }
    bool hitFloor() {
        return ((_shape.getPosition().y + _shape.getSize().y) > _viewSize.y);
    }
    bool hitWalls() {
        if (_shape.getPosition().x <= 0)
            return true;
        if (_shape.getPosition().x + _shape.getSize().x >= _viewSize.x)
            return true;
        return false;
    }
    bool hitLeftWall() {
        return _shape.getPosition().x <= 0;
    }
    bool hitRightWall() {
        return (_shape.getPosition().x + _shape.getSize().x) >= _viewSize.x;
    }
    sf::RectangleShape getBoundingBox() {
        return _shape;
    }
    sf::RectangleShape getDrawableShape() {
        return _shape;
    }
    float getCenterX() {
        return ((_shape.getPosition().x) / 2) + ((_shape.getPosition().x + _shape.getSize().x) / 2);
    }
};
