#include "enemyBasic.h"
#include "utils.h"
#include "player.h"
#include "core.h"
#include <stdio.h> 



BasicEnemy basicEnemies[MAX_BASIC_ENEMIES];
int enemyCountBasic = 0;
double spawnTimerBasic = 0;


void spawnBasicEnemies(){
    if(spawnTimerBasic > BASIC_SPAWN_RATE / (difficulty / 2)){
        int side = GetRandomValue(0, 3);
        BasicEnemy enemy;

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
        enemy.coinValue *= difficulty * 2;
        enemy.alive = true;
        enemy.damage = GetRandomValue(10, 25);
        enemy.damage *= difficulty;
        enemy.index = enemyCountBasic;
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

        basicEnemies[enemyCountBasic] = enemy;
        spawnTimerBasic = 0;
        //printf("Spawned enemy at %f, %f\n", enemy.pos.x, enemy.pos.y);
        enemyCountBasic++;
    }
}


void handleBasicEnemies(){
    spawnTimerBasic += GetFrameTime();
    for(int i = 0; i < MAX_BASIC_ENEMIES; i++){
        if(basicEnemies[i].alive == false){
            break;
        }
        if(basicEnemies[i].health <= 0){
            player.coins += basicEnemies[i].coinValue;
            destroyBasicEnemy(i);
        }


        double angleBetweenCore = atan2(core.rect.y + core.rect.height / 2 - basicEnemies[i].pos.y, core.rect.x + core.rect.width / 2 - basicEnemies[i].pos.x);
        if(angleBetweenCore < 0){
            angleBetweenCore += 2 * PI;
        }
        else if(angleBetweenCore > 2 * PI){
            angleBetweenCore -= 2 * PI;
        }
        basicEnemies[i].direction = angleBetweenCore;


        basicEnemies[i].pos.x += cos(angleBetweenCore) * basicEnemies[i].speed * deltaTime;
        basicEnemies[i].pos.y += sin(angleBetweenCore) * basicEnemies[i].speed * deltaTime;

        basicEnemies[i].healthBar.rect.x = basicEnemies[i].pos.x - basicEnemies[i].radius - basicEnemies[i].healthBar.rect.width / 2;
        basicEnemies[i].healthBar.rect.y = basicEnemies[i].pos.y - basicEnemies[i].radius - basicEnemies[i].healthBar.rect.height / 2;

        if (CheckCollisionCircleRec(basicEnemies[i].pos, basicEnemies[i].radius, core.rect)){
            core.health -= basicEnemies[i].damage;
            printf("Core health: %f\n", core.health);
            destroyBasicEnemy(i);
        }
        if (CheckCollisionCircleRec(basicEnemies[i].pos, basicEnemies[i].radius, player.hitbox)){
            player.health -= basicEnemies[i].damage;
            destroyBasicEnemy(i);
        }
        DrawCircle(basicEnemies[i].pos.x, basicEnemies[i].pos.y, basicEnemies[i].radius, basicEnemies[i].color);
        DrawRectangle(basicEnemies[i].healthBar.rect.x, basicEnemies[i].healthBar.rect.y,
                  basicEnemies[i].healthBar.rect.width, basicEnemies[i].healthBar.rect.height, basicEnemies[i].healthBar.backgroundColor);
        DrawRectangle(basicEnemies[i].healthBar.rect.x, basicEnemies[i].healthBar.rect.y,
                  basicEnemies[i].healthBar.rect.width * (basicEnemies[i].health / basicEnemies[i].maxHealth), basicEnemies[i].healthBar.rect.height, basicEnemies[i].healthBar.barColor);
    }
}


void destroyBasicEnemy(int enemyIndex){
    int lastUsedSlot = enemyCountBasic - 1;
    if(lastUsedSlot == -1){
        printf("Add more memory dumbass, enemy array full");
        return;
    }
    BasicEnemy enemyCP = basicEnemies[lastUsedSlot];
    enemyCP.index = enemyIndex;
    enemyCP.alive = true;
    basicEnemies[enemyIndex] = enemyCP;
    basicEnemies[lastUsedSlot] = (BasicEnemy){(Vector2){0, 0}, 0, 0, 0, 0, (Color){0, 0, 0, 0}, false};
    enemyCountBasic--;
}