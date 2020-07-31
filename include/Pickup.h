#pragma once

#include <SFML/Graphics.hpp>

namespace game {
    class Pickup {
        /* Start value for the health pickups*/
        const int HEALTH_START_VALUE = 50;
        const int AMMO_START_VALUE = 12;
        const int START_WAIT_TIME = 10;
        const int START_SECONDS_TO_LIVE = 5;

        /* The sprite that represents this picku */
        sf::Sprite sprite;

        /* The arena it exists in */
        sf::IntRect arena;

        /* How much is this pickup worth? */
        int value;

        /* What type of pickup is this? */
        /* 1 = health, 2 = ammo */
        int type;

        /* Handle spawning and disappearing */
        bool spawned = false;
        float secondsSinceSpawn;
        float secondsSinceDeSpawn;
        float secondsToLive;
        float secondsToWait;

      public:
        Pickup(int type);

        /* Prepare a new pickup */
        void setArena(sf::IntRect arena);

        void spawn();

        /* Check the position of a pickup */
        sf::FloatRect getPosition();

        /* Get the sprite for drawing */
        sf::Sprite getSprite();

        /* Let the pickup update itself each frame */
        void update(float et);

        /* Is this pickup currently spawned? */
        bool isSpawned();

        /* Get the goodness from the pickup */
        int gotIt();

        /* Upgrade the value of each pickup */
        void upgrade();
    };
}