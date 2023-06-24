#ifndef ENEMYBASIC_H
#define ENEMYBASIC_H

#include "raylib.h"
#include "gui.h"
#define MAX_BASIC_ENEMIES 1000
#define BASIC_SPAWN_RATE 0.5
typedef struct BasicEnemy{
    Vector2 pos;
    double health;
    double maxHealth;
    double speed;
    double damage;
    Color color;
    bool alive;
    int index;
    int coinValue;
    HealthBar healthBar;
    int radius;
    double direction;
}BasicEnemy;


extern BasicEnemy BasicEnemies[MAX_BASIC_ENEMIES];
extern int enemyCountBasic;
extern double spawnTimerBasic;


void spawnBasicEnemies();
void handleBasicEnemies();
void destroyBasicEnemy(int enemyIndex);

#endif /*ENEMYBASIC_H*/