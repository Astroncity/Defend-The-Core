#ifndef UTILS_H
#define UTILS_H


#include "../lib/raylib.h"
#include <math.h>

typedef struct Trig{
    Vector2 a;
    Vector2 b;
    Vector2 c;
}Trig;


void rotateVector2(Vector2* vector, float angle, Vector2 center);
double degToRad(double angle);
double radToDeg(double angle);
void translateTrig(Trig* trig, double x, double y);
double distance(Vector2 p1, Vector2 p2);
int roundToNearestMutiple(int num, int mul);
void clamp(double* value, double max);

#endif