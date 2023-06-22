#include "raylib.h"
#include "utils.h"





void initMainMenu();
void storeButtonFunc();
void playButtonFunc();
void settingsButtonFunc();
void deathScreen();
void resetGame();
void savePlayerData();
void loadPlayerData();
void upgradeDamage();
void upgradeHealth();
void handleStore();
void initStoreMenu();
void purchaseShotgun();
void scaleDifficulty(double runtime);
void placeTower();
void purchaseSniperTower();
void handleUI();
void addCommas(int number, char* resultStr);
void handleSniperTowerGUI();
void cleanUp(); 
int folderExists(const char* folderPath);
int getAppDataPath();
