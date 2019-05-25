#include "../Header Files/Structures.h"
#include "../Header Files/Utilities.h"
#include <stdlib.h>

Field *newField(int posX, int posY, FieldType fieldType, int fishCount) {
    Field *field = (Field*)malloc(sizeof(Field));
    field->fieldType = fieldType;
    field->posX = posX;
    field->posY = posY;
    field->fishCount = fishCount;
    field->randMovement = -2137;
    //field->penguin = NULL;
    return field;
}

Board *createRandomBoard(int sizeX, int sizeY) {
    Board *board = (Board*)malloc(sizeof(Board));
    board->sizeX = sizeX;
    board->sizeY = sizeY;
    board->fields = (Field***)malloc(sizeof(Field**) * sizeY);
    for (int y = 0; y < sizeY; y++) {
        board->fields[y] = (Field**)malloc(sizeof(Field*) * sizeX);
        for (int x = 0; x < sizeX; x++) {
            board->fields[y][x] = newField(x, y, (randFloat01() < 0.12f ? emptyFT : iceFloeFT), randInt(1, 4));
        }
    }
    return board;
}

Board *createEmptyBoard(int sizeX, int sizeY) {
    Board *board = (Board*)malloc(sizeof(Board));
    board->sizeX = sizeX;
    board->sizeY = sizeY;
    board->fields = (Field***)malloc(sizeof(Field**) * sizeY);
    for (int y = 0; y < sizeY; y++) {
        board->fields[y] = (Field**)malloc(sizeof(Field*) * sizeX);
        for (int x = 0; x < sizeX; x++) {
            board->fields[y][x] = newField(x, y, emptyFT, 0);
        }
    }
    return board;
}

Player *createNewPlayer(char name[16], int id, char *idStr, bool controlledLocally){
    Player *player = malloc(sizeof(Player));
    strcpy(player->name, name);
    player->controlledLocally = controlledLocally;
    player->score = 0;
    player->penguins = NULL;
    player->activePenguinId = 0;
    player->id = id;
    int tmpIdStrLength = 0;
    for (int i = 0; i < idStr[i]; ++i) {
        tmpIdStrLength++;
    }
    player->idStr = malloc(sizeof(char) * (tmpIdStrLength + 1));
    if (idStr[0] == 0) {
        player->idStr = malloc(sizeof(char) * 6);
        sprintf(player->idStr, "%c%c%c%c%c", randInt(48, 73), randInt(48, 73), randInt(48, 73), randInt(48, 73),
                randInt(48, 73));
    } else
        strcpy(player->idStr, idStr);
    return player;
}

void addPenguinsToPlayer(Player *player, int count){
    player->penguinsCount = count;
    player->penguins = malloc(sizeof(Penguin*) * count);
    for (int i = 0; i < count; ++i) {
        player->penguins[i] = malloc(sizeof(Penguin));
        player->penguins[i]->field = NULL;
        player->penguins[i]->sprite = NULL;
        player->penguins[i]->alive = true;
    }
}
