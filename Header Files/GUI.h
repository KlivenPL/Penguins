#ifndef PENGUINS_GUI_H
#define PENGUINS_GUI_H

#include "Resources.h"


void calculateFontRects(Font *font);
void guiDrawText(char *text, vec2 pixelPosition, vec3 scale, vec3 color, Font *font);
void guiDrawBox(int posX, int posY, int width, int height, vec3 color);
void guiDrawSprite(Sprite *sprite, int posX, int posY, int width, int height, vec3 color);
bool guiOnHover(Sprite *guiSprite);
bool guiOnClick(Sprite *guiSprite);
bool guiDrawButton(Sprite *sprite, int posX, int posY, int width, int height, vec3 color);
bool guiDrawButtonBox(int posX, int posY, int width, int height, vec3 color);


#endif //PENGUINS_GUI_H
