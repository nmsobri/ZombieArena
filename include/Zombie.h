#pragma once
#include <SFML/Graphics.hpp>

namespace game {
    class Zombie {
      private:
        /* How fast is each zombie type */
        const float BLOATER_SPEED = 40;
        const float CHASER_SPEED = 80;
        const float CRAWLER_SPEED = 20;

        /* How tough is each zombie type */
        const float BLOATER_HEALTH = 5;
        const float CHASER_HEALTH = 1;
        const float CRAWLER_HEALTH = 3;

        /* Make each zombie vary its speed slightly */
        const int MAX_VARRIANCE = 30;
        const int OFFSET = 101 - MAX_VARRIANCE;

        /* Where is this zombie */
        sf::Vector2f position;

        /* A sprite for the zombie */
        sf::Sprite sprite;

        /* How fast can this one run/crawl */
        float speed;

        /* How much health has it got */
        float health;

        /* Is it still alive */
        bool alive = true;

      public:
        /* Handle when a bullet hits a zombie */
        bool hit();

        /* Find out if the zombie is alive */
        bool isAlive();

        /* Spawn a new zombie */
        void spawn(float startX, float startY, int type, int seed);

        /* Return a rectangle that is the position in the world */
        sf::FloatRect getPosition();

        /* Get a copy of the sprite to draw */
        sf::Sprite getSprite();

        /* Update the zombie each frame */
        void update(float elapsedTime, sf::Vector2f playerLocation);
    };
}