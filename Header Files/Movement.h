#ifndef PENGUINS_MOVEMENT_H
#define PENGUINS_MOVEMENT_H

#include "SpriteRenderer.h"
#include "Structures.h"

typedef struct Cursor {
    int posX, posY;
}Cursor;

bool checkField(int fieldX, int fieldY, bool *outBlockedByOtherPenguin);
Field **getAvailableFields(Penguin *penguin, bool *outBlockedByOtherPenguin);

#endif //PENGUINS_MOVEMENT_H
