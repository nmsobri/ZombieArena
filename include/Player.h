#pragma once

#include <SFML/Graphics.hpp>

class Player {
    const float START_SPEED = 200;
    const float START_HEALTH = 100;

    /* Where is the player */
    sf::Vector2f position;

    /* Ofcourse we will need a sprite */
    sf::Sprite sprite;

    /* Add a texture */
    sf::Texture texture;

    /* What is the screen resolution */
    sf::Vector2f resolution;

    /* What size is the current arena */
    sf::IntRect arena;

    /* How big is each tile of the arena */
    int tileSize;

    /* Which directions is the player currently moving in */
    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;

    /* How much health has the player have */
    int health;

    /* What is the maximum health the player can have */
    int maxHealth;

    /* When was the player last hit*/
    sf::Time lastHit;

    /* Speed in pixels per seconds */
    float speed;

  public:
    Player();
    Player(const char*);

    void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);

    /* Handle the player getting hit by a zombie */
    bool hit(sf::Time timeHit);

    /* How long ago was the palyer last hit */
    sf::Time getLastHitTime();

    /* Where is the player */
    sf::FloatRect getPosition();

    /* Where is the center of the player */
    sf::Vector2f getCenter();

    /* Which angle is the player facing */
    float getRotation();

    /* Return a copy of the sprite to the caller */
    sf::Sprite getSprite();

    /* How much health has the player currently got */
    int getHealth();

    /* The next four functions move the player */
    void moveLeft();

    void moveRight();

    void moveUp();

    void moveDown();

    /* Stop the player moving in a specific direction */
    void stopLeft();

    void stopRight();

    void stopUp();

    void stopDown();

    /* We will call this function once every frame */
    void update(float elapsedTime, sf::Vector2i mouseposition);

    /* Give player a speed boost */
    void upgradeSpeed();

    /* Give the player some health */
    void upgradeHealth();

    /* Increase the maximum amount of health the player can have*/
    void increaseHealthLevel(int amount);
};