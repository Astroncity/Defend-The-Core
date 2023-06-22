// some_header_file.h
#ifndef GUI_H
#define GUI_H

#include "raylib.h"


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
    Vector2 pos;
    Rectangle hitbox;
    Rectangle src;
    Texture2D texture;
    Color tint;
    double scale;
    void (*func)();
}Button;



typedef struct ButtonArgs{
    Tower tower;
}ButtonArgs;



void handleButtonTower();
void handleButtonNorm(Button* button, Vector2 mousePos);
void handleButton(Button* button, Vector2 mousePos, ButtonArgs args);
Button initButton(Vector2 pos, Rectangle src, Texture2D texture, Vector2 shadow, void (*func)());
void drawTextShadow(Font font, const char* text, Vector2 pos, int fontSize, Color color);

#endif