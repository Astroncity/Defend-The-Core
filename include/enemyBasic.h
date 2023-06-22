#ifndef ENEMYBASIC_H
#define ENEMYBASIC_H

#include "raylib.h"
#include "gui.h"
#define MAX_ENEMIES 1000
typedef struct Enemy{
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
}Enemy;


extern Enemy ENEMIES[MAX_ENEMIES];
extern int enemyCount;
extern double spawnTimer;


void spawnEnemies();
void handleEnemies();
void destroyEnemy(int enemyIndex);

#endif /*ENEMYBASIC_H*/