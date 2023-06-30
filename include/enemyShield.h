#ifndef ENEMY_SHIELD_H 
#define ENEMY_SHIELD_H

#include "raylib.h"
#include "gui.h"
#define MAX_SHIELD_ENEMIES 1000
#define SHIELD_SPAWN_RATE 2
typedef struct ShieldEnemy{
    Vector2 pos;
    double health;
    double maxHealth;
    double speed;
    double damage;
    bool alive;
    int index;
    int coinValue;
    HealthBar healthBar;
    int radius;
    double direction;
    double shieldHealth;
    double shieldMaxHealth;
    bool shieldActive;
}ShieldEnemy;


extern ShieldEnemy shieldEnemies[MAX_SHIELD_ENEMIES];
extern int enemyCountShield;
extern double spawnTimerShield;
extern Texture2D shieldEnemyTexture;
extern Texture2D shieldEnemyShieldTexture;

void spawnShieldEnemies();
void handleShieldEnemies();
void destroyShieldEnemy(int enemyIndex);

#endif