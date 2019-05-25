#ifndef PENGUINS_INPUT_H
#define PENGUINS_INPUT_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "cglm.h"

vec2 inputMousePos;
void initInput(GLFWwindow *window);
bool inputGetKeyDown(int GLFW_KEY);
bool inputGetKey(int GLFW_KEY);
bool inputGetKeyUp(int GLFW_KEY);
bool inputAnyKey();
bool inputGetMouseKeyDown(int GLFW_MOUSE_BUTTON);
bool inputGetMouseKeyUp(int GLFW_MOUSE_BUTTON);


void inputResetKeys();
#endif //PENGUINS_INPUT_H
