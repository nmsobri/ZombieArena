#include <array>
#include <SFML/Graphics.hpp>
#include "include/Bullet.h"
#include "include/Player.h"
#include "include/Utility.h"
#include "include/Zombie.h"

int main() {
    /* The game will always be in one of four states */
    enum class State {
        PAUSED,
        LEVELING_UP,
        GAME_OVER,
        PLAYING
    };

    float fireRate = 3;
    sf::Time lastPressed;
    int currentBullet = 0;
    std::array<game::Bullet, 100> bullets;
    std::array<game::Zombie, 50> zombies;

    /* Start with the GAME_OVER state */
    State state = State::GAME_OVER;

    /* Get the screen resolution and create an SFML window */
    sf::Vector2f resolution;
    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;

    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Arena", sf::Style::Fullscreen);

    /* Create a an SFML View for the main action */
    sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    /* Here is our clock for timing everything */
    sf::Clock clock;

    /* How long has the PLAYING state been active */
    sf::Time gameTimeTotal;

    /* Where is the mouse in relation to world coordinates */
    sf::Vector2f mouseWorldPosition;

    /* Where is the mouse in relation to screen coordinates */
    sf::Vector2i mouseScreenPosition;

    /* Create an instance of the Player class */
    Player player;

    /* The boundaries of the arena */
    sf::IntRect arena;

    /* Create the background */
    sf::VertexArray background;

    /* Load the texture for our background vertex array */
    sf::Texture textureBackground;
    textureBackground.loadFromFile("asset/graphic/background_sheet.png");

    /* The main game loop */
    while (window.isOpen()) {
        /*
    *********************************
    * Handle input
    * *******************************
    */
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }

                    if (event.key.code == sf::Keyboard::Return) {
                        if (state == State::PLAYING) {
                            state = State::PAUSED;
                        } else if (state == State::PAUSED) {
                            state = State::PLAYING;
                            clock.restart();
                        } else if (state == State::GAME_OVER) {
                            state = State::LEVELING_UP;
                        }
                    }

                    /* Handle the levelling up state */
                    if (state == State::LEVELING_UP) {
                        /* Handle the player levelling up */
                        if (event.key.code == sf::Keyboard::Num1 ||
                            event.key.code == sf::Keyboard::Num2 ||
                            event.key.code == sf::Keyboard::Num3 ||
                            event.key.code == sf::Keyboard::Num4 ||
                            event.key.code == sf::Keyboard::Num5 ||
                            event.key.code == sf::Keyboard::Num6) {
                            /* Prepare the level */
                            arena.width = 1000;
                            arena.height = 1000;
                            arena.left = 0;
                            arena.top = 0;

                            /* Pass the vertex array by reference to the createBackground function */
                            int tileSize = game::Utility::createBackground(background, arena);
                            game::Utility::createHorde<game::Zombie, zombies.size()>(zombies, arena);

                            /* Spawn the player in the middle of the arena */
                            player.spawn(arena, resolution, tileSize);

                            /* Reset the clock so there isn't a frame jump */
                            clock.restart();

                            state = State::PLAYING;
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        if (state == State::PLAYING) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                player.moveUp();
            } else {
                player.stopUp();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                player.moveDown();
            } else {
                player.stopDown();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                player.moveLeft();
            } else {
                player.stopLeft();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player.moveRight();
            } else {
                player.stopRight();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate) {
                    bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);

                    currentBullet++;
                    currentBullet = currentBullet % bullets.size();    // set current bullet to 0 if its current value eq to bullet size
                    lastPressed = gameTimeTotal;
                }
            }
        }

        /*
        *********************************
        * Update the frame
        *********************************
        */
        if (state == State::PLAYING) {
            /* Update the delta time */
            sf::Time dt = clock.restart();

            /* Update the total game time */
            gameTimeTotal += dt;

            /* Make a decimal fraction of 1 from the delta time */
            float dtAsSeconds = dt.asSeconds();

            /* Where is the mouse pointer */
            mouseScreenPosition = sf::Mouse::getPosition();

            /* Convert mouse position to world coordinates of mainView */
            mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);

            /* Update the player (position & angle) */
            player.update(dtAsSeconds, sf::Mouse::getPosition());

            /* Make a note of the players new position */
            sf::Vector2f playerPosition(player.getCenter());

            /* Make the view centre around the player */
            mainView.setCenter(player.getCenter());

            for (auto& zombie : zombies) {
                if (zombie.isAlive()) {
                    zombie.update(dt.asSeconds(), playerPosition);
                }
            }

            for (auto& bullet : bullets) {
                if (bullet.isBulletInFlight()) {
                    bullet.update(dt.asSeconds());
                }
            }
        }

        /*
        *********************************
        * Draw the scene
        * *******************************
        */
        window.clear();

        if (state == State::PLAYING) {
            /* Set the mainView to be displayed in the window */
            /* And draw everything related to it */
            window.setView(mainView);

            /* Draw the background */
            window.draw(background, &textureBackground);

            /* Draw the zombies */
            for (game::Zombie& zombie : zombies) {
                window.draw(zombie.getSprite());
            }

            /* Draw the bullets*/
            for (game::Bullet& bullet : bullets) {
                window.draw(bullet.getShape());
            }

            /* Draw the player */
            window.draw(player.getSprite());
        }

        if (state == State::LEVELING_UP) {
        }

        if (state == State::PAUSED) {
        }

        if (state == State::GAME_OVER) {
        }

        window.display();
    }
}