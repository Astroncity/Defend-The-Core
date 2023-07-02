#include "snipertower.h"
#include "bullet.h"
#include "player.h"
#include "utils.h"
#include "enemies/enemies.h"
#include <stdio.h> 




double defaultSniperTowerRange = 500;
int sniperTowerCount = 0;
SniperTower sniperTowers[520];
Texture2D sniperTowerBaseTexture;
Texture2D sniperTowerTurretTexture;
Sound sniperSnd;


static GeneralEnemy compareEnemyDistances(SniperTower* tower, BasicEnemy basicEnemy, SplitterEnemy splitterEnemy, ShieldEnemy shieldEnemy);
static BasicEnemy targetBasicEnemy(SniperTower *tower);
static SplitterEnemy targetSplitterEnemy(SniperTower *tower);
static ShieldEnemy targetShieldEnemy(SniperTower *tower);

double distances[10];

void spawnSniperTower(){
    SniperTower tower;
    tower.rect.height = 64;
    tower.rect.width = 64;

    int towerPosX = virtualMousePos.x - (tower.rect.width / 2);
    int towerPosY = virtualMousePos.y - (tower.rect.height / 2);

    tower.rect.x = (double)roundToNearestMutiple(towerPosX, 64);
    tower.rect.y = (double)roundToNearestMutiple(towerPosY, 64);
    tower.fireRate = 1.0f;
    tower.range = defaultSniperTowerRange;
    tower.damage = 10.0f;
    tower.level = 1;
    tower.timer = 0;

    printf("Sniper tower placed at (%f, %f)\n", tower.rect.x, tower.rect.y);
    sniperTowers[sniperTowerCount] = tower;
    sniperTowerCount++;
}


void handleSniperTowers(){
    double angleBetweenEnemy = 0;
    for(int i = 0; i < sniperTowerCount; i++){
        sniperTowers[i].timer += GetFrameTime();
        DrawTextureEx(sniperTowerBaseTexture, (Vector2){sniperTowers[i].rect.x, sniperTowers[i].rect.y}, 0, 2, WHITE);
        DrawTexturePro(sniperTowerTurretTexture, (Rectangle){0, 0, 44, 64}, (Rectangle){sniperTowers[i].rect.x + (sniperTowers[i].rect.width / 2),
                    sniperTowers[i].rect.y + (sniperTowers[i].rect.height / 2),
                    44, 64}, (Vector2){21.5, 42.5}, radToDeg(sniperTowers[i].lastAngle) + 90, WHITE);
        if(sniperTowers[i].timer < sniperTowers[i].fireRate - 0.5){
            continue;
        }
        BasicEnemy closestBasicEnemy = targetBasicEnemy(&sniperTowers[i]);
        SplitterEnemy closestSplitterEnemy = targetSplitterEnemy(&sniperTowers[i]);
        ShieldEnemy closestShieldEnemy = targetShieldEnemy(&sniperTowers[i]);

        GeneralEnemy closestEnemy = compareEnemyDistances(&sniperTowers[i], closestBasicEnemy, closestSplitterEnemy, closestShieldEnemy);
        




        if(sniperTowers[i].shortestDist > sniperTowers[i].range){
            continue;
        }
        double framesToEnemy = sniperTowers[i].shortestDist / (2500 * deltaTime);
        if(framesToEnemy >= 1){
            closestEnemy.pos.x += cos(closestEnemy.direction) * framesToEnemy * deltaTime * closestEnemy.speed;
            closestEnemy.pos.y += sin(closestEnemy.direction) * framesToEnemy * deltaTime * closestEnemy.speed;
        }

        angleBetweenEnemy = atan2(closestEnemy.pos.y - (sniperTowers[i].rect.y + sniperTowers[i].rect.height / 2), closestEnemy.pos.x - (sniperTowers[i].rect.x + sniperTowers[i].rect.width / 2));

        //DrawLine(sniperTowers[i].rect.x + 25, sniperTowers[i].rect.y + 25, closestEnemy.pos.x, closestEnemy.pos.y, (Color){255, 0, 0, 255});
        //DrawCircleGradient(closestEnemy.pos.x, closestEnemy.pos.y, 10, (Color){255, 0, 0, 255}, (Color){0, 0, 0, 0});
        //DrawLine(0, sniperTowers[i].rect.y + 25, screenWidth, sniperTowers[i].rect.y + 25, (Color){0, 0, 255, 255});
            sniperTowers[i].timer = 0;
            Bullet bullet;
            bullet.pos.x = sniperTowers[i].rect.x + sniperTowers[i].rect.width / 2;
            bullet.pos.y = sniperTowers[i].rect.y + sniperTowers[i].rect.height / 2;
            bullet.direction = angleBetweenEnemy;
            sniperTowers[i].lastAngle = angleBetweenEnemy;

            bullet.speed = 2500;
            bullet.active = true;
            bullet.index = bulletCount;
            bullet.size = 6;
            bullet.damage = sniperTowers[i].damage;
            bullet.color = MAGENTA;
            bullets[bulletCount] = bullet;
            bulletCount++;
            PlaySound(sniperSnd);
        if(sniperTowers[i].lastAngle < 0){
            sniperTowers[i].lastAngle += 2 * PI;
        }
        if(sniperTowers[i].lastAngle > 2 * PI){
            sniperTowers[i].lastAngle -= 2 * PI;
        }
        sniperTowers[i].shortestDist = 1000000;
        angleBetweenEnemy = 0;
    }   
}


void handleSniperTowerGUI(){
    double dist = 999999999;
    SniperTower closestTower;
    for(int i = 0; i < sniperTowerCount; i++){
        double distanceToTower = distance(player.center, (Vector2){sniperTowers[i].rect.x + sniperTowers[i].rect.width / 2, sniperTowers[i].rect.y + sniperTowers[i].rect.height / 2});
        if(distanceToTower < dist){
            dist = distanceToTower;
            closestTower = sniperTowers[i];
        }
    }
    if(dist < 100){
        DrawRectangle(closestTower.rect.x, closestTower.rect.y - 50, 50, 50, (Color){0, 255, 0, 255});
    }
}


static BasicEnemy targetBasicEnemy(SniperTower* tower){
    BasicEnemy closestEnemy;
    double shortestDist = 9999999;
    for(int j = 0; j < enemyCountBasic; j++){
        double distanceToEnemy = distance((Vector2){tower->rect.x + (tower->rect.width / 2), tower->rect.y + (tower->rect.height / 2)},
                                          (Vector2){basicEnemies[j].pos.x, basicEnemies[j].pos.y});
        if(distanceToEnemy < shortestDist){
            shortestDist = distanceToEnemy;
            closestEnemy = basicEnemies[j];
        }
    }
    distances[0] = shortestDist;
    return closestEnemy;
}

static SplitterEnemy targetSplitterEnemy(SniperTower* tower){
    SplitterEnemy closestEnemy;
    double shortestDist = 9999999;
    for(int j = 0; j < enemyCountSplitter; j++){
        double distanceToEnemy = distance((Vector2){tower->rect.x + (tower->rect.width / 2), tower->rect.y + (tower->rect.height / 2)},
                                          (Vector2){splitterEnemies[j].pos.x, splitterEnemies[j].pos.y});
        if(distanceToEnemy < shortestDist){
            shortestDist = distanceToEnemy;
            closestEnemy = splitterEnemies[j];
        }
    }
    distances[1] = shortestDist;
    return closestEnemy;
}

static ShieldEnemy targetShieldEnemy(SniperTower* tower){
    ShieldEnemy closestEnemy;
    double shortestDist = 9999999;
    for(int i = 0; i < enemyCountShield; i++){
        double distanceToEnemy = distance((Vector2){tower->rect.x + (tower->rect.width / 2), tower->rect.y + (tower->rect.height / 2)},
                                          (Vector2){shieldEnemies[i].pos.x, shieldEnemies[i].pos.y});
        if(distanceToEnemy < shortestDist){
            shortestDist = distanceToEnemy;
            closestEnemy = shieldEnemies[i];
        }
    }
    distances[2] = shortestDist;
    return closestEnemy;
}




static GeneralEnemy compareEnemyDistances(SniperTower* tower, BasicEnemy basicEnemy, SplitterEnemy splitterEnemy, ShieldEnemy shieldEnemy){
    GeneralEnemy closestEnemy;
    double shortestDist = 9999999;
    int shortestIndex = 999999;
    for(int i = 0; i < 3; i++){
        if(distances[i] < shortestDist){
            shortestDist = distances[i];
            shortestIndex = i;
        }
    }
    switch(shortestIndex){
        case 0:
            closestEnemy = (GeneralEnemy){basicEnemy.pos, basicEnemy.direction, basicEnemy.speed};
            tower->shortestDist = shortestDist;
            break;
        case 1:
            closestEnemy = (GeneralEnemy){splitterEnemy.pos, splitterEnemy.direction, splitterEnemy.speed};
            tower->shortestDist = shortestDist;
            break;
        case 2:
            closestEnemy = (GeneralEnemy){shieldEnemy.pos, shieldEnemy.direction, shieldEnemy.speed};
            tower->shortestDist = shortestDist;
            break;
    }
    return closestEnemy;
}

