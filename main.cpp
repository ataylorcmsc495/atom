#include <SFML/Graphics.hpp>

#include "debug.h"
#include "game.h"

int main() {
    Game game(sf::Vector2i(200, 200));

    while (game.isRunning()) {
        game.update();
        game.render();
    }

    return 0;
}
