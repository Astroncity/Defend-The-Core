#include "player.h"
#include "../main.h"
#include "bullet.h"




Player player = {100, 100, 2, 10, 1, 90 * PI / 180, 0, (Trig)
                {(Vector2){screenWidth / 2 - 16, screenHeight / 2}, (Vector2){screenWidth / 2, screenHeight / 2 + 32}, (Vector2){screenWidth / 2 + 16, screenHeight / 2}}};

bool shotgunPurchased = false;

void handlePlayer(){
    Vector2 center = {(player.trig.a.x + player.trig.b.x + player.trig.c.x) / 3, (player.trig.a.y + player.trig.b.y + player.trig.c.y) / 3};
    player.center = center;
    double angleBetweenMouse = atan2(mousePos.y - center.y, mousePos.x - center.x);
    double angDiff = angleBetweenMouse - player.direction;

    if(radToDeg(angleBetweenMouse) > 360){player.direction -= degToRad(360);}
    else if(radToDeg(angleBetweenMouse) < 0){player.direction += degToRad(360);}
    
    if (IsKeyDown(KEY_LEFT_SHIFT)){player.speed = 600;}
    else{player.speed = 300;}  

    if(IsKeyDown(KEY_ESCAPE)){
        gameState = MAIN_MENU;
    }

    // Keyboard
    if(IsKeyDown(KEY_W)){translateTrig(&player.trig, cos(player.direction) * player.speed * deltaTime, sin(player.direction) * player.speed * deltaTime);}
    if(IsKeyDown(KEY_S)){translateTrig(&player.trig, -cos(player.direction) * player.speed * deltaTime, -sin(player.direction) * player.speed * deltaTime);}
    if(IsKeyDown(KEY_A)){translateTrig(&player.trig, -cos(player.direction + degToRad(90) * player.speed * deltaTime), -sin(player.direction + degToRad(90) * player.speed * deltaTime));}
    if(IsKeyDown(KEY_D)){translateTrig(&player.trig, cos(player.direction + degToRad(90)) * player.speed * deltaTime, sin(player.direction + degToRad(90)) * player.speed * deltaTime);}

    player.hitbox.x = player.center.x - player.hitbox.width / 2;
    player.hitbox.y = player.center.y - player.hitbox.height / 2;

    player.healthBar.rect.x = player.center.x - player.healthBar.rect.width / 2;
    player.healthBar.rect.y = player.center.y - player.healthBar.rect.height - 10;

    if(shotgunPurchased){
        shootShotgun(center, angleBetweenMouse);
    }
    else{
        shootBasic(center, angleBetweenMouse);
    }


    if(player.health <= 0){
        gameState = DEAD;
    }
    player.direction = angleBetweenMouse;
    rotateVector2(&player.trig.a, angDiff, center);
    rotateVector2(&player.trig.b, angDiff, center);
    rotateVector2(&player.trig.c, angDiff, center);
    center = (Vector2){(player.trig.a.x + player.trig.b.x + player.trig.c.x) / 3, (player.trig.a.y + player.trig.b.y + player.trig.c.y) / 3};
    player.center = center;
    DrawTriangle(player.trig.a, player.trig.b, player.trig.c, (Color){193, 225, 193, 255});
    DrawRectangle(player.healthBar.rect.x, player.healthBar.rect.y,
                  player.healthBar.rect.width, player.healthBar.rect.height, player.healthBar.backgroundColor);
    DrawRectangle(player.healthBar.rect.x, player.healthBar.rect.y,
                  player.healthBar.rect.width * (player.health / player.maxHealth), player.healthBar.rect.height, player.healthBar.barColor);
}   
