#include "../Header Files/Resources.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Header Files/stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include "../Header Files/GUI.h"
#include "../Header Files/Utilities.h"

Texture *loadTexture(char *path){
    Texture *texture = malloc(sizeof(Texture));
    texture->data = stbi_load(path, &texture->width, &texture->height, &texture->nrChannels, 0);
    if (!texture->data)
        printf("Texture %s could not be loaded!", path);

    // load the texture
    glGenTextures(1, &texture->tex);
    glBindTexture(GL_TEXTURE_2D, texture->tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

Font *loadFont(char *pathBmp, char *pathCsv){
    FILE *file = fopen(pathCsv, "r");
    if (!file){
        printf("FIle: %s does not exist!", pathCsv);
        return NULL;
    }
    Font *font = malloc(sizeof(Font));
    char cellSizeStr[5], firstCharStr[5];
    fscanf(file, "%[^\n]%*c", cellSizeStr);
    fscanf(file, "%[^\n]%*c", firstCharStr);

    font->cellSize = atoi(cellSizeStr);
    font->startChar = atoi(firstCharStr);

    for (int i = 0; i < 256; ++i) {
        char widthStr[5];
        fscanf(file, "%[^\n]%*c", widthStr);
        font->widths[i] = atoi(widthStr);
    }

    fclose(file);

    font->texture = loadTexture(pathBmp);
    calculateFontRects(font);

    return font;
}

Texture **textures; //one-dimensional array to pointers to texture
Font **fonts; //one-dimensional array to pointers to font

const int TEXTURES_COUNT = 12;
const int FONTS_COUNT = 1;

void initResources(){
    stbi_set_flip_vertically_on_load(1);
    textures = malloc(sizeof(Texture) * TEXTURES_COUNT);

    textures[0] = loadTexture("Lib/Sprites/penguin-yellow.psd");
    textures[1] = loadTexture("Lib/Sprites/penguin-red.psd");
    textures[2] = loadTexture("Lib/Sprites/penguin-green.psd");
    textures[3] = loadTexture("Lib/Sprites/penguin-blue.psd");
    textures[4] = loadTexture("Lib/Sprites/iceFloe1.psd");
    textures[5] = loadTexture("Lib/Sprites/iceFloe2.psd");
    textures[6] = loadTexture("Lib/Sprites/iceFloe3.psd");
    textures[7] = loadTexture("Lib/Sprites/water.psd");
    textures[8] = loadTexture("Lib/Sprites/box1.psd");
    textures[9] = loadTexture("Lib/Sprites/robot.psd");
    textures[10] = loadTexture("Lib/Sprites/human.psd");
    textures[11] = loadTexture("Lib/Sprites/play.psd");

    fonts = malloc(sizeof(Font) * FONTS_COUNT);

    char *path1, *path2;
    path1 = malloc(FILENAME_MAX);
    path2 = malloc(FILENAME_MAX);
    strcpy(path1, getCurrDir());
    strcat(path1, "\\Lib\\Fonts\\8BitDragon.psd");
    strcpy(path2, getCurrDir());
    strcat(path2, "\\Lib\\Fonts\\8BitDragon.csv");
    fonts[0] = loadFont(path1, path2);

    free(path1);
    free(path2);

}

Texture *resGetTexture(AvailableTextures avTex){
    return textures[avTex];
}

Font *resGetFont(AvailableFonts avFont){
    return fonts[avFont];
}


