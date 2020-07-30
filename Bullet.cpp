#include "include/Bullet.h"

game::Bullet::Bullet() {
    bulletShape.setSize(sf::Vector2f(2, 2));
}

game::Bullet::~Bullet() {
}

void game::Bullet::stop() {
    isInFlight = false;
}

bool game::Bullet::isBulletInFlight() {
    return isInFlight;
}

/* Launch a new bullet */
void game::Bullet::shoot(float startX, float startY, float targetX, float targetY) {
    /* Keep track of the bullet*/
    isInFlight = true;
    position.x = startX;
    position.y = startY;

    /* Calculate the gradient of the flight path */
    float gradient = (startX - targetX) / (startY - targetY);

    /* Any gradient less than zero needs to be negative */
    if (gradient < 0) {
        gradient *= -1;
    }

    /* Calculate the ratio between x and t */
    float ratioXY = bulletSpeed / (gradient + 1);

    /* Set the "speed" horizontally and vertically */
    bulletDistanceY = ratioXY;
    bulletDistanceX = ratioXY * gradient;

    /* Point the bullet in the right direction */
    if (targetX < startX) {
        bulletDistanceX *= -1;
    }

    if (targetY < startY) {
        bulletDistanceY *= -1;
    }

    /* Set a max range of 1000 pixels */
    float range = 1000;
    minX = startX - range;
    maxX = startX + range;
    minY = startY - range;
    maxY = startY + range;

    /* Position the bullet ready to be drawn */
    bulletShape.setPosition(position);
}

/* Tell the calling code where the bullet is in the world */
sf::FloatRect game::Bullet::getPosition() {
    return bulletShape.getGlobalBounds();
}

/* Return the actual shape (for drawing) */
sf::RectangleShape game::Bullet::getShape() {
    return bulletShape;
}

/* Update the bullet each frame */
void game::Bullet::update(float deltaTime) {
    /* Update the bullet position variables */
    position.x += bulletDistanceX * deltaTime;
    position.y += bulletDistanceY * deltaTime;

    /* Move the bullet */
    bulletShape.setPosition(position);

    if (position.x < minX || position.x > maxX ||
        position.y < minY || position.y > maxY) {
        isInFlight = false;
    }
}