#include "raylib.h"
#include "utils.h"

typedef enum GameState{
        MAIN_MENU,
        PLAYING,
        STORE,
        SETTINGS,
        PAUSED,
        DEAD,
        PLACING_TOWER
}GameState;

typedef struct SniperTower{
    double damage;
    double range;
    double direction;
    double fireRate;
    Rectangle rect;
    bool active;
    int index;
    int level;
    double lastAngle;
    double timer;
    double shortestDist;
}SniperTower;


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
    double direction;
}Enemy;

typedef struct Bullet{
    double speed;
    double direction;
    Vector2 pos;
    bool active;
    int index;
    int size;
    int damage;
    Color color;    
}Bullet;



void initMainMenu();
void storeButtonFunc();
void playButtonFunc();
void settingsButtonFunc();
void handlePlayer();
void handleCore();
double coreBonusTimer;
void handleEnemies();
int lastUsedEnemySlot();
void spawnEnemies();
double spawnTimer;
void destroyEnemy(int enemyIndex);
int lastUsedBulletSlot();
void handleBullets();
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
void scaleDifficulty(double runtime);
void spawnSniperTower();
void placeTower();
void handleSniperTowers();
void purchaseSniperTower();
void handleSniperTowers();
void handleUI();
void addCommas(int number, char* resultStr);
void handleSniperTowerGUI();
void cleanUp(); 
int folderExists(const char* folderPath);
int getAppDataPath();
