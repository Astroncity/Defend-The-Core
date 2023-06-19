// some_header_file.h
#ifndef GUI_H
#define GUI_H

#include "../lib/raylib.h"


typedef enum Tower{
    NONE,
    SNIPER
}Tower;

typedef struct HealthBar{
    double value;
    double max;
    Rectangle rect;
    Color barColor;
    Color backgroundColor;
}HealthBar;


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



typedef struct ButtonArgs{
    Tower tower;
}ButtonArgs;



void handleButtonTower();
void handleButtonNorm(Button* button, Vector2 mousePos);
void handleButton(Button* button, Vector2 mousePos, ButtonArgs args);
Button initButton(int x, int y, int width, int height, int borderThickness, 
                  int textOffset, const char* text, Color color, int fontSize, void (*func)());



#endif