#ifndef SNIPERTOWER_H
#define SNIPERTOWER_H

#include "raylib.h"


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


extern double defaultSniperTowerRange;
extern int sniperTowerCount;
extern SniperTower sniperTowers[520];
extern Texture2D sniperTowerBaseTexture;
extern Texture2D sniperTowerTurretTexture;
extern Sound sniperSnd;

void spawnSniperTower();
void handleSniperTowers();
void handleSniperTowerGUI();
#endif /*SNIPERTOWER_H*/