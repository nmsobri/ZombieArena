#include "include/Texture.h"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <assert.h>

game::Texture* game::Texture::instance = nullptr;

game::Texture::Texture()
    : textures() {
}

sf::Texture& game::Texture::getTexture(std::string fileName) {
    if (instance == nullptr) {
        instance = new Texture();
    }
    // Get a reference to m_Textures using instance
    // auto is the equivalent of map<string, Texture>
    auto& m = instance->textures;

    // Create an iterator to hold a key-value-pair (kvp)
    // and search for the required kvp
    // using the passed in file name
    auto found = m.find(fileName);
    // auto is equivelant of map<string, Texture>::iterator

    // Did we find a match?
    if (found != m.end()) {
        // Yes
        // Return the texture,
        // the second part of the kvp, the texture
        return found->second;
    } else {
        // File name not found, create a new key value pair using the filename
        auto& texture = m[fileName];

        // Load the texture from file in the usual way
        texture.loadFromFile(fileName);

        // Return the texture to the calling code
        return texture;
    }
}