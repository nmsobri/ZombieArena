#include "include/Player.h"
#include <cmath>

Player::Player(const char* filePath) {
    speed = START_SPEED;
    health = START_HEALTH;
    maxHealth = START_HEALTH;

    /* Associate a texture with the sprite */
    texture.loadFromFile(filePath);
    sprite.setTexture(texture);

    sprite.setOrigin(25, 25);
}

Player::Player() {
    speed = START_SPEED;
    health = START_HEALTH;
    maxHealth = START_HEALTH;

    /* Associate a texture with the sprite */
    texture.loadFromFile("asset/graphic/player.png");
    sprite.setTexture(texture);

    sprite.setOrigin(25, 25);
}

void Player::spawn(sf::IntRect _arena, sf::Vector2f _resolution, int _tileSize) {
    /* Place the player in the middle of the arena*/
    position.x = _arena.width / 2;
    position.y = _arena.height / 2;

    /* Copy the details of the arena to the players arena*/
    arena.left = _arena.left;
    arena.width = _arena.width;
    arena.top = _arena.top;
    arena.height = _arena.height;

    /* Remember how big the tiles are ins this arena */
    tileSize = _tileSize;

    /* Store the resolution for future use */
    resolution.x = _resolution.x;
    resolution.y = _resolution.y;
}

sf::Time Player::getLastHitTime() {
    return lastHit;
}

bool Player::hit(sf::Time timeHit) {
    /* 2 tenths of second */
    if (timeHit.asMilliseconds() - lastHit.asMilliseconds() > 200) {
        lastHit = timeHit;
        health -= 10;
        return true;
    }

    return false;
}

sf::FloatRect Player::getPosition() {
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getCenter() {
    return position;
}

float Player::getRotation() {
    return sprite.getRotation();
}

sf::Sprite Player::getSprite() {
    return sprite;
}

int Player::getHealth() {
    return health;
}

void Player::moveLeft() {
    leftPressed = true;
}

void Player::moveRight() {
    rightPressed = true;
}

void Player::moveUp() {
    upPressed = true;
}

void Player::moveDown() {
    downPressed = true;
}

void Player::stopLeft() {
    leftPressed = false;
}

void Player::stopRight() {
    rightPressed = false;
}

void Player::stopUp() {
    upPressed = false;
}

void Player::stopDown() {
    downPressed = false;
}

void Player::update(float elapsedTime, sf::Vector2i mousePosition) {
    if (upPressed) {
        position.y -= speed * elapsedTime;
    }

    if (downPressed) {
        position.y += speed * elapsedTime;
    }

    if (rightPressed) {
        position.x += speed * elapsedTime;
    }

    if (leftPressed) {
        position.x -= speed * elapsedTime;
    }

    sprite.setPosition(position);

    /* keep the player in the arena */
    /* Put player to the right most tile*/
    if (position.x > arena.width - tileSize) {
        position.x = arena.width - tileSize;
    }

    /* Put player to the left most tile*/
    if (position.x < arena.left + tileSize) {
        position.x = arena.left + tileSize;
    }

    /* If player bottom position is way too bottom, put player at the most bottom of the screen*/
    if (position.y > arena.height - tileSize) {
        position.y = arena.height - tileSize;
    }

    /* If player top position is way too top, put player at the most top of the screen*/
    if (position.y < arena.top + tileSize) {
        position.y = arena.top + tileSize;
    }

    /* Calculate the angle the player is facing */
    float angle = (std::atan2(mousePosition.y - resolution.y / 2, mousePosition.x - resolution.x / 2) * 180) / 3.141;
    sprite.setRotation(angle);
}

void Player::upgradeHealth() {
    /* 20% max health upgrade */
    maxHealth += START_HEALTH * 0.2;
}

void Player::upgradeSpeed() {
    speed += (START_SPEED * .2);
}

void Player::increaseHealthLevel(int amount) {
    health += amount;

    /*If health > maxHealth, cap it to maxHealth */
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void Player::resetPlayerStats() {
    speed = START_SPEED;
    health = START_HEALTH;
    maxHealth = START_HEALTH;
}