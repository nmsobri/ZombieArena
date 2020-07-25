#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

namespace game {
    class Texture {
      private:
        Texture();
        /* A map container from the STL,  that holds related pairs of String and Texture */
        std::map<std::string, sf::Texture> textures;

        /* A pointer of the same type as the class itself  the one and only instance */
        static Texture* instance;

      public:
        static sf::Texture& getTexture(std::string fileName);
    };
}