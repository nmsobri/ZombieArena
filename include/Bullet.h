#pragma once

#include <SFML/Graphics.hpp>

namespace game {
    class Bullet {
        sf::Vector2f position;
        sf::RectangleShape bulletShape;
        bool isInFlight = false;
        float bulletSpeed = 1000.0f;

        /* What fraction of 1 pixel does the bullet travel */
        /* Horizontally and vertically each frame */
        /* These values will be derived from bulletSpeed */
        float bulletDistanceX;
        float bulletDistanceY;

        /* Where is this bullet headed to */
        float targetX;
        float targetY;

        /* Some boundaries so the bullet doesn't fly forever */
        float maxX;
        float minX;
        float maxY;
        float minY;

      public:
        Bullet();

        virtual ~Bullet();

        void stop();

        bool isBulletInFlight();

        /* Launch a new bullet */
        void shoot(float startX, float startY, float targetX, float targetY);

        /* Tell the calling code where the bullet is in the world */
        sf::FloatRect getPosition();

        /* Return the actual shape (for drawing) */
        sf::RectangleShape getShape();

        /* Update the bullet each frame */
        void update(float deltaTime);
    };
}
