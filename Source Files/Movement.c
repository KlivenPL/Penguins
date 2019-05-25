#include "../Header Files/Movement.h"
#include "../Header Files/Penguins.h"

bool checkField(int fieldX, int fieldY, bool *outBlockedByOtherPenguin){
    if ((fieldX >= 0 && fieldY >= 0 && fieldX < board->sizeX && fieldY < board->sizeY) == false) {
        return false;
    }
    Field *field = board->fields[fieldY][fieldX];
    for (int i = 0; i < playersCount; ++i)
        for (int j = 0; j < players[i]->penguinsCount; ++j)
            if (field == players[i]->penguins[j]->field && players[i]->penguins[j]->alive) {
                if (outBlockedByOtherPenguin[0] == false)
                    outBlockedByOtherPenguin[0] = true;
                return false;
            }

    return field != NULL && field->fieldType != emptyFT && field->fishCount > 0;

}

Field **getAvailableFields(Penguin *penguin, bool *outBlockedByOtherPenguin){
    bool blockedByOtherPenguin = false;
    int maxFields = board->sizeY * board->sizeX;
    Field **avFields = (Field**)malloc(sizeof(Field*) * (maxFields + 1));
    for (int i = 0; i <= maxFields; ++i) {
        avFields[i] = NULL;
    }
    int lastField = 0;
    if (penguin == NULL){ // happens during placing stage.
        for (int i = 0; i < board->sizeX; ++i) {
            for (int j = 0; j < board->sizeY; ++j) {
                if (checkField(i, j, &blockedByOtherPenguin) && board->fields[j][i]->fishCount == 1)
                    avFields[lastField++] = board->fields[j][i];
            }
        }
        return avFields;
    }
    Field *startField = penguin->field;
    int fieldX = startField->posX + 1, fieldY = startField->posY;

    while(checkField(fieldX, fieldY, &blockedByOtherPenguin)){
        avFields[lastField++] = board->fields[fieldY][fieldX];
        fieldX++;
    }
    fieldX = startField->posX - 1;
    fieldY = startField->posY;

    while(checkField(fieldX, fieldY, &blockedByOtherPenguin)){
        avFields[lastField++] = board->fields[fieldY][fieldX];
        fieldX--;
    }
    fieldX = startField->posX;
    fieldY = startField->posY + 1;

    while(checkField(fieldX, fieldY, &blockedByOtherPenguin)){
        avFields[lastField++] = board->fields[fieldY][fieldX];
        fieldY++;
    }
    fieldX = startField->posX;
    fieldY = startField->posY - 1;

    while(checkField(fieldX, fieldY, &blockedByOtherPenguin)){
        avFields[lastField++] = board->fields[fieldY][fieldX];
        fieldY--;
    }

    for (int i = 0; avFields[i]; ++i) {
        if(i == 2){
            outBlockedByOtherPenguin[0] = blockedByOtherPenguin;
            break;
        }
    }

    return avFields;

}