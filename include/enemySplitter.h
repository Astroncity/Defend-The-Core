#ifndef ENEMYSPLITTER_H
#define ENEMYSPLITTER_H

#include "raylib.h"
#include "gui.h"
#define MAX_SPLITTER_ENEMIES 1000
#define SPLITTER_SPLIT_FACTOR 0.75
#define SPLITTER_MAX_SPLITS 3
#define SPLITTER_SPAWN_RATE 2

typedef struct SplitterEnemy{
    Vector2 pos;
    double health;
    double maxHealth;
    double speed;
    double damage;
    bool alive;
    int index;
    int coinValue;
    HealthBar healthBar;
    double direction;
    int splits;
    int radius;
}SplitterEnemy;


extern SplitterEnemy splitterEnemies[MAX_SPLITTER_ENEMIES];
extern int enemyCountSplitter;
extern double spawnTimerSplitter;
extern Texture2D splitterEnemyTexture;


void spawnSplitterEnemies();
void handleSplitterEnemies();
void destroySplitterEnemy(int enemyIndex);

#endif /*ENEMYSPLITTER_H*/