#include "raylib.h"

typedef enum Tower{
    NONE,
    SNIPER
}Tower;

typedef struct SniperTower{
    double damage;
    double attackSpeed;
    double range;
    double direction;
    Rectangle rect;
    bool active;
    int index;
}SniperTower;

typedef enum GameState{
        MAIN_MENU,
        PLAYING,
        STORE,
        SETTINGS,
        PAUSED,
        DEAD,
        PLACING_TOWER
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



void drawMainMenu();
Button initButton(int x, int y, int width, int height, int borderThickness, 
                  int textOffset, const char* text, Color color, int fontSize, void (*func)());
void handleButton(Button* button, Tower tower);
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
void spawnSniperTower();
void placeTower();
void handleSniperTowers();
void purchaseSniperTower();
inline double distance(Vector2 p1, Vector2 p2);
inline int roundToNearestMutiple(int num, int mul);