#include "enemySplitter.h"
#include "utils.h"
#include "player.h"
#include "core.h"
#include <stdio.h> 


SplitterEnemy SplitterEnemies[MAX_SPLITTER_ENEMIES];
int enemyCountSplitter = 0;
double spawnTimerSplitter = 0;
int splitterTextureRadius = 36;
Texture2D splitterEnemyTexture;


static void split(SplitterEnemy* parent, int dir);

void spawnSplitterEnemies(){
    if(spawnTimerSplitter > SPLITTER_SPAWN_RATE / (difficulty * 0.5) && difficulty > 1.5){
        int side = GetRandomValue(0, 3);
        SplitterEnemy enemy;

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
        enemy.speed = GetRandomValue(50, 200);
        enemy.coinValue = GetRandomValue(20, 50);
        enemy.coinValue *= difficulty * 2;
        enemy.alive = true;
        enemy.damage = GetRandomValue(10, 25);
        enemy.damage *= difficulty;
        enemy.index = enemyCountSplitter;
        enemy.health = 100;
        enemy.health *= difficulty;
        enemy.maxHealth = 100;
        enemy.maxHealth *= difficulty;
        enemy.splits = 0;
        enemy.radius = splitterTextureRadius;
        
        enemy.healthBar = (HealthBar){enemy.health, enemy.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 + 64, 128, 4}, RED, BLACK};

        SplitterEnemies[enemyCountSplitter] = enemy;
        spawnTimerSplitter = 0;
        //printf("Spawned enemy at %f, %f\n", enemy.pos.x, enemy.pos.y);
        enemyCountSplitter++;
    }
}


void handleSplitterEnemies(){
    spawnTimerSplitter += GetFrameTime();
    for(int i = 0; i < MAX_SPLITTER_ENEMIES; i++){
        if(SplitterEnemies[i].alive == false){
            break;
        }
        if(SplitterEnemies[i].health <= 0){
            player.coins += SplitterEnemies[i].coinValue;
            if(SplitterEnemies[i].splits < SPLITTER_MAX_SPLITS){
                split(&SplitterEnemies[i], 0);
                split(&SplitterEnemies[i], 1);
            }
            destroySplitterEnemy(i);
        }


        double angleBetweenCore = atan2(core.rect.y + core.rect.height / 2 - SplitterEnemies[i].pos.y, core.rect.x + core.rect.width / 2 - SplitterEnemies[i].pos.x);
        if(angleBetweenCore < 0){
            angleBetweenCore += 2 * PI;
        }
        else if(angleBetweenCore > 2 * PI){
            angleBetweenCore -= 2 * PI;
        }
        SplitterEnemies[i].direction = angleBetweenCore;


        SplitterEnemies[i].pos.x += cos(angleBetweenCore) * SplitterEnemies[i].speed * deltaTime;
        SplitterEnemies[i].pos.y += sin(angleBetweenCore) * SplitterEnemies[i].speed * deltaTime;

        SplitterEnemies[i].healthBar.rect.x = SplitterEnemies[i].pos.x - splitterTextureRadius - SplitterEnemies[i].healthBar.rect.width / 2;
        SplitterEnemies[i].healthBar.rect.y = SplitterEnemies[i].pos.y - splitterTextureRadius - SplitterEnemies[i].healthBar.rect.height / 2;

        if (CheckCollisionCircleRec(SplitterEnemies[i].pos, splitterTextureRadius, core.rect)){
            core.health -= SplitterEnemies[i].damage;
            printf("Core health: %f\n", core.health);
            destroySplitterEnemy(i);
        }
        if (CheckCollisionCircleRec(SplitterEnemies[i].pos, splitterTextureRadius, player.hitbox)){
            player.health -= SplitterEnemies[i].damage;
            destroySplitterEnemy(i);
        }
        //DrawCircle(SplitterEnemies[i].pos.x, SplitterEnemies[i].pos.y, splitterTextureRadius, SplitterEnemies[i].color);
        DrawTextureEx(splitterEnemyTexture, (Vector2){SplitterEnemies[i].pos.x - (SplitterEnemies[i].radius / 2), SplitterEnemies[i].pos.y - (SplitterEnemies[i].radius / 2)}, 0, SplitterEnemies[i].radius / 18, WHITE);
        DrawCircle(SplitterEnemies[i].pos.x, SplitterEnemies[i].pos.y, SplitterEnemies[i].radius / 2, (Color){75, 0, 130, 128});
        DrawRectangle(SplitterEnemies[i].healthBar.rect.x, SplitterEnemies[i].healthBar.rect.y,
                  SplitterEnemies[i].healthBar.rect.width, SplitterEnemies[i].healthBar.rect.height, SplitterEnemies[i].healthBar.backgroundColor);
        DrawRectangle(SplitterEnemies[i].healthBar.rect.x, SplitterEnemies[i].healthBar.rect.y,
                  SplitterEnemies[i].healthBar.rect.width * (SplitterEnemies[i].health / SplitterEnemies[i].maxHealth), SplitterEnemies[i].healthBar.rect.height, SplitterEnemies[i].healthBar.barColor);
    }
}



static void split(SplitterEnemy* parent, int dir){
    SplitterEnemy child;

    switch(dir){
        case 0:
            child.pos.x = parent->pos.x;
            child.pos.y = parent->pos.y + 32;
            break;
        case 1:
            child.pos.x = parent->pos.x;
            child.pos.y = parent->pos.y - 32;
            break;
    }


    child.speed = parent->speed;
    child.coinValue = parent->coinValue * SPLITTER_SPLIT_FACTOR;
    child.alive = true;
    child.damage = parent->damage * SPLITTER_SPLIT_FACTOR;
    child.index = enemyCountSplitter;
    child.health = parent -> maxHealth * SPLITTER_SPLIT_FACTOR;
    child.maxHealth = parent -> maxHealth * SPLITTER_SPLIT_FACTOR;
    child.splits = parent->splits + 1;
    child.radius = parent -> radius;
    child.direction = parent->direction;
    
    child.healthBar = (HealthBar){child.health, child.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 + 64, 128, 4}, RED, BLACK};
    SplitterEnemies[enemyCountSplitter] = child;
    printf("%d\n", child.radius);

    enemyCountSplitter++;
}



void destroySplitterEnemy(int enemyIndex){
    int lastUsedSlot = enemyCountSplitter - 1;
    if(lastUsedSlot == -1){
        printf("Add more memory dumbass, enemy array full");
        return;
    }
    SplitterEnemy enemyCP = SplitterEnemies[lastUsedSlot];
    enemyCP.index = enemyIndex;
    enemyCP.alive = true;
    SplitterEnemies[enemyIndex] = enemyCP;
    SplitterEnemies[lastUsedSlot] = (SplitterEnemy){};
    enemyCountSplitter--;
}