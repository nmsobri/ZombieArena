#include "include/Zombie.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "include/Texture.h"

void game::Zombie::spawn(float startX, float startY, int type, int seed) {
    switch (type) {
        case 0:
            /* Bloater */
            sprite = sf::Sprite(game::Texture::getTexture("asset/graphic/bloater.png"));
            speed = 40;
            health = 7;
            break;

        case 1:
            /* Chaser */
            sprite = sf::Sprite(game::Texture::getTexture("asset/graphic/chaser.png"));
            speed = 70;
            health = 3;
            break;

        case 2:
            /* Crawler */
            sprite = sf::Sprite(game::Texture::getTexture("asset/graphic/crawler.png"));
            speed = 20;
            health = 5;
            break;
    }

    /* Modify the speed to make the zombie unique */
    /* Every zombie is unique. Create a speed modifier */
    srand((int)time(0) * seed);

    /* Somewhere between 80 an 100 */
    float modifier = (rand() % MAX_VARRIANCE) + OFFSET;

    /* Express as a fraction of 1 */
    modifier /= 100;    // Now equals between .7 and 1
    speed *= modifier;

    position.x = startX;
    position.y = startY;

    sprite.setOrigin(25, 25);
    sprite.setPosition(position);
}

bool game::Zombie::hit() {
    health--;

    if (health <= 0) {
        /* Dead */
        alive = false;
        sprite.setTexture(game::Texture::getTexture("asset/graphic/blood.png"));

        return true;
    }

    /* Injured but not dead yet */
    return false;
}

bool game::Zombie::isAlive() {
    return alive;
}

sf::FloatRect game::Zombie::getPosition() {
    return sprite.getGlobalBounds();
}

sf::Sprite game::Zombie::getSprite() {
    return sprite;
}

void game::Zombie::update(float elapsedTime, sf::Vector2f playerLocation) {
    float playerX = playerLocation.x;
    float playerY = playerLocation.y;

    /* Update the zombie position variables */
    if (playerX > position.x) {
        position.x = position.x + speed * elapsedTime;
    }

    if (playerY > position.y) {
        position.y = position.y + speed * elapsedTime;
    }

    if (playerX < position.x) {
        position.x = position.x - speed * elapsedTime;
    }

    if (playerY < position.y) {
        position.y = position.y - speed * elapsedTime;
    }

    /* Move the sprite */
    sprite.setPosition(position);

    /* Face the sprite in the correct direction */
    float angle = (atan2(playerY - position.y, playerX - position.x) * 180) / 3.141;
    sprite.setRotation(angle);
}