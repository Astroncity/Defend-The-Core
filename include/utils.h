#ifndef UTILS_H
#define UTILS_H


#include "raylib.h"
#include <math.h>
#define screenWidth 1920
#define screenHeight 1080

typedef struct Trig{
    Vector2 a;
    Vector2 b;
    Vector2 c;
}Trig;

typedef enum GameState{
        MAIN_MENU,
        PLAYING,
        STORE,
        SETTINGS,
        PAUSED,
        DEAD,
        PLACING_TOWER
}GameState;

extern GameState gameState;
extern Vector2 mousePos;
extern double deltaTime;
extern double difficulty;

void rotateVector2(Vector2* vector, float angle, Vector2 center);
double degToRad(double angle);
double radToDeg(double angle);
void translateTrig(Trig* trig, double x, double y);
double distance(Vector2 p1, Vector2 p2);
int roundToNearestMutiple(int num, int mul);
void clamp(double* value, double max);
void drawGrid();

#endif /*UTILS_H*/