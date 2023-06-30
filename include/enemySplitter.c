#include "enemySplitter.h"
#include "utils.h"
#include "player.h"
#include "core.h"
#include <stdio.h> 


SplitterEnemy splitterEnemies[MAX_SPLITTER_ENEMIES];
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

        splitterEnemies[enemyCountSplitter] = enemy;
        spawnTimerSplitter = 0;
        //printf("Spawned enemy at %f, %f\n", enemy.pos.x, enemy.pos.y);
        enemyCountSplitter++;
    }
}


void handleSplitterEnemies(){
    spawnTimerSplitter += GetFrameTime();
    for(int i = 0; i < MAX_SPLITTER_ENEMIES; i++){
        if(splitterEnemies[i].alive == false){
            break;
        }
        if(splitterEnemies[i].health <= 0){
            player.coins += splitterEnemies[i].coinValue;
            if(splitterEnemies[i].splits < SPLITTER_MAX_SPLITS){
                split(&splitterEnemies[i], 0);
                split(&splitterEnemies[i], 1);
            }
            destroySplitterEnemy(i);
        }


        double angleBetweenCore = atan2(core.rect.y + core.rect.height / 2 - splitterEnemies[i].pos.y, core.rect.x + core.rect.width / 2 - splitterEnemies[i].pos.x);
        if(angleBetweenCore < 0){
            angleBetweenCore += 2 * PI;
        }
        else if(angleBetweenCore > 2 * PI){
            angleBetweenCore -= 2 * PI;
        }
        splitterEnemies[i].direction = angleBetweenCore;


        splitterEnemies[i].pos.x += cos(angleBetweenCore) * splitterEnemies[i].speed * deltaTime;
        splitterEnemies[i].pos.y += sin(angleBetweenCore) * splitterEnemies[i].speed * deltaTime;

        splitterEnemies[i].healthBar.rect.x = splitterEnemies[i].pos.x - splitterTextureRadius - splitterEnemies[i].healthBar.rect.width / 2;
        splitterEnemies[i].healthBar.rect.y = splitterEnemies[i].pos.y - splitterTextureRadius - splitterEnemies[i].healthBar.rect.height / 2;

        if (CheckCollisionCircleRec(splitterEnemies[i].pos, splitterTextureRadius, core.rect)){
            core.health -= splitterEnemies[i].damage;
            printf("Core health: %f\n", core.health);
            destroySplitterEnemy(i);
        }
        if (CheckCollisionCircleRec(splitterEnemies[i].pos, splitterTextureRadius, player.hitbox)){
            player.health -= splitterEnemies[i].damage;
            destroySplitterEnemy(i);
        }
        //DrawCircle(splitterEnemies[i].pos.x, splitterEnemies[i].pos.y, splitterTextureRadius, splitterEnemies[i].color);
        DrawTextureEx(splitterEnemyTexture, (Vector2){splitterEnemies[i].pos.x - (splitterEnemies[i].radius / 2), splitterEnemies[i].pos.y - (splitterEnemies[i].radius / 2)}, 0, splitterEnemies[i].radius / 18, WHITE);
        //DrawCircle(splitterEnemies[i].pos.x, splitterEnemies[i].pos.y, splitterEnemies[i].radius / 2, (Color){75, 0, 130, 128});
        DrawRectangle(splitterEnemies[i].healthBar.rect.x, splitterEnemies[i].healthBar.rect.y,
                  splitterEnemies[i].healthBar.rect.width, splitterEnemies[i].healthBar.rect.height, splitterEnemies[i].healthBar.backgroundColor);
        DrawRectangle(splitterEnemies[i].healthBar.rect.x, splitterEnemies[i].healthBar.rect.y,
                  splitterEnemies[i].healthBar.rect.width * (splitterEnemies[i].health / splitterEnemies[i].maxHealth), splitterEnemies[i].healthBar.rect.height, splitterEnemies[i].healthBar.barColor);
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
    splitterEnemies[enemyCountSplitter] = child;
    printf("%d\n", child.radius);

    enemyCountSplitter++;
}



void destroySplitterEnemy(int enemyIndex){
    int lastUsedSlot = enemyCountSplitter - 1;
    if(lastUsedSlot == -1){
        printf("Add more memory dumbass, enemy array full");
        return;
    }
    SplitterEnemy enemyCP = splitterEnemies[lastUsedSlot];
    enemyCP.index = enemyIndex;
    enemyCP.alive = true;
    splitterEnemies[enemyIndex] = enemyCP;
    splitterEnemies[lastUsedSlot] = (SplitterEnemy){};
    enemyCountSplitter--;
}