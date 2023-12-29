#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class TargetManager {
  private:
    std::vector<sf::RectangleShape> targets;

  public:
    std::vector<sf::RectangleShape> getTargets() {
        return targets;
    }

    void reset(int levelNumber) {
        targets.clear();
        if (levelNumber == 1) {
            setupLevelOne();
        }
    }

    void setupLevelOne() {
        int rowMax = 4;
        int colMax = 10;

        for (int r = 0; r < rowMax; ++r) {
            for (int c = 0; c < colMax; ++c) {
                sf::RectangleShape target;
                target.setSize(sf::Vector2f(50.f, 20.f));
                target.setPosition(sf::Vector2f(20.f + (c * 60.f), 100.f + (r * 30.f)));
                targets.push_back(target);
            }
        }
    }
    void damage(sf::Vector2f location) {
        for (sf::RectangleShape &target : targets) {
            if (target.getPosition().x == location.x && target.getPosition().y == location.y) {
                // std::cout << "**************************************************";
                target.setPosition(sf::Vector2f(-100.f, -100.f));
                target.setFillColor(sf::Color::Blue);
            }
        }
    }
};
