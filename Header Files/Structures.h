#ifndef PENGUINS_STRUCTURES_H
#define PENGUINS_STRUCTURES_H
#include "SpriteRenderer.h"
typedef struct Penguin Penguin;

typedef enum FieldType {
    emptyFT, iceFloeFT
} FieldType;

typedef struct Player {
    char name[16];
    char *idStr;
    int id;
    int score;
    bool controlledLocally;
    int penguinsCount;
    int activePenguinId;
    Penguin **penguins;
}Player;

typedef struct Field {
    int posX, posY;
    FieldType fieldType;
    int fishCount;
    Sprite *sprite;
    float randMovement;
    //Penguin *penguin;
}Field;

struct Penguin {
    Field *field;
    Sprite *sprite;
    bool alive;
};

typedef struct Board {
    int sizeX, sizeY;
    Field ***fields; //bi-dimensional array of pointers to fields
}Board;

typedef struct GameState {
    Board *board;
    Player **players;
} GameState;

Field *newField(int posX, int posY, FieldType fieldType, int fishCount);
Board *createRandomBoard(int sizeX, int sizeY);
Board *createEmptyBoard(int sizeX, int sizeY);
Player *createNewPlayer(char name[16], int id, char *idStr, bool controlledLocally);
void addPenguinsToPlayer(Player *player, int count);
//Field *getFieldAtPos(int x, int y);

#endif //PENGUINS_STRUCTURES_H
