#include "gui.h"
#include <memory.h>
#include <string.h>
#include <stdlib.h>


void handleButton(Button* button, Vector2 mousePos, ButtonArgs args){
    if(args.tower != NONE){
        handleButtonTower(button, args.tower, mousePos);
    }
    else{
        handleButtonNorm(button, mousePos);
    }
}

Button initButton(int x, int y, int width, int height, int borderThickness,
                  int textOffset, const char* text, Color color,  int fontSize, void (*func)()){
    Button button;
    button.x = x;
    button.y = y;
    button.width = width;
    button.height = height;
    button.textOffset = textOffset;
    button.func = func;
    button.text = malloc((strlen(text) + 1) * sizeof(char)); // Allocate memory for text
    strcpy(button.text, text);
    button.color = color;
    button.borderColor = BLACK;
    button.borderThickness = borderThickness;
    button.borderX = button.x - button.borderThickness;
    button.borderY = button.y - button.borderThickness;
    button.borderWidth = button.width + (2 * button.borderThickness);
    button.borderHeight = button.height + (2 * button.borderThickness);
    button.fontSize = fontSize;
    return button;
}











void handleButtonTower(Button* button, Tower tower, Vector2 mousePos){
    // border
    DrawRectangle(button -> borderX,
                  button -> borderY, 
                  button -> borderWidth, 
                  button -> borderHeight, 
                  button -> borderColor);
    // button  
    DrawRectangle(button -> x,
                  button -> y, 
                  button -> width, 
                  button -> height, 
                  button -> color);

    int centerX = button -> x + button -> width / 2;
    int centerY = button -> y + button -> height / 2;
    int startX = centerX - strlen(button -> text) * button -> fontSize / 2;
    int startY = centerY - button -> fontSize / 2;
    DrawText(button -> text, (startX + button -> textOffset), startY, button -> fontSize, BLACK);

    if (CheckCollisionPointRec(mousePos, (Rectangle){button -> borderX, button -> borderY,
                               button -> borderWidth, button -> borderHeight})){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            button -> func(tower);
        }
        button -> borderColor = BLUE;
    }
    else{
        button -> borderColor = BLACK;
    }

}

void handleButtonNorm(Button* button, Vector2 mousePos){
    // border
    DrawRectangle(button -> borderX,
                  button -> borderY, 
                  button -> borderWidth, 
                  button -> borderHeight, 
                  button -> borderColor);
    // button  
    DrawRectangle(button -> x,
                  button -> y, 
                  button -> width, 
                  button -> height, 
                  button -> color);

    int centerX = button -> x + button -> width / 2;
    int centerY = button -> y + button -> height / 2;
    int startX = centerX - strlen(button -> text) * button -> fontSize / 2;
    int startY = centerY - button -> fontSize / 2;
    DrawText(button -> text, (startX + button -> textOffset), startY, button -> fontSize, BLACK);

    if (CheckCollisionPointRec(mousePos, (Rectangle){button -> borderX, button -> borderY,
                               button -> borderWidth, button -> borderHeight})){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            button -> func();
        }
        button -> borderColor = BLUE;
    }
    else{
        button -> borderColor = BLACK;
    }

}

