#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "utils.h"
#include "gui.h"
#include "inttypes.h"

typedef struct Player{
    double health;
    double maxHealth;
    double speed;
    double damage;
    double attackSpeed;
    double direction;
    uint64_t coins;
    Trig trig;
    HealthBar healthBar;
    Vector2 center;
    Rectangle hitbox;
}Player;


extern Player player;
extern bool shotgunPurchased;

void handlePlayer();

#endif /*PLAYER_H*/