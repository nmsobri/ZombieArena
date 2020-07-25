#include "include/Utility.h"
#include <iostream>
#include "include/Zombie.h"

int game::Utility::createBackground(sf::VertexArray& vertex, sf::IntRect& arena) {
    const int TILE_SIZE = 50;
    const int VERTS_IN_QUAD = 4;
    const int TILE_WALL = 3;

    int tileWidthSize = arena.width / TILE_SIZE;
    int tileHeightSize = arena.height / TILE_SIZE;

    vertex.setPrimitiveType(sf::Quads);
    vertex.resize(tileWidthSize * tileHeightSize * VERTS_IN_QUAD);

    int currentVertex = 0;

    for (int i = 0; i < tileWidthSize; i++) {
        for (int j = 0; j < tileHeightSize; j++) {
            vertex[currentVertex + 0].position = sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
            vertex[currentVertex + 1].position = sf::Vector2f((i * TILE_SIZE) + TILE_SIZE, j * TILE_SIZE);
            vertex[currentVertex + 2].position = sf::Vector2f((i * TILE_SIZE) + TILE_SIZE, (j * TILE_SIZE) + TILE_SIZE);
            vertex[currentVertex + 3].position = sf::Vector2f(i * TILE_SIZE, (j * TILE_SIZE) + TILE_SIZE);

            srand(time(0) + i * j);
            int choosenTile = rand() % 3;
            /* Bounding wall */
            if (i == 0 || j == 0 || tileHeightSize - 1 == j || tileWidthSize - 1 == i) {
                vertex[currentVertex + 0].texCoords = sf::Vector2f(0, TILE_WALL * TILE_SIZE);
                vertex[currentVertex + 1].texCoords = sf::Vector2f(TILE_SIZE, (TILE_WALL * TILE_SIZE));
                vertex[currentVertex + 2].texCoords = sf::Vector2f(TILE_SIZE, (TILE_WALL * TILE_SIZE) + TILE_SIZE);
                vertex[currentVertex + 3].texCoords = sf::Vector2f(0, (TILE_WALL * TILE_SIZE) + TILE_SIZE);
            } else {
                /* Floor */
                vertex[currentVertex + 0].texCoords = sf::Vector2f(0, choosenTile * TILE_SIZE);
                vertex[currentVertex + 1].texCoords = sf::Vector2f(TILE_SIZE, (choosenTile * TILE_SIZE));
                vertex[currentVertex + 2].texCoords = sf::Vector2f(TILE_SIZE, (choosenTile * TILE_SIZE) + TILE_SIZE);
                vertex[currentVertex + 3].texCoords = sf::Vector2f(0, (choosenTile * TILE_SIZE) + TILE_SIZE);
            }

            currentVertex += 4;
        }
    }

    return TILE_SIZE;
}

game::Zombie* game::Utility::createHorde(int numZombies, sf::IntRect arena) {
    game::Zombie* zombies = new game::Zombie[numZombies];

    int maxY = arena.height - 20;
    int minY = arena.top + 20;
    int maxX = arena.width - 20;
    int minX = arena.left + 20;

    for (int i = 0; i < numZombies; i++) {
        /* Which side should the zombie spawn */
        srand((int)time(0) * i);
        int side = (rand() % 4);
        float x, y;

        switch (side) {
            case 0:
                /* left */
                x = minX;
                y = (rand() % maxY) + minY;
                break;

            case 1:
                /* right */
                x = maxX;
                y = (rand() % maxY) + minY;
                break;

            case 2:
                /* top */
                x = (rand() % maxX) + minX;
                y = minY;
                break;

            case 3:
                /* bottom */
                x = (rand() % maxX) + minX;
                y = maxY;
                break;
        }

        /* Bloater, crawler or runner */
        srand((int)time(0) * i * 2);
        int type = (rand() % 3);

        /* Spawn the new zombie into the array */
        zombies[i].spawn(x, y, type, i);
    }

    return zombies;
}