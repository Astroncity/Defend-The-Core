#include "utils.h"
#include "stdbool.h"
#include <stdlib.h>
#include "player.h"
#include <stdio.h>
#include <math.h>
GameState gameState = MAIN_MENU;
Vector2 mousePos;
Vector2 virtualMousePos = {0, 0};
double deltaTime;
double difficulty = 1;



void rotateVector2(Vector2* vector, float angle, Vector2 center){
    float s = sin(angle);
    float c = cos(angle);

    vector->x -= center.x;
    vector->y -= center.y;

    // rotate point
    float xnew = vector->x * c - vector->y * s;
    float ynew = vector->x * s + vector->y * c;

    vector->x = xnew + center.x;
    vector->y = ynew + center.y;
}

double degToRad(double angle){
    return angle * PI / 180;
}
double radToDeg(double angle){
    return angle * 180 / PI;
}

void translateTrig(Trig* trig, double x, double y){
    trig->a.x += x;
    trig->a.y += y;
    trig->b.x += x;
    trig->b.y += y;
    trig->c.x += x;
    trig->c.y += y;
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


Vector2* rotateRect(Rectangle rect, double angle, Vector2 center){
    Vector2* result = malloc(sizeof(Vector2) * 4);
    Vector2 topLeft = {rect.x, rect.y};
    Vector2 topRight = {rect.x + rect.width, rect.y};
    Vector2 bottomLeft = {rect.x, rect.y + rect.height};
    Vector2 bottomRight = {rect.x + rect.width, rect.y + rect.height};

    rotateVector2(&topLeft, angle, center);
    rotateVector2(&topRight, angle, center);
    rotateVector2(&bottomLeft, angle, center);
    rotateVector2(&bottomRight, angle, center);

    result[0] = topLeft;
    result[1] = topRight;
    result[2] = bottomLeft;
    result[3] = bottomRight;
    return result;    
}

Vector2 subtractVector2(Vector2 x, Vector2 y){
    Vector2 result = {x.x - y.x, x.y - y.y};
    return result;
}

bool circleRectCollision(Vector2 circlePos, double circleRadius, Vector2* points, double angle){   
    double x1,y1,x2,y2,x3,y3,x4,y4;
    double closestEdgeDist = 9999999;
    Vector2 closestEdge[2];
    Vector2 lines[4][2];
    if(points == NULL){
        printf("points is null\n");
        return false;
    }
    lines[0][0] = points[0];
    lines[0][1] = points[1];
    lines[1][0] = points[0];
    lines[1][1] = points[2];
    lines[2][0] = points[2];
    lines[2][1] = points[3];
    lines[3][0] = points[3];
    lines[3][1] = points[1];
    Vector2 mid;


    for(int i = 0; i < 4; i++){
        mid = midPoint(lines[i][0], lines[i][1]);
        if(distance(circlePos, mid) < closestEdgeDist){
            closestEdgeDist = distance(circlePos, mid);
            closestEdge[0] = lines[i][0];
            closestEdge[1] = lines[i][1];
        }
    }
    //DrawLineEx(closestEdge[0], closestEdge[1], 5, RED);
    if(angle > 2 * PI){
        angle -= PI * 2;
    }
    if(angle < 0){
        angle += PI * 2;
    }

    x1 = closestEdge[0].x; y1 = closestEdge[0].y;
    x2 = closestEdge[1].x; y2 = closestEdge[1].y;
    x3 = circlePos.x; y3 = circlePos.y;
    x4 = circlePos.x + cos(angle) * 100; y4 = circlePos.y + sin(angle) * 100;

    //DrawLineEx((Vector2){x3, y3}, (Vector2){x4, y4}, 5, GREEN);

    double t = 99999;
    double u = 99999;
    Vector2 p;

     
    t = (((x1 - x3) * (y3 - y4)) - ((y1 - y3) * (x3 - x4))) /
        (((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4))) + 0.0000001;

    //printf("t: %f\n", t);

    if((t >= 0 && t <= 1) == false){
        u = (((x1 - x3) * (y1 - y2)) - ((y1 - y3) * (x1 - x2))) /
            (((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))  +0.00001;
        if((u >= 0 && u <= 1) == false){
            //printf("u: %f\n", u);
            return false;
        }
    }
    if(t != 99999){
        p = (Vector2){x1 + t * (x2 - x1), y1 + t * (y2 - y1)};
    }
    else{
        p = (Vector2){x3 + u * (x4 - x3), y3 + u * (y4 - y3)};
    }
    DrawCircle(p.x, p.y, 10, BLUE);
    if(distance(circlePos, p) < circleRadius){
        return true;
    }
    return false;
}


double dotProductVector2(Vector2 v1, Vector2 v2){
    return v1.x * v2.x + v1.y * v2.y;
}

Vector2 normalizeVector2(Vector2 v){
    double length = sqrt(v.x * v.x + v.y * v.y);
    if(length == 0){
        return v;
    }
    return (Vector2){v.x / length, v.y / length};
}

Vector2 perpendicularVector2(Vector2 v){
    return (Vector2){-v.y, v.x};
}

Vector2 midPoint(Vector2 v1, Vector2 v2){
    return (Vector2){(v1.x + v2.x) / 2, (v1.y + v2.y) / 2};
}