#include "../Header Files/Utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include "cglm.h"
#include "../Header Files/Penguins.h"

char *currDir = NULL;
GLint m_viewport[4];
double fixedDeltaTime = 0;

void clearConsole() {
    system("cls");
    //printf("\e[1;1H\e[2J");
}

int randInt(int min, int max){
    int tmp;
    if (max >= min)
        max -= min;
    else {
        tmp = min - max;
        min = max;
        max = tmp;
    }
    return max ? (rand() % max + min) : min;
}

float randFloat(float min, float max) {
    return min + (float)(rand()) / ((float)(RAND_MAX / (max - min)));
}

float randFloat01() {
    return (float)(rand()) / (float)(RAND_MAX);
}

bool readFileToEnd(char *path, char **outFileText) {
    FILE *file = fopen(path, "r");
    if(file){
        char ch;
        int size = 0;
        while((ch = fgetc(file)) != EOF)
            size++;

        outFileText[0] = (char*)malloc((size + 1) * sizeof(char));
        rewind(file);
        for(int i = 0; (ch = fgetc(file)) != EOF; i++) {
            outFileText[0][i] = ch;
        }

        fclose(file);
        outFileText[0][size] = '\0';
        return true;
    }
    printf("Could not open file: %s\n", path);
    return false;
}

char *getCurrDir(){
    if (currDir == NULL)
        currDir = malloc(FILENAME_MAX + sizeof(char));
    getcwd(currDir, FILENAME_MAX);
    return currDir;
}

int *getScreenSize(){
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    return &(m_viewport[2]);
}

float col[3];
float *hexColor(char *hexStr){
    char rStr[3], gStr[3], bStr[3];
    rStr[0] = hexStr[0];
    rStr[1] = hexStr[1];
    rStr[2] = '\0';

    gStr[0] = hexStr[2];
    gStr[1] = hexStr[3];
    gStr[2] = '\0';

    bStr[0] = hexStr[4];
    bStr[1] = hexStr[5];
    bStr[2] = '\0';

    int r = strtol(rStr, NULL, 16);
    int g = strtol(gStr, NULL, 16);
    int b = strtol(bStr, NULL, 16);

    col[0] = r / 255.0f;
    col[1] = g / 255.0f;
    col[2] = b / 255.0f;
    return col;
}

void getUniformMat4(mat4 outUniMat){
    mat4 model = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
    };
    glm_mat4_copy(model, outUniMat);
}


void screenToWorldPoint(int pixPosX, int pixPosY, vec3 outWorldPosVec3, bool gui){
    int width = getScreenSize()[0];
    int height = getScreenSize()[1];
    float x = (2.0f * pixPosX) / width - 1.0f;
    float y = 1.0f - (2.0f * pixPosY) / height;
    float z = 1.0f;
    vec3 ray_nds = {x, y, z};

    vec4 rayClip = {ray_nds[0], ray_nds[1], -1.0f, 1.0};

    if (!gui && board) {
        getUniformMat4(srProjMat);
        glm_ortho_default_s(getScreenSize()[0]/(float)getScreenSize()[1], board->sizeX/2.0f, srProjMat);
        glm_translate(srProjMat, (vec3){
                -board->fields[board->sizeX-1][board->sizeY-1]->sprite->position[0]/1.5f - 0.5f,
                -board->fields[board->sizeX-1][board->sizeY-1]->sprite->position[1]/2 + 0.5f, 0});

    }
    mat4 reversedProj;
    glm_mat4_inv(srProjMat, reversedProj);

    vec4 tmpRayProj;
    glm_mat4_mulv(reversedProj, rayClip, tmpRayProj);

    glm_vec3_copy((vec3){tmpRayProj[0], tmpRayProj[1], 0}, outWorldPosVec3);
}