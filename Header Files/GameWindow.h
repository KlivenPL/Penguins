//
// Created by oskar on 24.03.2019.
//

#ifndef PENGUINS_GAMEWINDOW_H
#define PENGUINS_GAMEWINDOW_H
#include "SpriteRenderer.h"
int glCreateWindow();
Sprite **sprites;
int spritesCount;
const int MAX_SPRITES;
GLFWwindow* window;
float *boardPosVec3;
#endif //PENGUINS_GAMEWINDOW_H
