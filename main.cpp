#include <iostream>
#include <SFML/Graphics.hpp>
#include "include/Player.h"

int main() {
    /* The game will always be in one of four states */
    enum class State {
        PAUSED,
        LEVELING_UP,
        GAME_OVER,
        PLAYING
    };

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

    /* The main game loop */
    while (window.isOpen()) {
        /*
    *********************************
    * Handle input
    * *******************************
    */
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
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

                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }

                if (state == State::PLAYING) {
                    if (event.key.code == sf::Keyboard::W) {
                        player.moveUp();
                    } else {
                        player.stopUp();
                    }

                    if (event.key.code == sf::Keyboard::S) {
                        player.moveDown();
                    } else {
                        player.stopDown();
                    }

                    if (event.key.code == sf::Keyboard::A) {
                        player.moveLeft();
                    } else {
                        player.stopLeft();
                    }

                    if (event.key.code == sf::Keyboard::D) {
                        player.moveRight();
                    } else {
                        player.stopRight();
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
                        state = State::PLAYING;
                    }

                    if (state == State::PLAYING) {
                        /* Prepare the level */
                        /* We will modify the next two lines later */
                        arena.width = 500;
                        arena.height = 500;
                        arena.left = 0;
                        arena.top = 0;

                        /* We will modify this line of code later */
                        int tileSize = 50;

                        /* Spawn the player in the middle of the arena */
                        player.spawn(arena, resolution, tileSize);

                        /* Reset the clock so there isn't a frame jump */
                        clock.restart();
                    }
                }
            }
        }

        /*
        *********************************
        * Update the frame
        * *******************************
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
        }

        /*
        *********************************
        * Draw the scene
        * *******************************
        */
        if (state == State::PLAYING) {
            window.clear();

            /* Set the mainView to be displayed in the window */
            /* And draw everything related to it */
            window.setView(mainView);

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