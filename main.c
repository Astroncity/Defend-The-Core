#include "gui.h"
#include "main.h"
#include "utils.h"
#include "raylib.h"
#include "image.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#define screenWidth 1920
#define screenHeight 1080
#define MAX_ENEMIES 1000
#define MAX_BULLETS 50000
#define PELLET_COUNT 5
#define SHOTGUN_PELLET_SPREAD 4
#define CORESCALE 8




/**----------------------------------------------
 * todo                  TODO
 * todo make healthbar system--DONE
 * todo improve spawning--DONE
 * todo add audio
 * todo make a store--DONE
 * todo make a settings menu
 * todo make a pause menu
 * todo make a game over screen--DONE
 * todo find way to save settings/data--DONE
 *---------------------------------------------**/







// UI things
Button playButton;
Texture2D playButtonTexture;
Button storeButton;
Texture2D storeButtonTexture;
Button settingsButton;
Texture2D settingsButtonTexture;
Button upgradeHealthButton;
Texture2D upgradeHealthButtonTexture;
Button upgradeDamageButton;
Texture2D upgradeDamageButtonTexture;
Button shotgunButton;
Texture2D shotgunButtonTexture;
Button sniperTowerButton;
Texture2D sniperTowerButtonTexture;

Texture2D greenLabel;
Texture2D greenLabelShort;
Texture2D greyLabel;
Texture2D coin;


bool darkMode = false;
Vector2 mousePos;
Vector2 cameraPos = {0,0};  
Color backgroundColor;
char* savesFolderPath;

GameState gameState = MAIN_MENU;
double deltaTime = 0;
double runtime = 0;
double difficulty = 1;
Texture2D backgroundTexture;

Player player = {100, 100, 2, 10, 1, 90 * PI / 180, 0, (Trig)
                {(Vector2){screenWidth / 2 - 16, screenHeight / 2}, (Vector2){screenWidth / 2, screenHeight / 2 + 32}, (Vector2){screenWidth / 2 + 16, screenHeight / 2}}};

Core core = {500, 500, (Rectangle){screenWidth / 2 - (16 * CORESCALE), screenHeight / 2 - (16 * CORESCALE), 32 * CORESCALE, 32 * CORESCALE}, BLUE};
Texture2D coreTexture;
Enemy ENEMIES[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
Texture2D bulletTexture;
Rectangle bulletTextureSource = {0, 0, 16, 16};
SniperTower sniperTowers[520];
double defaultSniperTowerRange = 500;
Texture2D sniperTowerBaseTexture;
Texture2D sniperTowerTurretTexture;
Tower currentTower;
int sniperTowerCost = 100;
int sniperTowerCount = 0;
int enemyCount = 0;
int bulletCount = 0;
double upgradeDamageCost;
double upgradeHealthCost;
int shotgunCost = 500;
bool shotgunPurchased = false;
bool leftMouseDown = false;
bool useGamepad;
Font propagandaFont;
Sound shootSnd;
Sound selectSnd;
Sound upgradeSnd;
Sound negativeSnd;
Sound shotgunSnd;
Sound sniperSnd;

bool drawGhost = false;

int main(void){
    InitWindow(screenWidth, screenHeight, "Defend The Core");
    SetWindowIcon(LoadImageFromMemory(".png", logoImage, sizeof(logoImage)));
    InitAudioDevice();
    SetMasterVolume(0.1f);
    SetTargetFPS(144);
    getAppDataPath();  
    //propagandaFont = LoadFont("resources/propaganda.ttf");
    printf("reached");
    printf("%s", savesFolderPath);
    loadPlayerData();  
    printf("reached 2");
    //shootSnd = LoadSound("sounds/fire.wav");
    //selectSnd = LoadSound("sounds/selectSnd.wav");
    //upgradeSnd = LoadSound("sounds/upgradeSnd.wav");
    //negativeSnd = LoadSound("sounds/negative.wav");
    //shotgunSnd = LoadSound("sounds/shotgun.wav");
    //sniperSnd = LoadSound("sounds/sniper.wav");

    initMainMenu();
    initStoreMenu();
    SetExitKey(KEY_F12);
    if(darkMode){
        backgroundColor = BLACK;
    }
    else{
        backgroundColor = WHITE;
    }
    useGamepad = false;

    player.healthBar = (HealthBar){player.health, player.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 + 64, 128, 4}, GREEN, RED};
    player.hitbox = (Rectangle){0, 0, 28, 28};
    core.healthBar = (HealthBar){core.health, core.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 - 800, 512, 48}, SKYBLUE, RED};
    upgradeDamageCost = player.damage * 10;
    upgradeHealthCost = player.maxHealth * 0.5;
    core.rect.y = roundToNearestMutiple(core.rect.y, 64); 


    bulletTexture = LoadTextureFromImage(LoadImageFromMemory(".png", bulletImage, sizeof(bulletImage)));
    backgroundTexture = LoadTextureFromImage(LoadImageFromMemory(".png", BackgroundImage, sizeof(BackgroundImage)));
    greenLabel = LoadTextureFromImage(LoadImageFromMemory(".png", greenLabelImage, sizeof(greenLabelImage)));
    greenLabelShort = LoadTextureFromImage(LoadImageFromMemory(".png", greenLabelShortImage, sizeof(greenLabelShortImage)));
    greyLabel = LoadTextureFromImage(LoadImageFromMemory(".png", greyLabelImage, sizeof(greyLabelImage)));
    coin = LoadTextureFromImage(LoadImageFromMemory(".png", coinImage, sizeof(coinImage)));
    coreTexture = LoadTextureFromImage(LoadImageFromMemory(".png", coreImage, sizeof(coreImage)));
    sniperTowerBaseTexture = LoadTextureFromImage(LoadImageFromMemory(".png", sniperTowerBaseImage, sizeof(sniperTowerBaseImage)));
    sniperTowerTurretTexture = LoadTextureFromImage(LoadImageFromMemory(".png", sniperTowerTurretImage, sizeof(sniperTowerTurretImage)));

    while (!WindowShouldClose()){
        mousePos = GetMousePosition();
        if(IsMouseButtonUp(MOUSE_LEFT_BUTTON)) leftMouseDown = false;
        BeginDrawing();
        ClearBackground(backgroundColor);
        deltaTime = GetFrameTime();
        runtime += deltaTime;

        switch(gameState){
            case MAIN_MENU:
                ButtonArgs args;
                handleButton(&playButton, mousePos, args);
                handleButton(&storeButton, mousePos, args);
                handleButton(&settingsButton, mousePos, args);
                break;
            case PLAYING:
                DrawTexture(backgroundTexture, 0, 0, WHITE);
                handleCore();
                handlePlayer();
                handleEnemies();
                spawnEnemies();
                handleBullets();
                scaleDifficulty(runtime);
                handleSniperTowers();
                handleUI();
                handleSniperTowerGUI();
                if(IsKeyPressed(KEY_ESCAPE)){
                    gameState = STORE;
                }
                break;
            case STORE:
                handleStore();
                handleUI();
                break;
            case SETTINGS:
                break;
            case PAUSED:
                break;
            case DEAD:
                deathScreen();
                break;
            case PLACING_TOWER:
                handleSniperTowers();
                placeTower();
                handleUI();
                break;
        }
        DrawFPS(10, 50);
        drawTextShadow(propagandaFont, TextFormat("Difficulty %.3g", round(difficulty * 100) / 100), (Vector2){(screenWidth / 2) - 300, 20}, 70, WHITE);
        // draw lines 64x64 apart
        EndDrawing();

    }
    
    cleanUp();
    savePlayerData();

    return 0;
}

void initMainMenu(){
    Rectangle mainMenuButtonSrc = {0, 0, 110, 54};
    double midx = screenWidth/2 - 187;


    //playButtonTexture = LoadTexture("images/playButton.png");
    playButtonTexture = LoadTextureFromImage(LoadImageFromMemory(".png", playButtonImage, sizeof(playButtonImage)));
    storeButtonTexture = LoadTextureFromImage(LoadImageFromMemory(".png", storeButtonImage, sizeof(storeButtonImage)));
    settingsButtonTexture = LoadTextureFromImage(LoadImageFromMemory(".png", settingsButtonImage, sizeof(settingsButtonImage)));

    
    playButton = initButton((Vector2){midx, screenHeight/2 - 190}, mainMenuButtonSrc, playButtonTexture, (Vector2){0, 0}, playButtonFunc);
    storeButton = initButton((Vector2){midx, screenHeight/2}, mainMenuButtonSrc, storeButtonTexture, (Vector2){0, 0}, storeButtonFunc);
    settingsButton = initButton((Vector2){midx, screenHeight/2 + 190}, mainMenuButtonSrc, settingsButtonTexture, (Vector2){0, 0}, settingsButtonFunc);
}


void playButtonFunc(){
    gameState = PLAYING;
    printf("Play button pressed\n");
    //PlaySound(upgradeSnd);
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
    double gamepadAngle;

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

    // GamePad
    if(IsGamepadAvailable(0) && useGamepad){
        double leftStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
        double rightStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
        if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1)){player.speed = 600;}
        else{player.speed = 300;}
        translateTrig(&player.trig, -cos(player.direction) * player.speed * deltaTime * leftStickY, -sin(player.direction) * player.speed * deltaTime * leftStickY);
        gamepadAngle = player.direction + (rightStickX / 25); // broken
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
    if(!useGamepad){
        player.direction = angleBetweenMouse;
    }
    else{
        angDiff = gamepadAngle - player.direction;
        player.direction = gamepadAngle;
    }
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

void shootBasic(Vector2 center, double angleBetweenMouse){
    bool canShoot = true;

    if((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && canShoot) || ((useGamepad && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2)) && canShoot)){
        canShoot = false;
        Bullet bullet;
        bullet.pos.x = center.x;
        bullet.pos.y = center.y;
        bullet.direction = (useGamepad) ? player.direction : angleBetweenMouse;
        bullet.speed = 750;
        bullet.size = 4;
        bullet.active = true;
        bullet.color = GREEN;
        bullet.damage = player.damage;
        bullet.index = bulletCount;
        bullets[bulletCount] = bullet;
        bulletCount++;
        //PlaySound(shootSnd);
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || (useGamepad && IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))){
        canShoot = true;
    }
    //print gamepad state
    //print("Gamepad: %d", IsGamepadButtonReleased);
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
            bullet.color = GREEN;
            bullet.damage = player.damage;
            bullet.size = 4;
            bullets[bulletCount] = bullet;
            bulletCount++;
            //PlaySound(shotgunSnd);
        }
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
        canShoot = true;
    }
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

void destroyBullet(int bulletIndex){
    int lastUsedSlot = bulletCount - 1;
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


        //DrawCircle(bullets[i].pos.x, bullets[i].pos.y, bullets[i].size, bullets[i].color);
        DrawTextureEx(bulletTexture, bullets[i].pos, 0, 1, WHITE);
        // destory bullet if outside screen
        if((bullets[i].pos.x > screenWidth || bullets[i].pos.x < 0) || (bullets[i].pos.y < 0 || bullets[i].pos.y > screenHeight)){
            destroyBullet(i);
        }

        for(int j = 0; j < MAX_ENEMIES; j++){
            if(ENEMIES[j].alive == false){
                break;
            }
            if(CheckCollisionCircles(ENEMIES[j].pos, 10, bullets[i].pos, bullets[i].size)){
                ENEMIES[j].health -= bullets[i].damage;
                destroyBullet(i);
            }
        }
    }
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
    difficulty = 1;
}

void savePlayerData(){
    FILE *file;
    char* path = malloc(sizeof(char) * 260);
    char* resourcesName = malloc(sizeof(char) * 50);
    strcpy(path, savesFolderPath);
    strcpy(resourcesName, "\\playerData.txt");
    file = fopen(strcat(path, resourcesName), "w");
    fprintf(file, "%d\n", player.coins);
    fprintf(file, "%f\n", player.damage);
    fprintf(file, "%f\n", player.maxHealth);
    fprintf(file, "%d\n", shotgunPurchased);

    fprintf(file, "%d\n", sniperTowerCount);
    for(int i = 0; i < sniperTowerCount; i++){
        fprintf(file, "%f\n", sniperTowers[i].rect.x);
        fprintf(file, "%f\n", sniperTowers[i].rect.y);
        fprintf(file, "%d\n", sniperTowers[i].level);
    }

    fclose(file);
    free(path);
    free(resourcesName);
}
void loadPlayerData(){
    //read player coins from a file ending in .test
    int tempBool;
    int tmpLevel;
    FILE *file;
    char* path = malloc(sizeof(char) * 260);
    char* resourcesName = malloc(sizeof(char) * 50);
    strcpy(path, savesFolderPath);
    strcpy(resourcesName, "\\playerData.txt");
    strcat(path, resourcesName);
    if((file = fopen(path, "r")) != NULL)
        {
            fscanf(file, "%d\n", &player.coins);
            fscanf(file, "%lf\n", &player.damage);
            fscanf(file, "%lf\n", &player.maxHealth);
            fscanf(file, "%d\n", &tempBool);

            fscanf(file, "%d\n", &sniperTowerCount);
            for(int i = 0; i < sniperTowerCount; i++){
                fscanf(file, "%f\n", &sniperTowers[i].rect.x);
                fscanf(file, "%f\n", &sniperTowers[i].rect.y);
                fscanf(file, "%d\n", &tmpLevel);
                sniperTowers[i].rect.width = 64;
                sniperTowers[i].rect.height = 64;
                sniperTowers[i].level = tmpLevel;
                sniperTowers[i].damage = 10 * tmpLevel;
                sniperTowers[i].fireRate = 1 - ((tmpLevel - 1) *0.05);
                sniperTowers[i].range = defaultSniperTowerRange + ((tmpLevel - 1) * 50);

            }


            fclose(file);
            shotgunPurchased = tempBool;
            free(path);
            free(resourcesName);
        }
    else
        {
            file = fopen(path, "w");
            fprintf(file, "%d\n", 0);
            fprintf(file, "%d\n", 1);
            fprintf(file, "%d\n", 100);
            fprintf(file, "%d\n", 0);
            player.coins = 0;
            player.damage = 10;
            player.maxHealth = 100;
            shotgunPurchased = 0;
            sniperTowerCount = 0;
            fclose(file);
            free(path);
            free(resourcesName);
        }
}


void initStoreMenu(){
    Texture2D tempTexture = LoadTextureFromImage(LoadImageFromMemory(".png", upgradeButtonImage, sizeof(upgradeButtonImage)));
    upgradeDamageButtonTexture = LoadTextureFromImage(LoadImageFromMemory(".png", upgradeDamageButtonImage, sizeof(upgradeDamageButtonImage)));
    Rectangle tempTextureSrc = {0, 0, 110, 54};

    upgradeDamageButton = initButton((Vector2){200, screenHeight/2 - 190}, tempTextureSrc, tempTexture, (Vector2){0, 0}, upgradeDamage);
    upgradeHealthButton = initButton((Vector2){200, screenHeight/2 - 20}, tempTextureSrc, tempTexture, (Vector2){0, 0}, upgradeHealth);
    shotgunButton = initButton((Vector2){1200, screenHeight/2 - 190}, tempTextureSrc, tempTexture, (Vector2){0, 0}, purchaseShotgun);
    sniperTowerButton = initButton((Vector2){1100, screenHeight/2}, tempTextureSrc, tempTexture, (Vector2){0, 0}, purchaseSniperTower);
}

void handleStore(){
    
    handleButton(&upgradeDamageButton, mousePos, (ButtonArgs){0});
    DrawTextureEx(upgradeDamageButtonTexture, (Vector2){upgradeDamageButton.pos.x + 375, upgradeDamageButton.pos.y + 14}, 0, 2.5, WHITE);
    drawTextShadow(propagandaFont, TextFormat("%d", (int)player.damage), (Vector2){upgradeDamageButton.pos.x + 400, upgradeDamageButton.pos.y - 64}, 80, RED);
    DrawTextureEx(greenLabelShort, (Vector2){upgradeDamageButton.pos.x + 550, upgradeDamageButton.pos.y + 10}, 0, 4, WHITE);
    drawTextShadow(propagandaFont, TextFormat("%d", (int)upgradeDamageCost), (Vector2){upgradeDamageButton.pos.x + 600, upgradeDamageButton.pos.y + 42}, 80, BLACK);
    handleButton(&upgradeHealthButton, mousePos, (ButtonArgs){0});
    handleButton(&sniperTowerButton, mousePos, (ButtonArgs){0});
    

    if(shotgunPurchased == false){
        handleButton(&shotgunButton, mousePos, (ButtonArgs){SNIPER});
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
        //PlaySound(upgradeSnd);
    }
    else{
        printf("Not enough coins\n");
        //PlaySound(negativeSnd);
    }
}

void upgradeHealth(){
    if(player.coins >= upgradeHealthCost){
        player.coins -= upgradeHealthCost;
        player.maxHealth += 10;
        player.health += 10;
        upgradeHealthCost += 0.5;
        printf("Health upgraded to %f\n", player.maxHealth);
        //PlaySound(upgradeSnd);
    }
    else{
        printf("Not enough coins\n");
        //PlaySound(negativeSnd);
    }
}

void purchaseShotgun(){
    if(player.coins >= shotgunCost){
        player.coins -= shotgunCost;
        shotgunPurchased = true;
        //PlaySound(upgradeSnd);
        printf("Shotgun purchased\n");
        //PlaySound(upgradeSnd);
    }
    else{
        printf("Not enough coins\n");
        //PlaySound(negativeSnd);
    }
}

void scaleDifficulty(double runtime){
    double base_difficulty = 1.0;
    double difficulty_increase_rate = 0.01;
    double max_difficulty = 10.0;
    
    difficulty = base_difficulty + difficulty_increase_rate * runtime;
    
    clamp(&difficulty, max_difficulty);
    
}


void spawnSniperTower(){
    SniperTower tower;
    tower.rect.height = 64;
    tower.rect.width = 64;

    int towerPosX = mousePos.x - (tower.rect.width / 2);
    int towerPosY = mousePos.y - (tower.rect.height / 2);

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

void placeTower(){
    Vector2 roundedMousePos = (Vector2){roundToNearestMutiple((int)mousePos.x - 32, 64), roundToNearestMutiple((int)mousePos.y - 32, 64)};
    bool canPlace = true;
    if(IsKeyPressed(KEY_ESCAPE)){
        gameState = MAIN_MENU;
    }
    drawGrid(screenWidth, screenHeight);
    switch(currentTower){
        case SNIPER:
            if(player.coins >= sniperTowerCost){
                DrawTextureEx(coreTexture, (Vector2){core.rect.x, core.rect.y}, 0, CORESCALE, WHITE);
                DrawTextureEx(sniperTowerBaseTexture, roundedMousePos, 0, 2, (Color){255, 255, 255, 100});
                DrawCircle(roundedMousePos.x + 32, roundedMousePos.y + 32, defaultSniperTowerRange, (Color){0, 255, 0, 50});
        
                DrawTexturePro(sniperTowerTurretTexture, (Rectangle){0, 0, 44, 64}, (Rectangle){roundedMousePos.x + 32, roundedMousePos.y + 32, 44, 64},
                              (Vector2){21.5, 42.5}, 0, (Color){255, 255, 255, 100});


                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && leftMouseDown == false){
                    for(int i = 0; i < sniperTowerCount; i++){
                        if(roundedMousePos.x == sniperTowers[i].rect.x && roundedMousePos.y == sniperTowers[i].rect.y){
                            printf("Tower already placed here\n");
                            canPlace = false;
                        }
                    }
                    if(CheckCollisionRecs((Rectangle){roundedMousePos.x, roundedMousePos.y, 64, 64}, core.rect)){
                        printf("Tower too close to core\n");
                        canPlace = false;
                    }
                    if(canPlace == false){
                        break;
                    }
                    player.coins -= sniperTowerCost;
                    spawnSniperTower();
                    printf("Sniper tower placed\n");
                    //PlaySound(upgradeSnd);
                    if(player.coins < sniperTowerCost){
                        gameState = STORE;
                    }
                    break;
                }
            }
            else{
                printf("Not enough coins\n");
                //PlaySound(negativeSnd);
                gameState = STORE;
                drawGhost = false;
            }
            break;
        case NONE:
            printf("No tower selected\n");
            gameState = STORE;
            drawGhost = false;
            break;
    }
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
        sniperTowers[i].shortestDist = 1000000;
        Enemy closestEnemy;
        for(int j = 0; j < MAX_ENEMIES; j++){
            if(ENEMIES[j].alive == false){
                break;
            }
            double distanceToEnemy = distance((Vector2){sniperTowers[i].rect.x + sniperTowers[i].rect.width / 2, sniperTowers[i].rect.y + sniperTowers[i].rect.height / 2},
                                              (Vector2){ENEMIES[j].pos.x, ENEMIES[j].pos.y});
            if(distanceToEnemy < sniperTowers[i].shortestDist){
                sniperTowers[i].shortestDist = distanceToEnemy;
                closestEnemy = ENEMIES[j];
            }
        }
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
            //PlaySound(sniperSnd);
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


void purchaseSniperTower(){
    if (player.coins >= sniperTowerCost){
        leftMouseDown = true;
        currentTower = SNIPER;
        gameState = PLACING_TOWER;

    }
    else{
        printf("Not enough coins\n");
        //PlaySound(negativeSnd);
    }
}


void handleUI(){
    DrawTextureEx(greyLabel, (Vector2){0, 0}, 0, 2.25, WHITE);
    DrawTextureEx(coin, (Vector2){10, 14}, 0, 3, (Color){100, 100, 100, 100});
    DrawTextureEx(coin, (Vector2){10, 10}, 0, 3, WHITE);
    char playerCoinsStr;
    addCommas(player.coins, &playerCoinsStr);

    drawTextShadow(propagandaFont, (const char*)&playerCoinsStr, (Vector2){70, 15}, 45, (Color){255, 215, 0, 255});
}

void addCommas(int number, char* resultStr){
    char numberStr[20];
    sprintf(numberStr, "%d", number);
    int length = strlen(numberStr);
    int commas = (length - 1) / 3;

    int resultIndex = 0;

    int remainingDigits = length % 3;
    if (remainingDigits == 0)
        remainingDigits = 3;

    for (int i = 0; i < length; i++) {
        resultStr[resultIndex] = numberStr[i];
        resultIndex++;

        remainingDigits--;
        if (remainingDigits == 0 && commas > 0) {
            resultStr[resultIndex] = ',';
            resultIndex++;
            commas--;
            remainingDigits = 3;
        }
    }

    resultStr[resultIndex] = '\0';
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
        // draw a green rectangle above the tower
        DrawRectangle(closestTower.rect.x, closestTower.rect.y - 50, 50, 50, (Color){0, 255, 0, 255});
    }
}


void cleanUp(){
    UnloadTexture(greyLabel);
    UnloadTexture(coin);
    UnloadTexture(sniperTowerBaseTexture);
    UnloadTexture(sniperTowerTurretTexture);
    UnloadTexture(upgradeDamageButtonTexture);
    UnloadTexture(upgradeHealthButtonTexture);
    UnloadTexture(coreTexture);
    UnloadTexture(bulletTexture);
    UnloadTexture(backgroundTexture);
    UnloadSound(negativeSnd);
    UnloadSound(shotgunSnd);
    UnloadSound(selectSnd);
    UnloadSound(upgradeSnd);
    UnloadSound(shootSnd);
    UnloadSound(sniperSnd);
    CloseAudioDevice();
    CloseWindow();
}

int folderExists(const char* folderPath) {
    struct stat info;
    return stat(folderPath, &info) == 0 && S_ISDIR(info.st_mode);
}


int getAppDataPath(){
    const char* appdata = getenv("LOCALAPPDATA");
    if(appdata == NULL){
        printf("Error: Unable to access the appdata folder.\n");
        return 1;
    }

    // Create the folder name
    const char* folderNameM = "DefendTheCore";
    const char* folderName = "resourcess";

    size_t pathLengthM = strlen(appdata) + strlen(folderNameM) + 2; // +2 for '\' and null terminator
    size_t pathLength = strlen(appdata) + strlen(folderNameM) + strlen(folderName) + 2; // +2 for '\' and null terminator

    char* folderPathM = (char*)calloc(pathLengthM, sizeof(char));
    char* folderPath = (char*)calloc(pathLength, sizeof(char));

    snprintf(folderPathM, pathLengthM, "%s\\%s", appdata, folderNameM);
    snprintf(folderPath, pathLength, "%s\\%s\\%s", appdata, folderNameM, folderName);

    if (folderExists(folderPathM)){
        if(folderExists(folderPath)){
            printf("Folder already exists: %s\n", folderPath);
            savesFolderPath = folderPath;
            //free(folderPath);
            return 0;
        }
        else{
            int result = mkdir(folderPath);

            if(result == 0){
                printf("Folder created/accessed successfully: %s\n", folderPath);
            } 
            else{
                printf("Error: Failed to create/access folder: %s\n", folderPath);
            }
        }
    }
    else{
        int result = mkdir(folderPathM);

        if(result == 0){
            printf("Folder created/accessed successfully: %s\n", folderPathM);
        } 
        else{
            printf("Error: Failed to create/access folder: %s\n", folderPathM);
        }
        result = mkdir(folderPath);

        if(result == 0){
            printf("Folder created/accessed successfully: %s\n", folderPath);
        } 
        else{
            printf("Error: Failed to create/access folder: %s\n", folderPath);
        }
    }

    savesFolderPath = folderPath;
    free(folderPath);
    free(folderPathM);
    return 0;
}
