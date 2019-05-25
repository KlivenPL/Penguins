#include <stdbool.h>
#include "../Header Files/Physics.h"

void physicsMouseRaycast(int mouseX, int mouseY, vec3 outHitPoint, Sprite **outHitSprite) {
    screenToWorldPoint(mouseX, mouseY, outHitPoint, false);
    for (int i = spritesCount - 1; i >= 0; i--) {
        if (sprites[i]->visible == false)
            continue;
        float sMinX = sprites[i]->position[0];
        float sMaxX = sprites[i]->position[0] + sprites[i]->scale[0];
        float sMinY = sprites[i]->position[1];
        float sMaxY = sprites[i]->position[1] - sprites[i]->scale[1];


        if (sMinX > sMaxX){
            float tmp = sMinX;
            sMinX = sMaxX;
            sMaxX = tmp;
        }
        if (sMinY > sMaxY){
            float tmp = sMinY;
            sMinY = sMaxY;
            sMaxY = tmp;
        }

        if (outHitPoint[0] >= sMinX && outHitPoint[0] <= sMaxX && outHitPoint[1] >= sMinY && outHitPoint[1] <= sMaxY){
            outHitSprite[0] = sprites[i];
            break;
        }
    }
}