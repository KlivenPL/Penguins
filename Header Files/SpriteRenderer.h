#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H
#include <stdbool.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Resources.h"
#include "cglm.h"


void initSpriteRenderer();
bool compileShader(GLenum GL_TYPE_SHADER, unsigned int *handle, char *path);
void drawSprite(Sprite *sprite);

//Sprite *createNewSprite(float *vertices, float posX, float posY, int renderingOrder);
Sprite *createNewSprite(vec2 position, vec2 scale, int renderingOrder, Texture *texture, vec2 texPos[4], bool addToSpriteList);
void removeSprite(Sprite *sprite);
void sortSprites(Sprite **sprites, int count);
mat4 srProjMat;
#endif //SPRITERENDERER_H
