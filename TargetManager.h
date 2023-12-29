#pragma once
#include "target.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class TargetManager {
  private:
    std::vector<Target> targets;

  public:
    std::vector<Target> getTargets() {
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
        int hp = 4;

        for (int r = 0; r < rowMax; ++r) {
            for (int c = 0; c < colMax; ++c) {
                Target target = Target(hp, 20.f + (c * 60.f), 100.f + (r * 30.f), 50.f, 20.f);
                targets.push_back(target);
            }
        }
    }
    void damage(sf::Vector2f location) {
        for (Target &target : targets) {
            if (target.getPosition().x == location.x && target.getPosition().y == location.y) {
                target.damage(1);
            }
        }
    }
};
