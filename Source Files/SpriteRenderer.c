#include "../Header Files/SpriteRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Header Files/Utilities.h"
#include <stdio.h>
#include "../Header Files/GameWindow.h"
#include <../cglm/cglm.h>
#include "../OpenGL/GLM/include/cglm/call.h"
#include <time.h>
#include "../Header Files/Penguins.h"
#include <string.h>

unsigned int _srVertShader, _srFragShader, srShader;
char SR_VERTEX_SHADER_PATH[200] = "\\Source Files\\Shaders\\Vertex.glsl";
char SR_FRAGMENT_SHADER_PATH[200] = "\\Source Files\\Shaders\\Frag.glsl";
mat4 srProjMat;


Sprite *createNewSprite(vec2 position, vec2 scale, int renderingOrder, Texture *texture, vec2 texPos[4], bool addToSpriteList){
    Sprite *sprite = (Sprite*)malloc(sizeof(Sprite));
    glm_vec3_dup(position, sprite->position);
    glm_vec3_dup(scale, sprite->scale);
    glm_vec3_dup(((vec3){1,1,1}), sprite->color);
    sprite->renderingOrder = renderingOrder;
    sprite->texture = texture;
    sprite->visible = true;

    vec2 defTexPos[4] = {
            {0,0},
            {0,1},
            {1,1},
            {1,0},
    };

    if (texPos == NULL)
        texPos = defTexPos;



    glGenBuffers(1, &sprite->vbo);
    glGenVertexArrays(1, &sprite->vao);
    glGenBuffers(1, &sprite->ebo);

    glBindVertexArray(sprite->vao);

    float spriteData[] = {
                //verts         texture
        0.5f,  0.5f, 0.0f,      texPos[2][0], texPos[2][1],   // top right
        0.5f, -0.5f, 0.0f,      texPos[3][0], texPos[3][1],   // bottom right
        -0.5f, -0.5f, 0.0f,     texPos[0][0], texPos[0][1],   // bottom left
        -0.5f,  0.5f, 0.0f,     texPos[1][0], texPos[1][1]    // top left
    };
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spriteData), spriteData, GL_STATIC_DRAW);

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    if (addToSpriteList)
        sprites[spritesCount++] = sprite;

    return sprite;
}

void removeSprite(Sprite *sprite){
    int sprId = -1;
    for (int i = 0; i < spritesCount; ++i) {
        if (sprites[i] == sprite) {
            sprId = i;
            break;
        }
    }

    glDeleteBuffers(1, &sprite->ebo);
    glDeleteBuffers(1, &sprite->vao);
    glDeleteBuffers(1, &sprite->vbo);
    free(sprite);

    if (sprId != -1){
        for (int i = sprId; i < spritesCount; ++i) {
            sprites[i] = sprites[i + 1];
        }
        spritesCount--;
    }
}

void initSpriteRenderer() {
    char *path = getCurrDir();
    strcat(path, SR_VERTEX_SHADER_PATH);
    compileShader(GL_VERTEX_SHADER, &_srVertShader, path);
    //free(path);
    path = getCurrDir();
    strcat(path, SR_FRAGMENT_SHADER_PATH);
    compileShader(GL_FRAGMENT_SHADER, &_srFragShader, path);

    srShader = glCreateProgram();

    glAttachShader(srShader, _srVertShader);
    glAttachShader(srShader, _srFragShader);
    glLinkProgram(srShader);

    int success;
    char infoLog[512];
    glGetProgramiv(srShader, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(srShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::CREATING_PROGRAM::FAILED\n%s", infoLog);
        return;
    }

    glDeleteShader(_srVertShader);
    glDeleteShader(_srFragShader);

}

bool compileShader(GLenum GL_TYPE_SHADER, unsigned int *handle, char *path){
    char *shaderString;

    *handle = glCreateShader(GL_TYPE_SHADER);
    if (!readFileToEnd(path, &shaderString))
        return false;

    glShaderSource(*handle, 1, &shaderString, NULL);
    glCompileShader(*handle);

    int success;
    char infoLog[512];
    glGetShaderiv(*handle, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(*handle, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s", infoLog);
        return false;
    }
    return true;

}

void drawSprite(Sprite *sprite) {
    if (sprite->visible == false)
        return;
    glUseProgram(srShader);

    mat4 model;
    getUniformMat4(model);

    glm_translate(model, (vec3){sprite->position[0]+sprite->scale[0]/2.0f, sprite->position[1]-sprite->scale[1]/2.0f, 0.0f});
    glm_scale(model, sprite->scale);
    int modelLoc = glGetUniformLocation(srShader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);

    int projLoc = glGetUniformLocation(srShader, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)srProjMat);

    int colorLoc = glGetUniformLocation(srShader, "color");
    glUniform3fv(colorLoc, 1, sprite->color);

    glBindTexture(GL_TEXTURE_2D, sprite->texture->tex);
    glBindVertexArray(sprite->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void sortSprites(Sprite **sprites, int count){
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < count-1; ++j) {
            if (sprites[j]->renderingOrder < sprites[j+1]->renderingOrder){
                Sprite *tmp = sprites[j];
                sprites[j] = sprites[j + 1];
                sprites[j + 1] = tmp;
            }
        }
    }
}