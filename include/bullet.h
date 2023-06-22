#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#define MAX_BULLETS 50000
#define PELLET_COUNT 5
#define SHOTGUN_PELLET_SPREAD 4
typedef struct Bullet{
    double speed;
    double direction;
    Vector2 pos;
    bool active;
    int index;
    int size;
    int damage;
    Color color;    
}Bullet;


extern int bulletCount;
extern Sound shootSnd;
extern Sound shotgunSnd;
extern Texture2D bulletTexture;
extern Bullet bullets[MAX_BULLETS];
extern Rectangle bulletTextureSource;

void shootBasic(Vector2 center, double angleBetweenMouse);
void shootShotgun(Vector2 center, double angleBetweenMouse);
void destroyBullet(int bulletIndex);
void handleBullets();

#endif /*BULLET_H*/