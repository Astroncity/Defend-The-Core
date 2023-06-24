#include "bullet.h"
#include "player.h"
#include "enemyBasic.h"
#include "enemySplitter.h"
#include <stdio.h> 



int bulletCount = 0;
Sound shootSnd;
Sound shotgunSnd;
Texture2D bulletTexture;
Bullet bullets[MAX_BULLETS];
Rectangle bulletTextureSource = {0, 0, 16, 16};

static void checkCollisionBasic(Bullet bullet);
static void checkCollisionSplitter(Bullet bullet);

void shootBasic(Vector2 center, double angleBetweenMouse){
    bool canShoot = true;

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && canShoot){
        canShoot = false;
        Bullet bullet;
        bullet.pos.x = center.x;
        bullet.pos.y = center.y;
        bullet.direction = angleBetweenMouse;
        bullet.speed = 750;
        bullet.size = 4;
        bullet.active = true;
        bullet.color = GREEN;
        bullet.damage = player.damage;
        bullet.index = bulletCount;
        bullets[bulletCount] = bullet;
        bulletCount++;
        PlaySound(shootSnd);
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        canShoot = true;
    }
    //print gamepad state
    //print("Gamepad: %d", IsGamepadButtonReleased);
}


void shootShotgun(Vector2 center, double angleBetweenMouse){
    bool canShoot = true;

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && canShoot){
        canShoot = false;
        for(int i = 0; i < PELLET_COUNT; i++){
            Bullet bullet;
            bullet.pos.x = center.x;
            bullet.pos.y = center.y;
            bullet.direction = angleBetweenMouse - degToRad(PELLET_COUNT + i * SHOTGUN_PELLET_SPREAD)
                                + degToRad(PELLET_COUNT * SHOTGUN_PELLET_SPREAD / 2) + degToRad(PELLET_COUNT*0.6) + degToRad(GetRandomValue(-1, 1));
            bullet.speed = 750;
            bullet.active = true;
            bullet.index = bulletCount;
            bullet.color = GREEN;
            bullet.damage = player.damage;
            bullet.size = 4;
            bullets[bulletCount] = bullet;
            bulletCount++;
            PlaySound(shotgunSnd);
        }
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        canShoot = true;
    }
}


void destroyBullet(int bulletIndex){
    int lastUsedSlot = bulletCount - 1;
    if(lastUsedSlot == -1){
        printf("Add more memory dumbass, bullet array full");
        return;
    }
    Bullet bulletCP = bullets[lastUsedSlot];
    bulletCP.index = bulletIndex;
    bulletCP.active = true;
    bullets[bulletIndex] = bulletCP;
    bullets[lastUsedSlot] = (Bullet){0, 0, (Vector2){0, 0}, false};
    bulletCount--;
}


void handleBullets(){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(bullets[i].active == false){
            break;
        }

        bullets[i].pos.x += cos(bullets[i].direction) * bullets[i].speed * deltaTime;
        bullets[i].pos.y += sin(bullets[i].direction) * bullets[i].speed * deltaTime;

        bullets[i].speed += 10.5;


        //DrawCircle(bullets[i].pos.x, bullets[i].pos.y, bullets[i].size, bullets[i].color);
        DrawTextureEx(bulletTexture, bullets[i].pos, 0, 1, WHITE);
        // destory bullet if outside screen
        if((bullets[i].pos.x > screenWidth || bullets[i].pos.x < 0) || (bullets[i].pos.y < 0 || bullets[i].pos.y > screenHeight)){
            destroyBullet(i);
        }

        checkCollisionBasic(bullets[i]);
        checkCollisionSplitter(bullets[i]);
        
    }
}



static void checkCollisionBasic(Bullet bullet){
    for(int j = 0; j < enemyCountBasic; j++){
        if(CheckCollisionCircles(BasicEnemies[j].pos, 10, bullet.pos, bullet.size)){
            BasicEnemies[j].health -= bullet.damage;
            destroyBullet(bullet.index);
        }
    }
}


static void checkCollisionSplitter(Bullet bullet){
    for(int j = 0; j < enemyCountSplitter; j++){
        if(CheckCollisionCircles(SplitterEnemies[j].pos, 10, bullet.pos, bullet.size)){
            SplitterEnemies[j].health -= bullet.damage;
            destroyBullet(bullet.index);
        }
    }
}