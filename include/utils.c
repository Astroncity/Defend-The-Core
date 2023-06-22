#include "utils.h"


void rotateVector2(Vector2* vector, float angle, Vector2 center){
    float s = sin(angle);
    float c = cos(angle);

    vector -> x -= center.x;
    vector -> y -= center.y;

    // rotate point
    float xnew = vector -> x * c - vector -> y * s;
    float ynew = vector -> x * s + vector -> y * c;

    vector -> x = xnew + center.x;
    vector -> y = ynew + center.y;
}

double degToRad(double angle){
    return angle * PI / 180;
}
double radToDeg(double angle){
    return angle * 180 / PI;
}

void translateTrig(Trig* trig, double x, double y){
    trig -> a.x += x;
    trig -> a.y += y;
    trig -> b.x += x;
    trig -> b.y += y;
    trig -> c.x += x;
    trig -> c.y += y;
}

void clamp(double* value, double max){
    if(*value > max){
        *value = max;
    }
}


double distance(Vector2 p1, Vector2 p2){
    double x = abs(p1.x - p2.x);
    double y = abs(p1.y - p2.y);
    double distance = sqrt(x*x + y*y);
    return distance;
}

int roundToNearestMutiple(int num, int mul){
    int remainder = num % mul;
    if(remainder >= mul / 2){
        return num + (mul - remainder);
    }
    else{
        return num - remainder;
    }
}

void drawGrid(int screenWidth, int screenHeight){
    for(int i = 0; i < screenWidth / 32; i++){
        DrawLine(i * 64, 0, i * 64, screenHeight, DARKGRAY);
    }
    for(int i = 0; i < screenHeight / 32; i++){
        DrawLine(0, i * 64, screenWidth, i * 64, DARKGRAY);
    }
}