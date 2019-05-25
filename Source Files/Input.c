#include "../Header Files/Input.h"
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "cglm.h"

int lastKey = -1, lastAction = -1;
int lastMouseButton = -1, lastMouseAction = -1;
vec2 inputMousePos;

void keyDownCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    lastKey = key;
    lastAction = action;
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    inputMousePos[0] = (int)xpos;
    inputMousePos[1] = (int)ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    lastMouseButton = button;
    lastMouseAction = action;
}

void initInput(GLFWwindow *window){
    glfwSetKeyCallback(window, keyDownCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}


bool inputGetKeyDown(int GLFW_KEY){
    return (lastKey == GLFW_KEY && lastAction == GLFW_PRESS);
}
bool inputGetKey(int GLFW_KEY){
    return (lastKey == GLFW_KEY && lastAction != GLFW_RELEASE);
}
bool inputGetKeyUp(int GLFW_KEY){
    return (lastKey == GLFW_KEY && lastAction == GLFW_RELEASE);
}

bool inputAnyKey(){
    return (lastKey != -1);
}

bool inputGetMouseKeyDown(int GLFW_MOUSE_BUTTON){
    return (lastMouseButton == GLFW_MOUSE_BUTTON && lastMouseAction == GLFW_PRESS);
}

bool inputGetMouseKeyUp(int GLFW_MOUSE_BUTTON){
    return (lastMouseButton == GLFW_MOUSE_BUTTON && lastMouseAction == GLFW_RELEASE);
}


void inputResetKeys(){
    lastKey = -1;
    lastAction = -1;
    lastMouseAction = -1;
    lastMouseButton = -1;
}