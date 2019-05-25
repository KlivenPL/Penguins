#ifndef PENGUINS_RESOURCES_H
#define PENGUINS_RESOURCES_H
#include "cglm.h"
//#include "SpriteRenderer.h"

typedef struct Texture {
    int width, height, nrChannels;
    unsigned int tex;
    unsigned char *data;
}Texture;

typedef enum AvailableTextures{
    AVAILABLE_TEXTURES_PENGUIN_YELLOW,
    AVAILABLE_TEXTURES_PENGUIN_RED,
    AVAILABLE_TEXTURES_PENGUIN_GREEN,
    AVAILABLE_TEXTURES_PENGUIN_BLUE,
    AVAILABLE_TEXTURES_ICE_FLOE1,
    AVAILABLE_TEXTURES_ICE_FLOE2,
    AVAILABLE_TEXTURES_ICE_FLOE3,
    AVAILABLE_TEXTURES_WATER,
    AVAILABLE_TEXTURES_BOX1,
    AVAILABLE_TEXTURES_ROBOT,
    AVAILABLE_TEXTURES_HUMAN,
    AVAILABLE_TEXTURES_PLAY,
} AvailableTextures;

typedef struct Sprite {
    vec3 position, scale, color;
    int renderingOrder;
    Texture *texture;
    bool visible;

    unsigned int vbo, vao, ebo/*, tex*/;
}Sprite;

typedef struct Font {
    Texture *texture;
    int cellSize, startChar;
    int widths[256];
    Sprite *charSprites[256];

}Font;

typedef enum AvailableFonts {
    AVAILABLE_FONTS_8_BIT_DRAGON
} AvailableFonts;

void initResources();

Texture *resGetTexture(AvailableTextures avTex);
Font *resGetFont(AvailableFonts avFont);


#endif //PENGUINS_RESOURCES_H
