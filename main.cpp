#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "include/Bullet.h"
#include "include/Pickup.h"
#include "include/Player.h"
#include "include/Texture.h"
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

    int clipSize = 15;
    int bulletInClip = 15;
    int remainingBullet = 100;
    float healthPlayer = 100.f;
    int score = 0;
    int hiScore = 0;
    int waveCount = 0;
    float fireRate = 1;
    int currentBullet = 0;
    sf::Time lastPressed;
    sf::Time lastHealthReduction;
    std::array<game::Bullet, 100> bullets;
    std::array<game::Zombie, 10> zombies;
    int zombiesAlive = zombies.size();

    /* Start with the GAME_OVER state */
    State state = State::GAME_OVER;

    /* Get the screen resolution and create an SFML window */
    sf::Vector2f resolution;
    resolution.x = sf::VideoMode::getDesktopMode().width;
    resolution.y = sf::VideoMode::getDesktopMode().height;

    std::ifstream inputFile("data/score.txt");
    if (inputFile.is_open()) {
        inputFile >> hiScore;
        inputFile.close();
    }

    sf::Font font;
    font.loadFromFile("asset/font/zombiecontrol.ttf");

    sf::Text textGameOver;
    textGameOver.setFont(font);
    textGameOver.setCharacterSize(100.f);
    textGameOver.setString("Press Enter To Play");

    std::stringstream ssScore;
    ssScore << "Score: " << score;
    sf::Text textScore;
    textScore.setFont(font);
    textScore.setCharacterSize(60.f);
    textScore.setString(ssScore.str());

    std::stringstream ssHiScore;
    ssHiScore << "High Score: " << hiScore;
    sf::Text textHiScore;
    textHiScore.setFont(font);
    textHiScore.setCharacterSize(60.f);
    textHiScore.setString(ssHiScore.str());

    std::stringstream ssLevelUp;
    ssLevelUp << "1 - Increased rate of fire"
              << "\n2 - Increased clip size(next reload)"
              << "\n3 - Increased max health"
              << "\n4 - Increased run speed"
              << "\n5 - More and better health pickups"
              << "\n6 - More and better ammo pickups";

    sf::Text textLevelUp;
    textLevelUp.setFont(font);
    textLevelUp.setCharacterSize(60.f);
    textLevelUp.setString(ssLevelUp.str());

    sf::Text textContinue;
    textContinue.setFont(font);
    textContinue.setCharacterSize(60.f);
    textContinue.setString("Press Enter To Continue");

    std::stringstream ssWaveCount;
    ssWaveCount << "Wave: " << waveCount;
    sf::Text textWaveCount;
    textWaveCount.setFont(font);
    textWaveCount.setCharacterSize(60.f);
    textWaveCount.setString(ssWaveCount.str());

    std::stringstream ssZombieCount;
    ssZombieCount << "Zombie: " << zombiesAlive;
    sf::Text textZombieCount;
    textZombieCount.setFont(font);
    textZombieCount.setCharacterSize(60.f);
    textZombieCount.setString(ssZombieCount.str());

    std::stringstream ssBulletCount;
    ssBulletCount << bulletInClip << "/" << remainingBullet;
    sf::Text textBulletCount;
    textBulletCount.setFont(font);
    textBulletCount.setCharacterSize(60.f);
    textBulletCount.setString(ssBulletCount.str());

    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Arena", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);

    sf::Sprite spriteGameOver;
    sf::Texture textureGameOver = game::Texture::getTexture("asset/graphic/background.png");
    spriteGameOver.setTexture(textureGameOver);
    spriteGameOver.setPosition(0, 0);

    sf::Sprite spriteCrossHair;
    sf::Texture textureCrosshair = game::Texture::getTexture("asset/graphic/crosshair.png");
    spriteCrossHair.setTexture(textureCrosshair);
    spriteCrossHair.setOrigin(16, 16);

    sf::Sprite spriteAmmoCount;
    sf::Texture textureAmmoCount = game::Texture::getTexture("asset/graphic/ammo_icon.png");
    spriteAmmoCount.setTexture(textureAmmoCount);
    spriteAmmoCount.setPosition(30, resolution.y - 160);

    sf::Sprite spriteHealthIndicator;
    sf::Texture textureHealthIndicator = game::Texture::getTexture("asset/graphic/health_pickup.png");
    spriteHealthIndicator.setTexture(textureHealthIndicator);
    spriteHealthIndicator.setScale(1.3f, 1.3f);
    spriteHealthIndicator.setPosition(20, resolution.y - 100);

    sf::RectangleShape playerHealth;
    playerHealth.setSize(sf::Vector2f(300.f, 45.f));
    playerHealth.setFillColor(sf::Color::Red);
    playerHealth.setPosition(spriteHealthIndicator.getLocalBounds().width + 40, resolution.y - 85);

    /* Positioning the text*/
    textGameOver.setOrigin(textGameOver.getLocalBounds().width / 2, textGameOver.getLocalBounds().height / 2);
    textGameOver.setPosition(resolution.x / 2.f, resolution.y - (textGameOver.getCharacterSize() + 20));
    textHiScore.setPosition(resolution.x - (textHiScore.getLocalBounds().width + 40), 40);
    textScore.setPosition(40, 40);
    textLevelUp.setOrigin(textLevelUp.getLocalBounds().width / 2.f, textLevelUp.getLocalBounds().height / 2.f);
    textLevelUp.setPosition(resolution.x / 2.f, resolution.y / 2.f);
    textContinue.setOrigin(textContinue.getLocalBounds().width / 2, textContinue.getLocalBounds().height / 2);
    textContinue.setPosition(resolution.x / 2.f, resolution.y / 2.f);
    textWaveCount.setPosition(resolution.x - (textWaveCount.getLocalBounds().width + 30), resolution.y - 170);
    textZombieCount.setPosition(resolution.x - (textZombieCount.getLocalBounds().width + 30), resolution.y - 100);
    textBulletCount.setPosition(spriteAmmoCount.getLocalBounds().width + 50, resolution.y - 170);

    sf::SoundBuffer sbHit;
    sbHit.loadFromFile("asset/sound/hit.wav");
    sf::Sound soundHit;
    soundHit.setBuffer(sbHit);

    sf::SoundBuffer sbPickup;
    sbPickup.loadFromFile("asset/sound/pickup.wav");
    sf::Sound soundPickup;
    soundPickup.setBuffer(sbPickup);

    sf::SoundBuffer sbPowerup;
    sbPowerup.loadFromFile("asset/sound/powerup.wav");
    sf::Sound soundPowerup;
    soundPowerup.setBuffer(sbPowerup);

    sf::SoundBuffer sbReload;
    sbReload.loadFromFile("asset/sound/reload.wav");
    sf::Sound soundReload;
    soundReload.setBuffer(sbReload);

    sf::SoundBuffer sbReloadFailed;
    sbReloadFailed.loadFromFile("asset/sound/reload_failed.wav");
    sf::Sound soundReloadFailed;
    soundReloadFailed.setBuffer(sbReloadFailed);

    sf::SoundBuffer sbShoot;
    sbShoot.loadFromFile("asset/sound/shoot.wav");
    sf::Sound soundShoot;
    soundShoot.setBuffer(sbShoot);

    sf::SoundBuffer sbSplat;
    sbSplat.loadFromFile("asset/sound/splat.wav");
    sf::Sound soundSplat;
    soundSplat.setBuffer(sbSplat);

    sf::SoundBuffer sbIntro;
    sbIntro.loadFromFile("asset/sound/intro.wav");
    sf::Sound soundIntro;
    soundIntro.setBuffer(sbIntro);

    /* Create a an SFML View for the main action */
    sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    /* Create a an SFML View for the hud */
    sf::View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

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

    /* Pickups */
    game::Pickup healthPickup(1);
    game::Pickup ammoPickup(2);

    soundIntro.play();

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
                            waveCount = 0;
                            currentBullet = 0;
                            remainingBullet = 100;
                            clipSize = 15;
                            bulletInClip = 15;
                            fireRate = 1;

                            player.resetPlayerStats();

                            state = State::LEVELING_UP;
                        }
                    }

                    /* Handle the levelling up state */
                    if (state == State::LEVELING_UP) {
                        if (event.key.code == sf::Keyboard::Num1) {
                            fireRate++;
                            state = State::PLAYING;
                        }

                        if (event.key.code == sf::Keyboard::Num2) {
                            clipSize += clipSize;
                            state = State::PLAYING;
                        }

                        if (event.key.code == sf::Keyboard::Num3) {
                            player.upgradeHealth();
                            state = State::PLAYING;
                        }

                        if (event.key.code == sf::Keyboard::Num4) {
                            player.upgradeSpeed();
                            state = State::PLAYING;
                        }

                        if (event.key.code == sf::Keyboard::Num5) {
                            healthPickup.upgrade();
                            state = State::PLAYING;
                        }

                        if (event.key.code == sf::Keyboard::Num6) {
                            ammoPickup.upgrade();
                            state = State::PLAYING;
                        }

                        if (state == State::PLAYING) {
                            soundIntro.stop();
                            /* Prepare the level */
                            arena.width = 1000;
                            arena.height = 1000;
                            arena.left = 0;
                            arena.top = 0;
                            waveCount++;

                            /* Pass the vertex array by reference to the createBackground function */
                            int tileSize = game::Utility::createBackground(background, arena);
                            game::Utility::createHorde<game::Zombie, zombies.size()>(zombies, arena);

                            /* Spawn the player in the middle of the arena */
                            player.spawn(arena, resolution, tileSize);

                            ammoPickup.setArena(arena);
                            healthPickup.setArena(arena);

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                int needToTopup = clipSize - bulletInClip;

                if (remainingBullet != 0) {
                    if (remainingBullet > needToTopup) {
                        bulletInClip = clipSize;
                        remainingBullet -= needToTopup;
                    } else {
                        bulletInClip = remainingBullet;
                        remainingBullet = 0;
                    }
                    soundReload.play();
                } else {
                    soundReloadFailed.play();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletInClip > 0) {
                    bullets[currentBullet].shoot(
                        player.getCenter().x, player.getCenter().y,
                        mouseWorldPosition.x, mouseWorldPosition.y);

                    currentBullet++;
                    currentBullet = currentBullet % bullets.size();    // Set current bullet to 0 if its current value eq to bullet size
                    lastPressed = gameTimeTotal;
                    bulletInClip--;
                    soundShoot.play();
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

            /* Convert mouse position to world coordinates of mainView */
            mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);

            /* Make a note of the players new position */
            sf::Vector2f playerPosition(player.getCenter());

            spriteCrossHair.setPosition(mouseWorldPosition.x, mouseWorldPosition.y);

            /* Where is the mouse pointer */
            mouseScreenPosition = sf::Mouse::getPosition();

            /* Update the player (position & angle) */
            player.update(dtAsSeconds, sf::Mouse::getPosition());

            /* Make the view centre around the player */
            mainView.setCenter(player.getCenter());

            for (game::Zombie& zombie : zombies) {
                if (zombie.isAlive()) {
                    zombie.update(dt.asSeconds(), playerPosition);
                }
            }

            /* Check for collision between bullet and zombie */
            for (int i = 0; i < bullets.size(); i++) {
                for (int j = 0; j < zombies.size(); j++) {
                    if (bullets[i].isBulletInFlight() && zombies[j].isAlive()) {
                        if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
                            bullets[i].stop();
                            if (zombies[j].hit()) {
                                zombiesAlive--;
                                score += 10;
                                soundSplat.play();

                                if (score >= hiScore) {
                                    hiScore = score;
                                }

                                if (zombiesAlive == 0) {
                                    state = State::LEVELING_UP;
                                    waveCount++;
                                    zombiesAlive = zombies.size();
                                }
                            }
                        }
                    }
                }
            }

            /* Check for collision between zombie and player */
            for (int i = 0; i < zombies.size(); i++) {
                if (zombies[i].isAlive() && zombies[i].getSprite().getGlobalBounds().intersects(player.getSprite().getGlobalBounds())) {
                    if (gameTimeTotal.asMilliseconds() - lastHealthReduction.asMilliseconds() > 250) {
                        healthPlayer -= 10;
                        soundHit.play();

                        if (healthPlayer <= 0) {
                            waveCount = 0;
                            healthPlayer = 100.f;
                            state = State::GAME_OVER;

                            std::ofstream outputFile("data/score.txt");
                            if (outputFile.is_open()) {
                                outputFile << hiScore;
                                outputFile.close();
                            }
                        }

                        playerHealth.setSize(sf::Vector2f((300.f / 100.f) * healthPlayer, 45.f));
                        lastHealthReduction = gameTimeTotal;
                    }
                }
            }

            if (player.getPosition().intersects(ammoPickup.getPosition())) {
                if (ammoPickup.isSpawned()) {
                    soundReload.play();
                    int value = ammoPickup.gotIt();
                    remainingBullet += value;
                }
            }

            if (player.getPosition().intersects(healthPickup.getPosition())) {
                if (healthPickup.isSpawned()) {
                    soundPickup.play();
                    int value = healthPickup.gotIt();
                    healthPlayer += value;

                    if (healthPlayer > 100.f) {
                        healthPlayer = 100.f;
                    }

                    std::cout << "Player health: " << healthPlayer << std::endl;
                }
            }

            for (game::Bullet& bullet : bullets) {
                if (bullet.isBulletInFlight()) {
                    bullet.update(dt.asSeconds());
                } else {
                    bullet.destroy();
                }
            }

            ammoPickup.update(dtAsSeconds);
            healthPickup.update(dtAsSeconds);

            ssWaveCount.str("");
            ssWaveCount << "Wave: " << waveCount;
            textWaveCount.setString(ssWaveCount.str());

            ssZombieCount.str("");
            ssZombieCount << "Zombie: " << zombiesAlive;
            textZombieCount.setString(ssZombieCount.str());

            ssScore.str("");
            ssScore << "Score: " << score;
            textScore.setString(ssScore.str());

            ssBulletCount.str("");
            ssBulletCount << bulletInClip << "/" << remainingBullet;
            textBulletCount.setString(ssBulletCount.str());

            playerHealth.setSize(sf::Vector2f((300.f / 100.f) * healthPlayer, 45.f));

            ssHiScore.str("");
            ssHiScore << "High Score: " << hiScore;
            textHiScore.setString(ssHiScore.str());
        }

        /*
        *********************************
        * Draw the scene
        * *******************************
        */
        window.clear();

        if (state == State::PLAYING || state == State::PAUSED) {
            /* Set the mainView to be displayed in the window */
            /* And draw everything related to it */
            window.setView(mainView);

            /* Draw the background */
            window.draw(background, &textureBackground);

            /* Draw the bullets*/
            for (game::Bullet& bullet : bullets) {
                window.draw(bullet.getShape());
            }

            /* Draw the pickup*/
            if (ammoPickup.isSpawned()) {
                window.draw(ammoPickup.getSprite());
            }

            if (healthPickup.isSpawned()) {
                window.draw(healthPickup.getSprite());
            }

            /* Draw the cursor */
            window.draw(spriteCrossHair);

            /* Draw the zombies */
            for (game::Zombie& zombie : zombies) {
                window.draw(zombie.getSprite());
            }

            /* Draw the player */
            window.draw(player.getSprite());

            window.setView(hudView);
            window.draw(textScore);
            window.draw(textHiScore);
            window.draw(textWaveCount);
            window.draw(textZombieCount);
            window.draw(spriteAmmoCount);
            window.draw(textBulletCount);
            window.draw(playerHealth);
            window.draw(spriteHealthIndicator);
        }

        if (state == State::PAUSED) {
            window.setView(hudView);
            window.draw(textContinue);
        }

        if (state == State::LEVELING_UP) {
            window.setView(hudView);
            window.draw(spriteGameOver);
            window.draw(textLevelUp);
        }

        if (state == State::GAME_OVER) {
            window.setView(hudView);
            window.draw(spriteGameOver);
            window.draw(textGameOver);
            window.draw(textHiScore);
        }

        window.display();
    }
}
