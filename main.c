#include "gui.h"
#include "main.h"
#include "utils.h"
#include "raylib.h"
#include "snipertower.h"
#include "image.h"
#include "bullet.h"
#include "font.h"
#include "player.h"
#include "enemyBasic.h"
#include "enemySplitter.h"
#include "enemyShield.h"
#include "particleSystem.h"
#include "core.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#define PLATFORM_DESKTOP
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))






//! Fix snipertower targetting onto shield enemy
//! Fix shield enemy in general












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
Vector2 cameraPos = {0,0};  
Color backgroundColor;
char* savesFolderPath;

double runtime = 0;
Texture2D backgroundTexture;

Tower currentTower;
int sniperTowerCost = 100;
double upgradeDamageCost;
double upgradeHealthCost;
int shotgunCost = 500;
bool leftMouseDown = false;
Font propagandaFont;
Sound selectSnd;
Sound upgradeSnd;
Sound negativeSnd;
double saveDataTimer;
bool drawGhost = false;
bool firstTime = true;
bool f11Down = false;




ParticleSystem testSys;
Shader shader;
int main(void){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Defend The Core");
    SetWindowIcon(LoadImageFromMemory(".png", logoImage, sizeof(logoImage)));

    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    InitAudioDevice();
    SetMasterVolume(0.1f);
    SetTargetFPS(144);
    getAppDataPath();  
    printf("reached");
    printf("%s", savesFolderPath);
    char* fontpath = malloc(strlen(savesFolderPath) + strlen("\\propaganda.ttf") + 2);
    strcat(fontpath, savesFolderPath);
    strcat(fontpath, "\\propaganda.ttf");
    loadPlayerData();  
    printf("reached 2");
    shader = LoadShader(0, TextFormat("glow.frag", GLSL_VERSION));
    //shootSnd = LoadSound("sounds/fire.wav");
    //selectSnd = LoadSound("sounds/selectSnd.wav");
    //upgradeSnd = LoadSound("sounds/upgradeSnd.wav");
    //negativeSnd = LoadSound("sounds/negative.wav");       //! Still need to add Sounds to binary
    //shotgunSnd = LoadSound("sounds/shotgun.wav");
    //sniperSnd = LoadSound("sounds/sniper.wav");
    //propagandaFont = LoadFont(fontpath);
    printf("%s", fontpath);

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
    core.healthBar = (HealthBar){core.health, core.maxHealth, (Rectangle){screenWidth / 2 - 16, screenHeight / 2 - 800, 512, 48}, SKYBLUE, RED};
    upgradeDamageCost = player.damage * 10;
    upgradeHealthCost = player.maxHealth * 0.5;
    core.rect.y = roundToNearestMutiple(core.rect.y, 64); 

    loadTextures();

    initParticleSystem(&testSys, (Vector2){screenWidth / 2, screenHeight / 2}, (Vector2){200, 200}, (Vector2){2,2}, 1, 0.25, 10, ORANGE, (ParticleType){RECT});



    while (!WindowShouldClose()){
        mousePos = GetMousePosition();
        if(IsMouseButtonUp(MOUSE_LEFT_BUTTON)) leftMouseDown = false;
        BeginTextureMode(target);
        ClearBackground(backgroundColor);
        deltaTime = GetFrameTime();
        runtime += deltaTime;
        saveDataTimer += deltaTime;
        float scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);
        virtualMousePos.x = (mousePos.x - (GetScreenWidth() - (screenWidth*scale))*0.5f)/scale;
        virtualMousePos.y = (mousePos.y - (GetScreenHeight() - (screenHeight*scale))*0.5f)/scale;
        virtualMousePos = vector2Clamp(virtualMousePos, (Vector2){0, 0}, (Vector2){(float)screenWidth, (float)screenHeight});
        if(IsKeyPressed(KEY_F11) && !f11Down){
            ToggleFullscreen();
        }
        else{
            f11Down = false;
        }
        if(saveDataTimer >= 5){
            savePlayerData();
            saveDataTimer = 0;
        }

        switch(gameState){
            case MAIN_MENU:
                ButtonArgs args;
                handleButton(&playButton, virtualMousePos, args);
                handleButton(&storeButton, virtualMousePos, args);
                handleButton(&settingsButton, virtualMousePos, args);
                break;
            case PLAYING:
                DrawTexture(backgroundTexture, 0, 0, WHITE);
                handleBasicEnemies();
                handleSplitterEnemies();
                handleShieldEnemies();
                spawnBasicEnemies();
                spawnSplitterEnemies();
                spawnShieldEnemies();
                scaleDifficulty(runtime);
                handleSniperTowers();
                handleCore();
                handlePlayer();
                handleBullets();
                //handleParticleSystem(&testSys);
                handleUI();
                handleSniperTowerGUI();
                if(IsKeyPressed(KEY_ESCAPE)){
                    gameState = MAIN_MENU;
                    difficulty = 1;
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
        EndTextureMode();
        BeginDrawing();
            ClearBackground(BLACK);     // Clear screen background

            // Draw render texture to screen, properly scaled
            DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)screenWidth*scale))*0.5f, (GetScreenHeight() - ((float)screenHeight*scale))*0.5f,
                           (float)screenWidth*scale, (float)screenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();


    }
    
    cleanUp();
    cleanParticleSystem(&testSys);
    savePlayerData();

    return 0;
}

void initMainMenu(){
    Rectangle mainMenuButtonSrc = {0, 0, 110, 54};
    double midx = screenWidth/2 - 187;


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
    PlaySound(upgradeSnd);
}
void storeButtonFunc(){
    gameState = STORE;
    printf("Store button pressed\n");
}
void settingsButtonFunc(){
    gameState = SETTINGS;
    printf("Settings button pressed\n");
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
    for(int i = 0; i < enemyCountBasic; i++){
        basicEnemies[i] = (BasicEnemy){};
    }

    for(int i = 0; i < enemyCountSplitter; i++){
        splitterEnemies[i] = (SplitterEnemy){};
    }

    for(int i = 0; i < MAX_BULLETS; i++){
        bullets[i] = (Bullet){};
    }
    enemyCountBasic = 0;
    enemyCountSplitter = 0;
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
    fprintf(file, "%lld\n", player.coins);
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
            firstTime = false;
            fscanf(file, "%lld\n", &player.coins);
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
            memset(path,0,strlen(path));
            memset(resourcesName,0,strlen(resourcesName));
            strcpy(path, savesFolderPath);
            strcpy(resourcesName, "\\propaganda.ttf");    //not working rn
            strcat(path, resourcesName);
            
            file = fopen(path, "wb");
            fwrite(propagandaFontData, sizeof(unsigned char), sizeof(propagandaFontData), file);
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
    
    handleButton(&upgradeDamageButton, virtualMousePos, (ButtonArgs){0});
    DrawTextureEx(upgradeDamageButtonTexture, (Vector2){upgradeDamageButton.pos.x + 375, upgradeDamageButton.pos.y + 14}, 0, 2.5, WHITE);
    drawTextShadow(propagandaFont, TextFormat("%d", (int)player.damage), (Vector2){upgradeDamageButton.pos.x + 375, upgradeDamageButton.pos.y - 64}, 80, RED);
    DrawTextureEx(greenLabelShort, (Vector2){upgradeDamageButton.pos.x + 550, upgradeDamageButton.pos.y + 10}, 0, 4, WHITE);
    drawTextShadow(propagandaFont, TextFormat("%d", (int)upgradeDamageCost), (Vector2){upgradeDamageButton.pos.x + 600, upgradeDamageButton.pos.y + 42}, 80, BLACK);
    handleButton(&upgradeHealthButton, virtualMousePos, (ButtonArgs){0});
    handleButton(&sniperTowerButton, virtualMousePos, (ButtonArgs){0});
    

    if(shotgunPurchased == false){
        handleButton(&shotgunButton, virtualMousePos, (ButtonArgs){SNIPER});
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
        PlaySound(upgradeSnd);
    }
    else{
        printf("Not enough coins\n");
        PlaySound(negativeSnd);
    }
}

void upgradeHealth(){
    if(player.coins >= upgradeHealthCost){
        player.coins -= upgradeHealthCost;
        player.maxHealth += 10;
        player.health += 10;
        upgradeHealthCost += 0.5;
        printf("Health upgraded to %f\n", player.maxHealth);
        PlaySound(upgradeSnd);
    }
    else{
        printf("Not enough coins\n");
        PlaySound(negativeSnd);
    }
}

void purchaseShotgun(){
    if(player.coins >= shotgunCost){
        player.coins -= shotgunCost;
        shotgunPurchased = true;
        PlaySound(upgradeSnd);
        printf("Shotgun purchased\n");
        PlaySound(upgradeSnd);
    }
    else{
        printf("Not enough coins\n");
        PlaySound(negativeSnd);
    }
}

void scaleDifficulty(double runtime){
    double base_difficulty = 1.0;
    double difficulty_increase_rate = 0.01;
    double max_difficulty = 10.0;
    
    difficulty = base_difficulty + difficulty_increase_rate * runtime;
    
    clamp(&difficulty, max_difficulty);
    
}

void placeTower(){
    Vector2 roundedMousePos = (Vector2){roundToNearestMutiple((int)virtualMousePos.x - 32, 64), roundToNearestMutiple((int)virtualMousePos.y - 32, 64)};
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
                    PlaySound(upgradeSnd);
                    if(player.coins < sniperTowerCost){
                        gameState = STORE;
                    }
                    break;
                }
            }
            else{
                printf("Not enough coins\n");
                PlaySound(negativeSnd);
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

void purchaseSniperTower(){
    if (player.coins >= sniperTowerCost){
        leftMouseDown = true;
        currentTower = SNIPER;
        gameState = PLACING_TOWER;

    }
    else{
        printf("Not enough coins\n");
        PlaySound(negativeSnd);
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
    UnloadTexture(splitterEnemyTexture);
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

    char* defendTheCorePath = (char*)calloc(pathLengthM, sizeof(char));
    char* resourcesPath = (char*)calloc(pathLength, sizeof(char));

    snprintf(defendTheCorePath, pathLengthM, "%s\\%s", appdata, folderNameM);
    snprintf(resourcesPath, pathLength, "%s\\%s\\%s", appdata, folderNameM, folderName);

    if (folderExists(defendTheCorePath)){
        if(folderExists(resourcesPath)){
            printf("Folder already exists: %s\n", resourcesPath);
            savesFolderPath = resourcesPath;
            //free(folderPath);
            return 0;
        }
        else{
            int result = mkdir(resourcesPath);

            if(result == 0){
                printf("Folder created/accessed successfully: %s\n", resourcesPath);
            } 
            else{
                printf("Error: Failed to create/access folder: %s\n", resourcesPath);
            }
        }
    }
    else{
        int result = mkdir(defendTheCorePath);

        if(result == 0){
            printf("Folder created/accessed successfully: %s\n", defendTheCorePath);
        } 
        else{
            printf("Error: Failed to create/access folder: %s\n", defendTheCorePath);
        }
        result = mkdir(resourcesPath);

        if(result == 0){
            printf("Folder created/accessed successfully: %s\n", resourcesPath);
        } 
        else{
            printf("Error: Failed to create/access folder: %s\n", resourcesPath);
        }
    }

    savesFolderPath = resourcesPath;
    //free(folderPath);
    free(defendTheCorePath);
    return 0;
}













/*void loadSounds(){
    FILE *file;
    char* resPath = savesFolderPath;
    char* soundsFolderPath = strcat(resPath, "\\sounds");
    mkdir(soundsFolderPath);

}*/

void loadTextures(){
    bulletTexture = LoadTextureFromImage(LoadImageFromMemory(".png", bulletImage, sizeof(bulletImage)));
    backgroundTexture = LoadTextureFromImage(LoadImageFromMemory(".png", BackgroundImage, sizeof(BackgroundImage)));
    greenLabel = LoadTextureFromImage(LoadImageFromMemory(".png", greenLabelImage, sizeof(greenLabelImage)));
    greenLabelShort = LoadTextureFromImage(LoadImageFromMemory(".png", greenLabelShortImage, sizeof(greenLabelShortImage)));
    greyLabel = LoadTextureFromImage(LoadImageFromMemory(".png", greyLabelImage, sizeof(greyLabelImage)));
    coin = LoadTextureFromImage(LoadImageFromMemory(".png", coinImage, sizeof(coinImage)));
    coreTexture = LoadTextureFromImage(LoadImageFromMemory(".png", coreImage, sizeof(coreImage)));
    sniperTowerBaseTexture = LoadTextureFromImage(LoadImageFromMemory(".png", sniperTowerBaseImage, sizeof(sniperTowerBaseImage)));
    sniperTowerTurretTexture = LoadTextureFromImage(LoadImageFromMemory(".png", sniperTowerTurretImage, sizeof(sniperTowerTurretImage)));
    splitterEnemyTexture = LoadTextureFromImage(LoadImageFromMemory(".png", enemySplitterImage, sizeof(enemySplitterImage)));
    shieldEnemyTexture = LoadTextureFromImage(LoadImageFromMemory(".png", enemyShieldImage, sizeof(enemyShieldImage))); 
    shieldEnemyShieldTexture = LoadTextureFromImage(LoadImageFromMemory(".png", enemyShieldShieldImage, sizeof(enemyShieldShieldImage)));
}