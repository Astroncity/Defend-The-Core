#include "enemyBasic.h"
#include "utils.h"
#include "player.h"
#include "core.h"
#include <stdio.h> 



Enemy ENEMIES[MAX_ENEMIES];
int enemyCount = 0;
double spawnTimer = 0;


void spawnEnemies(){
    if(spawnTimer > 0.1 / (difficulty / 2)){
        int side = GetRandomValue(0, 3);
        Enemy enemy;

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
        enemy.color = RED;
        enemy.coinValue = GetRandomValue(10, 25);
        enemy.coinValue *= difficulty;
        enemy.alive = true;
        enemy.damage = GetRandomValue(10, 25);
        enemy.damage *= difficulty;
        enemy.index = enemyCount;
        enemy.health = 50;
        enemy.health *= difficulty;
        enemy.maxHealth = 50;
        enemy.maxHealth *= difficulty;
        enemy.radius = 10;
        
        enemy.healthBar = (HealthBar){enemy.health, enemy.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 + 64, 128, 4}, RED, BLACK};

        if(enemy.damage > 17.5){
            enemy.color = ORANGE;
            enemy.coinValue *= 2;
        }

        ENEMIES[enemyCount] = enemy;
        spawnTimer = 0;
        //printf("Spawned enemy at %f, %f\n", enemy.pos.x, enemy.pos.y);
        enemyCount++;
    }
}


void handleEnemies(){
    spawnTimer += GetFrameTime();
    for(int i = 0; i < MAX_ENEMIES; i++){
        if(ENEMIES[i].alive == false){
            break;
        }
        if(ENEMIES[i].health <= 0){
            player.coins += ENEMIES[i].coinValue;
            destroyEnemy(i);
        }


        double angleBetweenCore = atan2(core.rect.y + core.rect.height / 2 - ENEMIES[i].pos.y, core.rect.x + core.rect.width / 2 - ENEMIES[i].pos.x);
        if(angleBetweenCore < 0){
            angleBetweenCore += 2 * PI;
        }
        else if(angleBetweenCore > 2 * PI){
            angleBetweenCore -= 2 * PI;
        }
        ENEMIES[i].direction = angleBetweenCore;


        ENEMIES[i].pos.x += cos(angleBetweenCore) * ENEMIES[i].speed * deltaTime;
        ENEMIES[i].pos.y += sin(angleBetweenCore) * ENEMIES[i].speed * deltaTime;

        ENEMIES[i].healthBar.rect.x = ENEMIES[i].pos.x - ENEMIES[i].radius - ENEMIES[i].healthBar.rect.width / 2;
        ENEMIES[i].healthBar.rect.y = ENEMIES[i].pos.y - ENEMIES[i].radius - ENEMIES[i].healthBar.rect.height / 2;

        if (CheckCollisionCircleRec(ENEMIES[i].pos, ENEMIES[i].radius, core.rect)){
            core.health -= ENEMIES[i].damage;
            printf("Core health: %f\n", core.health);
            destroyEnemy(i);
        }
        if (CheckCollisionCircleRec(ENEMIES[i].pos, ENEMIES[i].radius, player.hitbox)){
            player.health -= ENEMIES[i].damage;
            destroyEnemy(i);
        }
        DrawCircle(ENEMIES[i].pos.x, ENEMIES[i].pos.y, ENEMIES[i].radius, ENEMIES[i].color);
        DrawRectangle(ENEMIES[i].healthBar.rect.x, ENEMIES[i].healthBar.rect.y,
                  ENEMIES[i].healthBar.rect.width, ENEMIES[i].healthBar.rect.height, ENEMIES[i].healthBar.backgroundColor);
        DrawRectangle(ENEMIES[i].healthBar.rect.x, ENEMIES[i].healthBar.rect.y,
                  ENEMIES[i].healthBar.rect.width * (ENEMIES[i].health / ENEMIES[i].maxHealth), ENEMIES[i].healthBar.rect.height, ENEMIES[i].healthBar.barColor);
    }

}


void destroyEnemy(int enemyIndex){
    int lastUsedSlot = enemyCount - 1;
    if(lastUsedSlot == -1){
        printf("Add more memory dumbass, enemy array full");
        return;
    }
    Enemy enemyCP = ENEMIES[lastUsedSlot];
    enemyCP.index = enemyIndex;
    enemyCP.alive = true;
    ENEMIES[enemyIndex] = enemyCP;
    ENEMIES[lastUsedSlot] = (Enemy){(Vector2){0, 0}, 0, 0, 0, 0, (Color){0, 0, 0, 0}, false};
    enemyCount--;
}