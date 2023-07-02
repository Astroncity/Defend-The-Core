#include "enemyShield.h"
#include "utils.h"
#include "player.h"
#include "core.h"
#include <stdio.h> 
#include <stdlib.h>

ShieldEnemy shieldEnemies[MAX_SHIELD_ENEMIES];
int enemyCountShield = 0;
double spawnTimerShield = 0;
Texture2D shieldEnemyTexture;
Texture2D shieldEnemyShieldTexture;

void spawnShieldEnemies(){
        if(spawnTimerShield > SHIELD_SPAWN_RATE / (difficulty / 2)){
        int side = GetRandomValue(0, 3);
        ShieldEnemy enemy;

        switch(side){
            case 0:
                enemy.pos.x = GetRandomValue(0, screenWidth);
                enemy.pos.y = 0;
                break;
            case 1:
                enemy.pos.x = GetRandomValue(0, screenWidth);
                enemy.pos.y = screenHeight;
                break;
            case 2:
                enemy.pos.x = 0;
                enemy.pos.y = GetRandomValue(0, screenHeight);
                break;
            case 3:
                enemy.pos.x = screenWidth;
                enemy.pos.y = GetRandomValue(0, screenHeight);
                break;
        }
        enemy.speed = GetRandomValue(75, 225);
        enemy.coinValue = GetRandomValue(10, 25);
        enemy.coinValue *= difficulty * 2;
        enemy.alive = true;
        enemy.damage = GetRandomValue(10, 25);
        enemy.damage *= difficulty;
        enemy.index = enemyCountShield;
        enemy.health = 50;
        enemy.health *= difficulty;
        enemy.maxHealth = 50;
        enemy.maxHealth *= difficulty;
        enemy.radius = 36;
        enemy.shieldMaxHealth = 100;
        enemy.shieldMaxHealth *= difficulty * 2;
        enemy.shieldHealth = enemy.shieldMaxHealth;
        enemy.shieldActive = true;
        
        enemy.healthBar = (HealthBar){enemy.health, enemy.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 + 64, 128, 4}, RED, BLACK};

        if(enemy.damage > 17.5){
            enemy.coinValue *= 2;
        }

        shieldEnemies[enemyCountShield] = enemy;
        spawnTimerShield = 0;
        //printf("Spawned enemy at %f, %f\n", enemy.pos.x, enemy.pos.y);
        enemyCountShield++;
    }
}


void handleShieldEnemies(){
    spawnTimerShield += GetFrameTime();
    for(int i = 0; i < MAX_SHIELD_ENEMIES; i++){
        if(shieldEnemies[i].alive == false){
            break;
        }
        if(shieldEnemies[i].health <= 0){
            player.coins += shieldEnemies[i].coinValue;
            destroyShieldEnemy(i);
        }


        double angleBetweenCore = atan2(core.rect.y + core.rect.height / 2 - shieldEnemies[i].pos.y, core.rect.x + core.rect.width / 2 - shieldEnemies[i].pos.x);
        if(angleBetweenCore < 0){
            angleBetweenCore += 2 * PI;
        }
        else if(angleBetweenCore > 2 * PI){
            angleBetweenCore -= 2 * PI;
        }
        shieldEnemies[i].direction = angleBetweenCore;


        shieldEnemies[i].pos.x += cos(angleBetweenCore) * shieldEnemies[i].speed * deltaTime;
        shieldEnemies[i].pos.y += sin(angleBetweenCore) * shieldEnemies[i].speed * deltaTime;

        shieldEnemies[i].healthBar.rect.x = shieldEnemies[i].pos.x - shieldEnemies[i].radius - shieldEnemies[i].healthBar.rect.width / 2;
        shieldEnemies[i].healthBar.rect.y = shieldEnemies[i].pos.y - shieldEnemies[i].radius - shieldEnemies[i].healthBar.rect.height / 2;
        

        if (CheckCollisionCircleRec(shieldEnemies[i].pos, shieldEnemies[i].radius, core.rect)){
            core.health -= shieldEnemies[i].damage;
            printf("Core health: %f\n", core.health);
            destroyShieldEnemy(i);
        }
        if (CheckCollisionCircleRec(shieldEnemies[i].pos, shieldEnemies[i].radius, player.hitbox)){
            player.health -= shieldEnemies[i].damage;
            destroyShieldEnemy(i);
        }
        if(shieldEnemies[i].shieldHealth <= 0){
            shieldEnemies[i].shieldActive = false;
        }
        Rectangle shieldRectOld = (Rectangle){shieldEnemies[i].pos.x + cos(angleBetweenCore) * 50, shieldEnemies[i].pos.y + sin(angleBetweenCore) * 50,
                          8*2, 32*4};
        DrawTextureEx(shieldEnemyTexture, (Vector2){shieldEnemies[i].pos.x - (shieldEnemies[i].radius / 2), shieldEnemies[i].pos.y - (shieldEnemies[i].radius / 2)}, 0, 2, WHITE);
        if(shieldEnemies[i].shieldActive == true){
            DrawTexturePro(shieldEnemyShieldTexture, (Rectangle){0, 0, 16, 32},
                        shieldRectOld, (Vector2){8, 16*4}, radToDeg(angleBetweenCore), (Color){255, 255, 255, shieldEnemies[i].shieldHealth / shieldEnemies[i].shieldMaxHealth * 200});
        }
        shieldRectOld.x -= 8;
        shieldRectOld.y -= 16*4;
        shieldEnemies[i].shieldRect = shieldRectOld;
        Vector2* shieldRectPointss;
        shieldRectPointss = rotateRect(shieldRectOld, shieldEnemies[i].direction, (Vector2){shieldRectOld.x + shieldRectOld.width / 2, shieldRectOld.y + shieldRectOld.height / 2}); 
        shieldEnemies[i].shieldRectPoints = shieldRectPointss;
        
        
        
        //DrawLineEx(shieldRectPointss[0], shieldRectPointss[1], 2, (Color){255, 255, 255, shieldEnemies[i].shieldHealth / shieldEnemies[i].shieldMaxHealth * 200});
        //DrawLineEx(shieldRectPointss[0], shieldRectPointss[2], 2, (Color){255, 255, 255, shieldEnemies[i].shieldHealth / shieldEnemies[i].shieldMaxHealth * 200});
        //DrawLineEx(shieldRectPointss[2], shieldRectPointss[3], 2, (Color){255, 255, 255, shieldEnemies[i].shieldHealth / shieldEnemies[i].shieldMaxHealth * 200});
        //DrawLineEx(shieldRectPointss[3], shieldRectPointss[1], 2, (Color){255, 255, 255, shieldEnemies[i].shieldHealth / shieldEnemies[i].shieldMaxHealth * 200});


        DrawRectangle(shieldEnemies[i].healthBar.rect.x, shieldEnemies[i].healthBar.rect.y,
                  shieldEnemies[i].healthBar.rect.width, shieldEnemies[i].healthBar.rect.height, shieldEnemies[i].healthBar.backgroundColor);
        DrawRectangle(shieldEnemies[i].healthBar.rect.x, shieldEnemies[i].healthBar.rect.y,
                  shieldEnemies[i].healthBar.rect.width * (shieldEnemies[i].health / shieldEnemies[i].maxHealth), shieldEnemies[i].healthBar.rect.height, shieldEnemies[i].healthBar.barColor);
    }
}


void destroyShieldEnemy(int enemyIndex){
    int lastUsedSlot = enemyCountShield - 1;
    if(lastUsedSlot == -1){
        printf("Add more memory dumbass, enemy array full");
        return;
    }
    ShieldEnemy enemyCP = shieldEnemies[lastUsedSlot];
    enemyCP.index = enemyIndex;
    enemyCP.alive = true;
    shieldEnemies[enemyIndex] = enemyCP;
    shieldEnemies[lastUsedSlot] = (ShieldEnemy){};
    enemyCountShield--;
} 