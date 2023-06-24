#include "enemyBasic.h"
#include "utils.h"
#include "player.h"
#include "core.h"
#include <stdio.h> 



BasicEnemy BasicEnemies[MAX_BASIC_ENEMIES];
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

        BasicEnemies[enemyCountBasic] = enemy;
        spawnTimerBasic = 0;
        //printf("Spawned enemy at %f, %f\n", enemy.pos.x, enemy.pos.y);
        enemyCountBasic++;
    }
}


void handleBasicEnemies(){
    spawnTimerBasic += GetFrameTime();
    for(int i = 0; i < MAX_BASIC_ENEMIES; i++){
        if(BasicEnemies[i].alive == false){
            break;
        }
        if(BasicEnemies[i].health <= 0){
            player.coins += BasicEnemies[i].coinValue;
            destroyBasicEnemy(i);
        }


        double angleBetweenCore = atan2(core.rect.y + core.rect.height / 2 - BasicEnemies[i].pos.y, core.rect.x + core.rect.width / 2 - BasicEnemies[i].pos.x);
        if(angleBetweenCore < 0){
            angleBetweenCore += 2 * PI;
        }
        else if(angleBetweenCore > 2 * PI){
            angleBetweenCore -= 2 * PI;
        }
        BasicEnemies[i].direction = angleBetweenCore;


        BasicEnemies[i].pos.x += cos(angleBetweenCore) * BasicEnemies[i].speed * deltaTime;
        BasicEnemies[i].pos.y += sin(angleBetweenCore) * BasicEnemies[i].speed * deltaTime;

        BasicEnemies[i].healthBar.rect.x = BasicEnemies[i].pos.x - BasicEnemies[i].radius - BasicEnemies[i].healthBar.rect.width / 2;
        BasicEnemies[i].healthBar.rect.y = BasicEnemies[i].pos.y - BasicEnemies[i].radius - BasicEnemies[i].healthBar.rect.height / 2;

        if (CheckCollisionCircleRec(BasicEnemies[i].pos, BasicEnemies[i].radius, core.rect)){
            core.health -= BasicEnemies[i].damage;
            printf("Core health: %f\n", core.health);
            destroyBasicEnemy(i);
        }
        if (CheckCollisionCircleRec(BasicEnemies[i].pos, BasicEnemies[i].radius, player.hitbox)){
            player.health -= BasicEnemies[i].damage;
            destroyBasicEnemy(i);
        }
        DrawCircle(BasicEnemies[i].pos.x, BasicEnemies[i].pos.y, BasicEnemies[i].radius, BasicEnemies[i].color);
        DrawRectangle(BasicEnemies[i].healthBar.rect.x, BasicEnemies[i].healthBar.rect.y,
                  BasicEnemies[i].healthBar.rect.width, BasicEnemies[i].healthBar.rect.height, BasicEnemies[i].healthBar.backgroundColor);
        DrawRectangle(BasicEnemies[i].healthBar.rect.x, BasicEnemies[i].healthBar.rect.y,
                  BasicEnemies[i].healthBar.rect.width * (BasicEnemies[i].health / BasicEnemies[i].maxHealth), BasicEnemies[i].healthBar.rect.height, BasicEnemies[i].healthBar.barColor);
    }
}


void destroyBasicEnemy(int enemyIndex){
    int lastUsedSlot = enemyCountBasic - 1;
    if(lastUsedSlot == -1){
        printf("Add more memory dumbass, enemy array full");
        return;
    }
    BasicEnemy enemyCP = BasicEnemies[lastUsedSlot];
    enemyCP.index = enemyIndex;
    enemyCP.alive = true;
    BasicEnemies[enemyIndex] = enemyCP;
    BasicEnemies[lastUsedSlot] = (BasicEnemy){(Vector2){0, 0}, 0, 0, 0, 0, (Color){0, 0, 0, 0}, false};
    enemyCountBasic--;
}