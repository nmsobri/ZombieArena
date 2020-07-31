#include "include/Pickup.h"
#include "include/Texture.h"

game::Pickup::Pickup(int type) {
    this->type = type;

    /* Associate the texture with the sprite */
    if (this->type == 1) {
        this->sprite = sf::Sprite(game::Texture::getTexture(
            "asset/graphic/health_pickup.png"));

        this->value = HEALTH_START_VALUE;
    } else {
        this->sprite = sf::Sprite(game::Texture::getTexture(
            "asset/graphic/ammo_pickup.png"));

        this->sprite.setScale(0.7, 0.7);

        this->value = AMMO_START_VALUE;
    }

    this->sprite.setOrigin(25, 25);

    this->secondsToLive = START_SECONDS_TO_LIVE;
    this->secondsToWait = START_WAIT_TIME;
}

void game::Pickup::setArena(sf::IntRect arena) {
    this->arena.left = arena.left + 70;
    this->arena.width = arena.width - 70;
    this->arena.top = arena.top + 70;
    this->arena.height = arena.height - 70;

    this->spawn();
}

void game::Pickup::spawn() {
    /* Spawn at a random location */
    srand((int)time(0) / this->type);
    int x = (rand() % this->arena.width);
    srand((int)time(0) * this->type);
    int y = rand() % this->arena.height;

    /* Not currently spawned */
    /*m_Spawned = false; */
    this->secondsSinceSpawn = 0;
    this->spawned = true;
    this->sprite.setPosition(x, y);
}

sf::FloatRect game::Pickup::getPosition() {
    return this->sprite.getGlobalBounds();
}

sf::Sprite game::Pickup::getSprite() {
    return this->sprite;
}

bool game::Pickup::isSpawned() {
    return this->spawned;
}

int game::Pickup::gotIt() {
    this->spawned = false;
    this->secondsSinceDeSpawn = 0;
    return this->value;
}

void game::Pickup::update(float et) {
    if (this->spawned) {
        this->secondsSinceSpawn += et;
    } else {
        this->secondsSinceDeSpawn += et;
    }

    /* Do we need to hide a pickup? */
    if (this->secondsSinceSpawn > this->secondsToLive && this->spawned) {
        this->spawned = false;
        this->secondsSinceDeSpawn = 0;
    }

    /* Do we need to spawn a pickup */
    if (this->secondsSinceDeSpawn > this->secondsToWait && !this->spawned) {
        /* spawn the pickup and reset the timer */
        this->spawn();
    }
}

void game::Pickup::upgrade() {
    if (this->type == 1) {
        this->value += (HEALTH_START_VALUE * .5);
    } else {
        this->value += (AMMO_START_VALUE * .5);
    }

    /* Make them more frequent and last longer */
    this->secondsToLive += (START_SECONDS_TO_LIVE / 10);
    this->secondsToWait -= (START_WAIT_TIME / 10);
}