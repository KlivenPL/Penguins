#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include "../Header Files/SpriteRenderer.h"
#include "../Header Files/Penguins.h"
#include <../cglm/cglm.h>
#include "../Header Files/Resources.h"
#include "../Header Files/Input.h"
#include "../Header Files/Utilities.h"
#include "../Header Files/Menu.h"
#include <time.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void initGlStructures();
void drawOpenGL();
void drawOpenGlGui();

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
GLFWwindow* window = NULL;

Sprite **sprites = NULL;
int spritesCount = 0;
int lastSpritesCount = 0;
const int MAX_SPRITES = 1000;

clock_t fixedTimerLast, fixedTimerCurr;

int glCreateWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Penguins", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    initGlStructures();

   // Sprite *testSprite = createNewSprite((vec2){0,0},(vec2){1/3.0f * 3,2/3.0f * 3}, 0, resGetTexture(AVAILABLE_TEXTURES_PENGUIN_YELLOW));

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        //glClearColor(23/256.0f, 111/256.0f, 180/256.0f, 0.0f);
        static float sinFlt1 = 2137;
        static float sinFlt2 = 2137;
        static float sinFlt3 = 2137;

        if (sinFlt1 == 2137) {
            sinFlt1 = randFloat(-4, 4);
            sinFlt2 = randFloat(-4, 4);
            sinFlt3 = randFloat(-4, 4);
            fixedTimerLast = clock();
        }
        fixedTimerCurr = clock();
        fixedDeltaTime = (fixedTimerCurr - fixedTimerLast)/(double)CLOCKS_PER_SEC;
        if (fixedDeltaTime >= 1/60.0f) {
            fixedTimerLast = clock();
            backgroundColor[0] = (1 + sinf(sinFlt1 += fixedDeltaTime / 2.0 * randFloat01())) / 2.0f * 0.75f;
            backgroundColor[1] = (1 + sinf(sinFlt2 += fixedDeltaTime / 2.0 * randFloat01())) / 2.0f * 0.75f;
            backgroundColor[2] = (1 + sinf(sinFlt3 += fixedDeltaTime / 2.0 * randFloat01())) / 2.0f * 0.75f;
            fixedGameLoop();
        }
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        gameLoop();
        drawOpenGL();
        drawOpenGlGui();

        glfwSwapBuffers(window);
        inputResetKeys();
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}

float *boardPosVec3 = NULL;

void initGlStructures(){
    sprites = malloc(sizeof(Sprite) * (MAX_SPRITES + 1));
    initSpriteRenderer();
    initResources();
    initInput(window);
    initMenu();
    boardPosVec3 = malloc(sizeof(float) * 3);
}



void drawOpenGL(){
    if (lastSpritesCount != spritesCount) {
        sortSprites(sprites, spritesCount);
        lastSpritesCount = spritesCount;
    }
    if (spritesCount > 0) {
        getUniformMat4(srProjMat);
        glm_ortho_default_s(getScreenSize()[0]/(float)getScreenSize()[1], board->sizeX/2.0f, srProjMat);
        glm_translate(srProjMat, boardPosVec3);
        for (int i = 0; i < spritesCount; ++i) {
            drawSprite(sprites[i]);
        }
    }
}

void drawOpenGlGui(){
    getUniformMat4(srProjMat);
    glm_ortho_default_s(getScreenSize()[0]/(float)getScreenSize()[1], 5.0f, srProjMat);
    menuGuiUpdate();
}

void processInput(GLFWwindow *window) {
    //if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      //  glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}