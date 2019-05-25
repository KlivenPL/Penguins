#ifndef PENGUINS_MENU_H
#define PENGUINS_MENU_H

#include "Resources.h"

void menuGuiUpdate();

typedef enum MenuAvailableStates {MENU_AVAILABLE_STATES_NONE, MENU_AVAILABLE_STATES_MAIN_MENU, MENU_AVAILABLE_STATES_GAME } MenuAvailableStates;
MenuAvailableStates menuState;
float backgroundColor[3];
void initMenu();

Sprite *skins[4][4];
int choosenSkins[4];

#endif //PENGUINS_MENU_H
