#include "include/raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>

#define screenWidth 1920
#define screenHeight 1080
#define MAX_ENEMIES 100
#define MAX_BULLETS 10000
#define PELLET_COUNT 5
#define SHOTGUN_PELLET_SPREAD 4




/**----------------------------------------------
 * todo                  TODO
 * todo make healthbar system--DONE
 * todo improve spawning so that it starts at the edge of the screen
 * todo add audio
 * todo make a store
 * todo make a settings menu
 * todo make a pause menu
 * todo make a game over screen--DONE
 * todo find way to save settings/data--DONE
 *---------------------------------------------**/



typedef enum GameState{
        MAIN_MENU,
        PLAYING,
        STORE,
        SETTINGS,
        PAUSED,
        DEAD
}GameState;

typedef struct Button{
    int x;
    int y;
    int width;
    int height;
    int borderX;
    int borderY;
    int borderWidth;
    int borderHeight;
    int borderThickness;
    int textOffset;
    double scale;
    char* text;
    Color color;
    Color borderColor;
    void (*func)();
    int fontSize;
}Button;

typedef struct HealthBar{
    double value;
    double max;
    Rectangle rect;
    Color barColor;
    Color backgroundColor;
}HealthBar;

typedef struct Trig{
    Vector2 a;
    Vector2 b;
    Vector2 c;
}Trig;

typedef struct Player{
    double health;
    double maxHealth;
    double speed;
    double damage;
    double attackSpeed;
    double direction;
    int coins;
    Trig trig;
    HealthBar healthBar;
    Vector2 center;
    Rectangle hitbox;
}Player;

typedef struct Core{
    double health;
    double maxHealth;
    Rectangle rect;
    Color color;
    HealthBar healthBar;
    Vector2 center;
}Core;

typedef struct Enemy{
    Vector2 pos;
    double health;
    double maxHealth;
    double speed;
    double damage;
    Color color;
    bool alive;
    int index;
    int coinValue;
    HealthBar healthBar;
    int radius;
}Enemy;

typedef struct Bullet{
    double speed;
    double direction;
    Vector2 pos;
    bool active;
    int index;
}Bullet;

void drawMainMenu();
Button initButton(int x, int y, int width, int height, int borderThickness, 
                  int textOffset, const char* text, Color color, int fontSize, void (*func)());
void handleButton(Button* button);
void initMainMenu();
void storeButtonFunc();
void playButtonFunc();
void settingsButtonFunc();
void handlePlayer();
void rotateVector2(Vector2* vector, float angle, Vector2 center);
double degToRad(double angle);
double radToDeg(double angle);
void handleCore();
double coreBonusTimer;
void handleEnemies();
int lastUsedEnemySlot();
void spawnEnemies();
double spawnTimer;
void destroyEnemy(int enemyIndex);
int lastUsedBulletSlot();
void handleBullets();
void handleCoreHealthBar();
void deathScreen();
void resetGame();
void savePlayerData();
void loadPlayerData();
void upgradeDamage();
void upgradeHealth();
void handleStore();
void initStoreMenu();
void purchaseShotgun();
void shootBasic(Vector2 center, double angleBetweenMouse);
void shootShotgun(Vector2 center, double angleBetweenMouse);
void translateTrig(Trig* trig, double x, double y);
void clamp(double* value, double max);
void scaleDifficulty(double runtime);




// UI things
Button playButton;
Button storeButton;
Button settingsButton;
Button upgradeHealthButton;
Button upgradeDamageButton;
Button shotgunButton;

bool darkMode = false;
Vector2 mousePos;
Color backgroundColor;

GameState gameState = MAIN_MENU;
double deltaTime = 0;
double runtime = 0;
double difficulty = 1;

Player player = {100, 100, 2, 10, 1, 90 * PI / 180, 0, (Trig)
                {(Vector2){screenWidth / 2 - 16, screenHeight / 2}, (Vector2){screenWidth / 2, screenHeight / 2 + 32}, (Vector2){screenWidth / 2 + 16, screenHeight / 2}}};

Core core = {500, 500, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 - 16, 32, 32}, BLUE};
Enemy ENEMIES[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
int enemyCount = 0;
int bulletCount = 0;
double upgradeDamageCost;
double upgradeHealthCost;
int shotgunCost = 500;
bool shotgunPurchased = false;

int main(void){
    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");
    SetTargetFPS(144);  

    loadPlayerData();  

    initMainMenu();
    initStoreMenu();
    SetExitKey(KEY_F12);
    if(darkMode){
        backgroundColor = BLACK;
    }
    else{
        backgroundColor = WHITE;
    }

    player.healthBar = (HealthBar){player.health, player.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 + 64, 128, 4}, GREEN, RED};
    player.hitbox = (Rectangle){0, 0, 28, 28};
    core.healthBar = (HealthBar){core.health, core.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 - 128, 256, 4}, SKYBLUE, RED};
    upgradeDamageCost = player.damage * 10;
    upgradeHealthCost = player.maxHealth * 0.5;

    while (!WindowShouldClose()){
        mousePos = GetMousePosition();
        BeginDrawing();
        ClearBackground(backgroundColor);
        deltaTime = GetFrameTime();
        runtime += deltaTime;

        switch(gameState){
            case MAIN_MENU:
                handleButton(&playButton);
                handleButton(&storeButton);
                handleButton(&settingsButton);
                break;
            case PLAYING:
                handleCore();
                handlePlayer();
                handleEnemies();
                spawnEnemies();
                handleBullets();
                handleCoreHealthBar();
                scaleDifficulty(runtime);
                break;
            case STORE:
                handleStore();
                break;
            case SETTINGS:
                break;
            case PAUSED:
                break;
            case DEAD:
                deathScreen();
                break;
        }
        DrawFPS(10, 50);
        DrawLine(player.center.x, player.center.y, mousePos.x, mousePos.y, RED);
        DrawText(TextFormat("Coins: %d", player.coins), 10, 10, 20, GREEN);
        EndDrawing();

    }
    CloseWindow();
    savePlayerData();

    return 0;
}

void initMainMenu(){
    playButton = initButton(screenWidth/2 - 187, screenHeight/2 - 190, 374, 130, 10, 40, "Play", WHITE, 50, playButtonFunc);
    storeButton = initButton(screenWidth/2 - 187, screenHeight/2, 374, 130, 10, 45, "Store", WHITE, 50, storeButtonFunc);
    settingsButton = initButton(screenWidth/2 - 187, screenHeight/2 + 190, 374, 130, 10, 95, "Settings", WHITE, 50, settingsButtonFunc);
}

Button initButton(int x, int y, int width, int height, int borderThickness,
                  int textOffset, const char* text, Color color,  int fontSize, void (*func)()){
    Button button;
    button.x = x;
    button.y = y;
    button.width = width;
    button.height = height;
    button.textOffset = textOffset;
    button.func = func;
    button.text = malloc((strlen(text) + 1) * sizeof(char)); // Allocate memory for text
    strcpy(button.text, text);
    button.color = color;
    button.borderColor = BLACK;
    button.borderThickness = borderThickness;
    button.borderX = button.x - button.borderThickness;
    button.borderY = button.y - button.borderThickness;
    button.borderWidth = button.width + (2 * button.borderThickness);
    button.borderHeight = button.height + (2 * button.borderThickness);
    button.fontSize = fontSize;
    return button;
}
void handleButton(Button* button){
    // border
    DrawRectangle(button -> borderX,
                  button -> borderY, 
                  button -> borderWidth, 
                  button -> borderHeight, 
                  button -> borderColor);
    // button  
    DrawRectangle(button -> x,
                  button -> y, 
                  button -> width, 
                  button -> height, 
                  button -> color);

    int centerX = button -> x + button -> width / 2;
    int centerY = button -> y + button -> height / 2;
    int startX = centerX - strlen(button -> text) * button -> fontSize / 2;
    int startY = centerY - button -> fontSize / 2;
    DrawText(button -> text, (startX + button -> textOffset), startY, button -> fontSize, BLACK);

    if (CheckCollisionPointRec(mousePos, (Rectangle){button -> borderX, button -> borderY, button -> borderWidth, button -> borderHeight})){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            button -> func();
        }
        button -> borderColor = BLUE;
    }
    else{
        button -> borderColor = BLACK;
    }

}

void playButtonFunc(){
    gameState = PLAYING;
    printf("Play button pressed\n");
}
void storeButtonFunc(){
    gameState = STORE;
    printf("Store button pressed\n");
}
void settingsButtonFunc(){
    gameState = SETTINGS;
    printf("Settings button pressed\n");
}

void handlePlayer(){
    Vector2 center = {(player.trig.a.x + player.trig.b.x + player.trig.c.x) / 3, (player.trig.a.y + player.trig.b.y + player.trig.c.y) / 3};
    player.center = center;
    double angleBetweenMouse = atan2(mousePos.y - center.y, mousePos.x - center.x);
    double angDiff = angleBetweenMouse - player.direction;

    if(radToDeg(angleBetweenMouse) > 360){
        player.direction -= degToRad(360);
    }
    else if(radToDeg(angleBetweenMouse) < 0){
        player.direction += degToRad(360);
    }
    
    if (IsKeyDown(KEY_LEFT_SHIFT)){
        player.speed = 600;
    }
    else{
        player.speed = 300;
    }   

    if(IsKeyDown(KEY_W)){
        translateTrig(&player.trig, cos(player.direction) * player.speed * deltaTime, sin(player.direction) * player.speed * deltaTime);
    }
    if(IsKeyDown(KEY_S)){
        translateTrig(&player.trig, -cos(player.direction) * player.speed * deltaTime, -sin(player.direction) * player.speed * deltaTime);
    }
    if(IsKeyDown(KEY_A)){
        translateTrig(&player.trig, -cos(player.direction + degToRad(90) * player.speed * deltaTime),
                      -sin(player.direction + degToRad(90) * player.speed * deltaTime));
    }
    if(IsKeyDown(KEY_D)){
        translateTrig(&player.trig, cos(player.direction + degToRad(90)) * player.speed * deltaTime,
                      sin(player.direction + degToRad(90)) * player.speed * deltaTime);
    }

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
    rotateVector2(&player.trig.a, angDiff, center);
    rotateVector2(&player.trig.b, angDiff, center);
    rotateVector2(&player.trig.c, angDiff, center);
    player.direction = angleBetweenMouse;
    DrawTriangle(player.trig.a, player.trig.b, player.trig.c, (Color){193, 225, 193, 255});
    DrawRectangle(player.healthBar.rect.x, player.healthBar.rect.y,
                  player.healthBar.rect.width, player.healthBar.rect.height, player.healthBar.backgroundColor);
    DrawRectangle(player.healthBar.rect.x, player.healthBar.rect.y,
                  player.healthBar.rect.width * (player.health / player.maxHealth), player.healthBar.rect.height, player.healthBar.barColor);
}   

void shootBasic(Vector2 center, double angleBetweenMouse){
    bool canShoot = true;

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && canShoot){
        canShoot = false;
        Bullet bullet;
        bullet.pos.x = center.x;
        bullet.pos.y = center.y;
        bullet.direction = angleBetweenMouse;
        bullet.speed = 750;
        bullet.active = true;
        bullet.index = bulletCount;
        bullets[bulletCount] = bullet;
        bulletCount++;
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        canShoot = true;
    }
}

void shootShotgun(Vector2 center, double angleBetweenMouse){
    bool canShoot = true;

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && canShoot){
        canShoot = false;
        for(int i = 0; i < PELLET_COUNT; i++){
            Bullet bullet;
            bullet.pos.x = center.x;
            bullet.pos.y = center.y;
            bullet.direction = angleBetweenMouse - degToRad(PELLET_COUNT + i * SHOTGUN_PELLET_SPREAD) + degToRad(PELLET_COUNT * SHOTGUN_PELLET_SPREAD / 2) + degToRad(PELLET_COUNT*0.6);
            bullet.speed = 750;
            bullet.active = true;
            bullet.index = bulletCount;
            bullets[bulletCount] = bullet;
            bulletCount++;
        }
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        canShoot = true;
    }
}

void rotateVector2(Vector2* vector, float angle, Vector2 center){
    // rotate all points of the triangle around its center
    float s = sin(angle);
    float c = cos(angle);

    // translate point back to origin:
    vector -> x -= center.x;
    vector -> y -= center.y;

    // rotate point
    float xnew = vector -> x * c - vector -> y * s;
    float ynew = vector -> x * s + vector -> y * c;

    // translate point back:
    vector -> x = xnew + center.x;
    vector -> y = ynew + center.y;
}

inline double degToRad(double angle){
    return angle * PI / 180;
}
inline double radToDeg(double angle){
    return angle * 180 / PI;
}

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
    DrawRectangle(core.rect.x, core.rect.y, core.rect.width, core.rect.height, core.color);
}

void spawnEnemies(){
    if(spawnTimer > 1 / (difficulty / 2)){
        Enemy enemy;
        enemy.pos.x = GetRandomValue(0, screenWidth);
        enemy.pos.y = GetRandomValue(0, screenHeight);
        enemy.speed = GetRandomValue(75, 225);
        enemy.color = RED;
        enemy.coinValue = GetRandomValue(1, 5);
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

        if(enemy.damage > 12.5){
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
            destroyEnemy(i);
            player.coins += ENEMIES[i].coinValue;
        }


        double angleBetweenCore = atan2(core.rect.y + core.rect.height / 2 - ENEMIES[i].pos.y, core.rect.x + core.rect.width / 2 - ENEMIES[i].pos.x);
        if(angleBetweenCore < 0){
            angleBetweenCore += 2 * PI;
        }
        else if(angleBetweenCore > 2 * PI){
            angleBetweenCore -= 2 * PI;
        }


        ENEMIES[i].pos.x += cos(angleBetweenCore) * ENEMIES[i].speed * deltaTime;
        ENEMIES[i].pos.y += sin(angleBetweenCore) * ENEMIES[i].speed * deltaTime;

        ENEMIES[i].healthBar.rect.x = ENEMIES[i].pos.x - ENEMIES[i].radius - ENEMIES[i].healthBar.rect.width / 2;
        ENEMIES[i].healthBar.rect.y = ENEMIES[i].pos.y - ENEMIES[i].radius - ENEMIES[i].healthBar.rect.height / 2;

        if (CheckCollisionCircleRec(ENEMIES[i].pos, ENEMIES[i].radius, core.rect)){
            destroyEnemy(i);
            core.health -= ENEMIES[i].damage;
            printf("Core health: %f\n", core.health);
        }
        if (CheckCollisionCircleRec(ENEMIES[i].pos, ENEMIES[i].radius, player.hitbox)){
            destroyEnemy(i);
            player.health -= ENEMIES[i].damage;
        }
        DrawCircle(ENEMIES[i].pos.x, ENEMIES[i].pos.y, ENEMIES[i].radius, (Color){25 * difficulty, 0, 0, 255});
        DrawRectangle(ENEMIES[i].healthBar.rect.x, ENEMIES[i].healthBar.rect.y,
                  ENEMIES[i].healthBar.rect.width, ENEMIES[i].healthBar.rect.height, ENEMIES[i].healthBar.backgroundColor);
        DrawRectangle(ENEMIES[i].healthBar.rect.x, ENEMIES[i].healthBar.rect.y,
                  ENEMIES[i].healthBar.rect.width * (ENEMIES[i].health / ENEMIES[i].maxHealth), ENEMIES[i].healthBar.rect.height, ENEMIES[i].healthBar.barColor);
    }

}

int lastUsedEnemySlot(){
    for(int i = 0; i < MAX_ENEMIES; i++){
        if(ENEMIES[i].alive == false){
            return i - 1;
        }
    }
    return -1;
}

int lastUsedBulletSlot(){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(bullets[i].active == false){
            return i - 1;
        }
    }
    return -1;
}

void destroyEnemy(int enemyIndex){
    int lastUsedSlot = lastUsedEnemySlot();
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

void destroyBullet(int bulletIndex){
    int lastUsedSlot = lastUsedBulletSlot();
    if(lastUsedSlot == -1){
        printf("Add more memory dumbass, bullet array full");
        return;
    }
    Bullet bulletCP = bullets[lastUsedSlot];
    bulletCP.index = bulletIndex;
    bulletCP.active = true;
    bullets[bulletIndex] = bulletCP;
    bullets[lastUsedSlot] = (Bullet){0, 0, (Vector2){0, 0}, false};
    bulletCount--;
}

void handleBullets(){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(bullets[i].active == false){
            break;
        }

        bullets[i].pos.x += cos(bullets[i].direction) * bullets[i].speed * deltaTime;
        bullets[i].pos.y += sin(bullets[i].direction) * bullets[i].speed * deltaTime;

        bullets[i].speed += 10.5;


        DrawCircle(bullets[i].pos.x, bullets[i].pos.y, 4, GREEN);
        // destory bullet if outside screen
        if((bullets[i].pos.x > screenWidth || bullets[i].pos.x < 0) || (bullets[i].pos.y < 0 || bullets[i].pos.y > screenHeight)){
            destroyBullet(i);
        }

        if(bullets[i].active == false){
            break;
        }
        for(int j = 0; j < MAX_ENEMIES; j++){
            if(ENEMIES[j].alive == false){
                break;
            }
            if(CheckCollisionCircles(ENEMIES[j].pos, 10, bullets[i].pos, 4)){
                ENEMIES[j].health -= player.damage;
                destroyBullet(i);
            }
        }
    }
}
void handleCoreHealthBar(){
    core.healthBar.rect.x = core.center.x - core.healthBar.rect.width / 2;
    core.healthBar.rect.y = core.center.y - core.healthBar.rect.height - 10;

    DrawRectangle(core.healthBar.rect.x, core.healthBar.rect.y,
                  core.healthBar.rect.width, core.healthBar.rect.height, core.healthBar.backgroundColor);
    DrawRectangle(core.healthBar.rect.x, core.healthBar.rect.y,
                  core.healthBar.rect.width * (core.health / core.maxHealth), core.healthBar.rect.height, core.healthBar.barColor);
}

void deathScreen(){
    DrawText("You died", screenWidth / 2 - 50, screenHeight / 2 - 50, 50, RED);
    DrawText("Press R to restart", screenWidth / 2 - 100, screenHeight / 2, 20, RED);
    if(IsKeyPressed(KEY_R)){
        resetGame();
    }
}

void resetGame(){
    gameState = MAIN_MENU;
    player.health = player.maxHealth;
    core.health = core.maxHealth;
    player.center = (Vector2){screenWidth / 2, screenHeight / 2};
    for(int i = 0; i < MAX_ENEMIES; i++){
        if(ENEMIES[i].alive == false){
            break;
        }
        ENEMIES[i] = (Enemy){(Vector2){0, 0}, 0, 0, 0, 0, (Color){0, 0, 0, 0}, false};
    }
    for(int i = 0; i < MAX_BULLETS; i++){
        if(bullets[i].active == false){
            break;
        }
        bullets[i] = (Bullet){0, 0, (Vector2){0, 0}, false};
    }
    enemyCount = 0;
    bulletCount = 0;
}

void savePlayerData(){
    FILE *file;
    file = fopen("resources/playerData.txt", "w");
    fprintf(file, "%d\n", player.coins);
    fprintf(file, "%f\n", player.damage);
    fprintf(file, "%f\n", player.maxHealth);
    fprintf(file, "%d\n", shotgunPurchased);
    fclose(file);
}
void loadPlayerData(){
    //read player coins from a file ending in .test
    int tempBool;
    FILE *file;
    if((file = fopen("resources/playerData.txt", "r"))!=NULL)
        {
            fscanf(file, "%d\n", &player.coins);
            fscanf(file, "%lf\n", &player.damage);
            fscanf(file, "%lf\n", &player.maxHealth);
            fscanf(file, "%d\n", &tempBool);
            fclose(file);
            shotgunPurchased = tempBool;
        }
    else
        {
            file = fopen("resources/playerData.txt", "w");
            fprintf(file, "%d\n", 0);
            fprintf(file, "%d\n", 1);
            fprintf(file, "%d\n", 100);
            fprintf(file, "%d\n", 0);
            player.coins = 10000;
            player.damage = 10;
            player.maxHealth = 100;
            shotgunPurchased = 0;
            fclose(file);
        }
}


void initStoreMenu(){
    upgradeDamageButton = initButton(200, screenHeight/2 - 190, 374, 130, 10, 110, "Upgrade Damage", WHITE, 40, upgradeDamage);
    upgradeHealthButton = initButton(200, screenHeight/2 - 50, 374, 130, 10, 110, "Upgrade Health", WHITE, 40, upgradeHealth);
    shotgunButton = initButton(1200, screenHeight/2 - 190, 200, 130, 10, 60, "Shotgun", WHITE, 40, purchaseShotgun);
}

void handleStore(){
    int upgradeDamageCostInt = (int)upgradeDamageCost;
    int upgradeHealthCostInt = (int)upgradeHealthCost;
    
    handleButton(&upgradeDamageButton);
    DrawText(TextFormat("Price: %d", upgradeDamageCostInt), upgradeDamageButton.x + upgradeDamageButton.width + 30, upgradeDamageButton.y + upgradeDamageButton.height / 2 - 10, 40, BLACK);
    
    handleButton(&upgradeHealthButton);
    DrawText(TextFormat("Price: %d", upgradeHealthCostInt), upgradeHealthButton.x + upgradeHealthButton.width + 30, upgradeHealthButton.y + upgradeHealthButton.height / 2 - 10, 40, BLACK);
    
    if(shotgunPurchased == false){
        handleButton(&shotgunButton);
        DrawText(TextFormat("Price: %d", shotgunCost), shotgunButton.x + shotgunButton.width + 30, shotgunButton.y + shotgunButton.height / 2 - 10, 40, BLACK);
    }
    if(IsKeyPressed(KEY_ESCAPE)){
        gameState = MAIN_MENU;
    }
}

void upgradeDamage(){
    if(player.coins >= upgradeDamageCost){
        player.coins -= upgradeDamageCost;
        player.damage += 1;
        upgradeDamageCost += 10;
        printf("Damage upgraded to %f\n", player.damage);
    }
    else{
        printf("Not enough coins\n");
    }
}

void upgradeHealth(){
    if(player.coins >= upgradeHealthCost){
        player.coins -= upgradeHealthCost;
        player.maxHealth += 10;
        player.health += 10;
        upgradeHealthCost += 0.5;
        printf("Health upgraded to %f\n", player.maxHealth);
    }
    else{
        printf("Not enough coins\n");
    }
}

void purchaseShotgun(){
    if(player.coins >= shotgunCost){
        player.coins -= shotgunCost;
        shotgunPurchased = true;
        printf("Shotgun purchased\n");
    }
    else{
        printf("Not enough coins\n");
    }
}

void translateTrig(Trig* trig, double x, double y){
    trig -> a.x += x;
    trig -> a.y += y;
    trig -> b.x += x;
    trig -> b.y += y;
    trig -> c.x += x;
    trig -> c.y += y;
}
void scaleDifficulty(double runtime){
    double base_difficulty = 1.0;
    double difficulty_increase_rate = 0.01;
    double max_difficulty = 10.0;
    
    difficulty = base_difficulty + difficulty_increase_rate * runtime;
    
    clamp(&difficulty, max_difficulty);
    printf("Difficulty: %f\n", difficulty);
    
}

void clamp(double* value, double max){
    if(*value > max){
        *value = max;
    }
}