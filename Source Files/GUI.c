#include "../Header Files/GUI.h"
#include "../Header Files/SpriteRenderer.h"
#include "../Header Files/Physics.h"
#include "../Header Files/Utilities.h"
#include "../Header Files/Input.h"

Sprite *boxSprite = NULL;

void calculateFontRects(Font *font){
    for (int i = 0; i < 256-32; i++) {
        vec2 fontRects[4];

        fontRects[0][0] = (i % (font->texture->width/font->cellSize)) * (float)font->cellSize / font->texture->width;
        fontRects[0][1] = 1-((i / ((int)((float)font->texture->width/font->cellSize)))+1) * (float)font->cellSize / font->texture->height;

        fontRects[1][0] = fontRects[0][0];
        fontRects[1][1] = fontRects[0][1] + font->cellSize / (float)font->texture->height;

        fontRects[2][0] = fontRects[0][0] + font->widths[i+font->startChar] / (float)font->texture->width;
        fontRects[2][1] = fontRects[1][1];

        fontRects[3][0] = fontRects[2][0];
        fontRects[3][1] = fontRects[0][1];

    Sprite *sprite = createNewSprite((vec3) {0,0,0},
                                     (vec3) {(font->widths[i + font->startChar] / (float) font->cellSize), 1, 1}, 0,
                                     font->texture,
                                     fontRects, false);
    font->charSprites[i] = sprite;
    }
}

void guiDrawBox(int posX, int posY, int width, int height, vec3 color){
    if (boxSprite == NULL)
        boxSprite = createNewSprite((vec2){0,0}, (vec2){1,1}, 0, resGetTexture(AVAILABLE_TEXTURES_BOX1), NULL, false);
    guiDrawSprite(boxSprite, posX, posY, width, height, color);
}

void guiDrawSprite(Sprite *sprite, int posX, int posY, int width, int height, vec3 color){
    if (color == NULL)
        color = (vec3){1,1,1};

    vec3 hitPoint;
    int *screenSize = getScreenSize();
    screenToWorldPoint((posX / 1920.0f) * screenSize[0], (posY/ 1080.0f) * screenSize[1], hitPoint, true);

    glm_vec3_dup(((vec3){hitPoint[0], hitPoint[1]}), sprite->position);
    glm_vec3_dup(((vec2){width/100.0f, height/100.0f}), sprite->scale);
    glm_vec3_dup(color, sprite->color);

    drawSprite(sprite);
}

void guiDrawText(char *text, vec2 pixelPosition, vec3 scale, vec3 color, Font *font){
    if (scale == NULL)
        scale = (vec3){1,1,1};

    if  (font == NULL)
        font = resGetFont(AVAILABLE_FONTS_8_BIT_DRAGON);

    if (color == NULL)
        color = (vec3){1,1,1};

    float offsetPixX = 0;
    
    vec3 basePosition;
    int prevWidth = 0;

    int width = getScreenSize()[0];
    int height = getScreenSize()[1];
    screenToWorldPoint((pixelPosition[0] / 1920.0f) * width , (pixelPosition[1] / 1080.0f) * height, basePosition, true);
    for (int i = 0; text[i]; ++i) {
       // char ch = text[i];
        Sprite *sprite = font->charSprites[text[i]-font->startChar];
        if (i > 0) {
            offsetPixX += ((float)prevWidth) / font->cellSize * scale[0];
        }
        vec3 charWorldPosition;
        glm_vec3_dup(basePosition, charWorldPosition);
        charWorldPosition[0] += offsetPixX;
        
        glm_vec3_dup(charWorldPosition, sprite->position);
        glm_vec3_dup(color, sprite->color);

        vec3 origScale;
        glm_vec3_dup(sprite->scale, origScale);
        glm_vec3_mul(origScale, scale, sprite->scale);

        drawSprite(sprite);

        glm_vec3_dup(origScale, sprite->scale);
        prevWidth = font->widths[text[i]];
    }
}

bool guiOnHover(Sprite *guiSprite){
    vec3 outHitPoint;
    screenToWorldPoint(inputMousePos[0], inputMousePos[1], outHitPoint, true);
    float sMinX = guiSprite->position[0];
    float sMaxX = guiSprite->position[0] + guiSprite->scale[0];
    float sMinY = guiSprite->position[1];
    float sMaxY = guiSprite->position[1] - guiSprite->scale[1];

    if (sMinX > sMaxX){
        float tmp = sMinX;
        sMinX = sMaxX;
        sMaxX = tmp;
    }
    if (sMinY > sMaxY){
        float tmp = sMinY;
        sMinY = sMaxY;
        sMaxY = tmp;
    }
    return (outHitPoint[0] >= sMinX && outHitPoint[0] <= sMaxX && outHitPoint[1] >= sMinY && outHitPoint[1] <= sMaxY);
}

bool guiOnClick(Sprite *guiSprite){
    return inputGetMouseKeyDown(GLFW_MOUSE_BUTTON_1) && guiOnHover(guiSprite);
}

bool guiDrawButton(Sprite *sprite, int posX, int posY, int width, int height, vec3 color){
    guiDrawSprite(sprite, posX, posY, width, height, color);
    return guiOnClick(sprite);
}

bool guiDrawButtonBox(int posX, int posY, int width, int height, vec3 color){
    if (boxSprite == NULL)
        boxSprite = createNewSprite((vec2){0,0}, (vec2){1,1}, 0, resGetTexture(AVAILABLE_TEXTURES_BOX1), NULL, false);
    return guiDrawButton(boxSprite, posX, posY, width, height, color);
}