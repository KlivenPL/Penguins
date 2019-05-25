#ifndef PENGUINS_UTILITIES_H
#define PENGUINS_UTILITIES_H
#include <stdbool.h>
#include "cglm.h"
#include <direct.h>
#include "GameWindow.h"


void clearConsole();
int randInt(int min, int max);
float randFloat(float min, float max);
float randFloat01();
bool readFileToEnd(char *path, char **outFileText);
void getUniformMat4(mat4 outUniMat);
int *getScreenSize();
char *getCurrDir();
void screenToWorldPoint(int pixPosX, int pixPosY, vec3 outWorldPosVec3, bool gui);
float *hexColor(char *hexStr);
double fixedDeltaTime;


#define F_COLOR_RED     "\x1b[31m"
#define F_COLOR_BLACK     "\x1b[30m"
#define F_COLOR_GREEN   "\x1b[32m"
#define F_COLOR_YELLOW  "\x1b[33m"
#define F_COLOR_BLUE    "\x1b[34m"
#define F_COLOR_MAGENTA "\x1b[35m"
#define F_COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define BG_COLOR_RED     "\x1b[41m"
#define BG_COLOR_WHITE   "\x1b[47m"
#define BG_COLOR_LIGHT_WHITE   "\x1b[107m"
#define BG_COLOR_GRAY   "\x1b[100m"
#define BG_COLOR_GREEN   "\x1b[42m"
#define BG_COLOR_YELLOW  "\x1b[43m"
#define BG_COLOR_BLUE    "\x1b[44m"
#define BG_COLOR_LIGHT_BLUE    "\x1b[104m"
#define BG_COLOR_MAGENTA "\x1b[45m"
#define BG_COLOR_CYAN    "\x1b[46m"
//#define BG_COLOR_RESET   "\x1b[49m"


#endif //PENGUINS_UTILITIES_H
