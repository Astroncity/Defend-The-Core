#include "gui.h"
#include <memory.h>
#include <string.h>
#include <stdlib.h>




Button initButton(Vector2 pos, Rectangle src, Texture2D texture, Vector2 shadow, void (*func)()){
    Button button;
    button.pos = pos;
    button.src = src;
    button.texture = texture;
    button.func = func;
    button.scale = 3;
    button.tint = WHITE;
    button.hitbox = (Rectangle){pos.x + shadow.x, pos.y + shadow.y,
                                src.width - shadow.x, src.height - shadow.y};

    button.hitbox.width *= button.scale;
    button.hitbox.height *= button.scale;
    
    return button;
}

void handleButton(Button* button, Vector2 mousePos, ButtonArgs args){
    if(args.tower != NONE){
        handleButtonTower(button, args.tower, mousePos);
    }
    else{
        handleButtonNorm(button, mousePos);
    }
}


void handleButtonTower(Button* button, Tower tower, Vector2 mousePos){
    DrawTextureEx(button -> texture, button -> pos, 0, button -> scale, button -> tint);

    if (CheckCollisionPointRec(mousePos, (Rectangle){button -> hitbox.x, button -> hitbox.y,
                               button -> hitbox.width, button -> hitbox.height})){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            button -> func(tower);
        }
        button -> tint = BLUE;
    }
    else{
        button -> tint = WHITE;
    }

}


void handleButtonNorm(Button* button, Vector2 mousePos){
    DrawTextureEx(button -> texture, button -> pos, 0, button -> scale, button -> tint);

    if (CheckCollisionPointRec(mousePos, (Rectangle){button -> hitbox.x, button -> hitbox.y,
                               button -> hitbox.width, button -> hitbox.height})){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            button -> func();
        }
        button -> tint = BLUE;
    }
    else{
        button -> tint = WHITE;
    }

}

