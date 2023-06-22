#ifndef CORE_H
#define CORE_H

#include "raylib.h"
#include "gui.h"
#define CORESCALE 8

typedef struct Core{
    double health;
    double maxHealth;
    Rectangle rect;
    Color color;
    HealthBar healthBar;
    Vector2 center;
}Core;


extern double coreBonusTimer;
extern Core core;
extern Texture2D coreTexture;

void handleCore();

#endif /*CORE_H*/