#include "enemyShield.h"
#include "utils.h"
#include "player.h"
#include "core.h"
#include <stdio.h> 


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
        enemy.radius = 18;
        enemy.shieldMaxHealth = 100;
        enemy.shieldMaxHealth *= difficulty * 2;
        enemy.shieldHealth = enemy.shieldMaxHealth;
        
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

        DrawTextureEx(shieldEnemyTexture, (Vector2){shieldEnemies[i].pos.x - (shieldEnemies[i].radius / 2), shieldEnemies[i].pos.y - (shieldEnemies[i].radius / 2)}, 0, 2, WHITE);
        DrawTextureEx(shieldEnemyShieldTexture, (Vector2){cos(angleBetweenCore) * shieldEnemies[i].radius + shieldEnemies[i].pos.x - (shieldEnemies[i].radius / 2), sin(angleBetweenCore) * shieldEnemies[i].radius + shieldEnemies[i].pos.y - (shieldEnemies[i].radius / 2)}, 0, 2, (Color){255, 255, 255, shieldEnemies[i].shieldHealth / shieldEnemies[i].shieldMaxHealth * 255});



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