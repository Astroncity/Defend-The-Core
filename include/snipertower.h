#ifndef SNIPERTOWER_H
#define SNIPERTOWER_H

#include "raylib.h"
#define MAX_SNIPER_TOWERS 520

typedef struct SniperTower{
    double damage;
    double range;
    double direction;
    double fireRate;
    Rectangle rect;
    bool active;
    int index;
    int level;
    double lastAngle;
    double timer;
    double shortestDist;
}SniperTower;

typedef struct GeneralEnemy{
    Vector2 pos;
    double direction;
    double speed;
}GeneralEnemy;

extern double defaultSniperTowerRange;
extern int sniperTowerCount;
extern SniperTower sniperTowers[MAX_SNIPER_TOWERS];
extern Texture2D sniperTowerBaseTexture;
extern Texture2D sniperTowerTurretTexture;
extern Sound sniperSnd;

void spawnSniperTower();
void handleSniperTowers();
void handleSniperTowerGUI();
#endif /*SNIPERTOWER_H*/