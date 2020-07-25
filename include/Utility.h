#pragma once
#include <SFML/Graphics.hpp>
#include "Zombie.h"

namespace game {
    class Utility {
      public:
        static int createBackground(sf::VertexArray& rVA, sf::IntRect& arena);
        static Zombie* createHorde(int numZombies, sf::IntRect arena);
    };
}
