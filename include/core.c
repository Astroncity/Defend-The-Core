#include "core.h"
#include "utils.h"
#include "player.h"
#include <stdio.h> 



double coreBonusTimer = 0;
Core core = {500, 500, (Rectangle){screenWidth / 2 - (16 * CORESCALE), screenHeight / 2 - (16 * CORESCALE), 32 * CORESCALE, 32 * CORESCALE}, BLUE};
Texture2D coreTexture;

void handleCore(){
    coreBonusTimer += GetFrameTime();
    core.center = (Vector2){core.rect.x + core.rect.width / 2, core.rect.y + core.rect.height / 2};
    if(core.health <= 0){
        gameState = DEAD;
    }
    if(coreBonusTimer > 5 && core.health == core.maxHealth){
        coreBonusTimer = 0;
        player.coins += 15;
        printf("Core bonus! +15 coins\n");
    }

    core.healthBar.rect.x = core.center.x - core.healthBar.rect.width / 2;
    core.healthBar.rect.y = core.center.y - core.healthBar.rect.height - 300;



    //DrawTextureEx(coreTexture, (Vector2){core.rect.x, core.rect.y + 5}, 0, CORESCALE, (Color){0, 0, 0, 150});
    DrawTextureEx(coreTexture, (Vector2){core.rect.x, core.rect.y}, 0, CORESCALE, WHITE);
    
    //DrawRectangle(core.rect.x, core.rect.y, core.rect.width, core.rect.height, (Color){255, 0, 0, 100});

    DrawRectangle(core.healthBar.rect.x, core.healthBar.rect.y,
                  core.healthBar.rect.width, core.healthBar.rect.height, core.healthBar.backgroundColor);
    DrawRectangle(core.healthBar.rect.x, core.healthBar.rect.y,
                  core.healthBar.rect.width * (core.health / core.maxHealth), core.healthBar.rect.height, core.healthBar.barColor);
}