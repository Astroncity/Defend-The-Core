#include "utils.h"



GameState gameState = MAIN_MENU;
Vector2 mousePos;
Vector2 virtualMousePos = {0, 0};
double deltaTime;
double difficulty = 1;



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

void drawGrid(int width, int height){
    for(int i = 0; i < width / 32; i++){
        DrawLine(i * 64, 0, i * 64, height, DARKGRAY);
    }
    for(int i = 0; i < height / 32; i++){
        DrawLine(0, i * 64, width, i * 64, DARKGRAY);
    }
}


Vector2 vector2Clamp(Vector2 v, Vector2 min, Vector2 max){
    Vector2 result = v;
    if(result.x < min.x){
        result.x = min.x;
    }
    if(result.x > max.x){
        result.x = max.x;
    }
    if(result.y < min.y){
        result.y = min.y;
    }
    if(result.y > max.y){
        result.y = max.y;
    }
    return result;
}