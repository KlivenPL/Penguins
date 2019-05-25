//
// Created by oskar on 18.04.2019.
//

#include "../Header Files/Menu.h"
#include "../Header Files/GUI.h"
#include "../Header Files/Utilities.h"
#include "../Header Files/Penguins.h"
#include "../Header Files/Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Header Files/GameWindow.h"
typedef enum MenuAvailableSubstates {substatesNone, localOptions, onlineOptions, escapeMenu} MenuAvailableSubstates;

MenuAvailableStates menuState = MENU_AVAILABLE_STATES_MAIN_MENU;
MenuAvailableSubstates menuSubstate = substatesNone;

Sprite *robotSprite = NULL;
Sprite *humanSprite = NULL;
Sprite *playSprite = NULL;
int menuPlayerCount = 2;
char menuPlayerNames[4][16];
bool menuPlayerCtrlLoc[4];

float backgroundColor[3];

void displayMainMenu();
void displayGameInfo();

Sprite *skins[4][4];
int choosenSkins[4];


void initMenu(){
    robotSprite = createNewSprite((vec2){0,0}, (vec2){1,1}, 0, resGetTexture(AVAILABLE_TEXTURES_ROBOT), NULL, false);
    humanSprite = createNewSprite((vec2){0,0}, (vec2){0.9f,1}, 0, resGetTexture(AVAILABLE_TEXTURES_HUMAN), NULL, false);
    playSprite = createNewSprite((vec2){0,0}, (vec2){1,1}, 0, resGetTexture(AVAILABLE_TEXTURES_PLAY), NULL, false);

    for (int i = 0; i < 4; ++i) {
        sprintf(menuPlayerNames[i], "Player %d", i + 1);
        menuPlayerCtrlLoc[i] = i%2==0;
    }

    for (int i = 0; i < 4; ++i) {
        choosenSkins[i] = i;
        for (int j = 0; j < 4; ++j) {
            skins[i][j] = createNewSprite((vec2){0, 0}, (vec2){2/5.0f * 2, 3/5.0f * 2}, 0, resGetTexture(j), NULL, false);
        }
    }

    //sprintf(menuPlayerNames[0], "OOOOOOOOOOOOOOO");

}

void menuGuiUpdate(){
    switch (menuState){
        case MENU_AVAILABLE_STATES_NONE:
            break;
        case MENU_AVAILABLE_STATES_MAIN_MENU:
            displayMainMenu();
            break;
        case MENU_AVAILABLE_STATES_GAME:
            displayGameInfo();
    }
}

void displayMainMenu(){
    guiDrawBox(0, 0, 1920, 1080, backgroundColor);
    guiDrawText("Penguins", (vec2) {1920 / 2 - 260, 120}, (vec2) {1, 1},
                (vec3) {1 - backgroundColor[0], 1 - backgroundColor[1], 1 - backgroundColor[2]}, NULL);

    if (menuSubstate == substatesNone) {
        for (int i = 0; i < 2; ++i) {
            if (guiDrawButtonBox(1920 / 2 - 200, 1080 / 3 + i * 120, 400, 100,
                                 (vec3) {(1 - backgroundColor[0]) * 0.75f, (1 - backgroundColor[1]) * 0.75f,
                                         (1 - backgroundColor[2]) * 0.75f})) {
                menuSubstate = i + 1;
            }
            if (i == 0)
                guiDrawText("Play locally", (vec2) {1920 / 2 - 200 + 40, 1080 / 3 + 30 + i * 120}, (vec2) {0.5f, 0.5f},
                            (vec3) {1.25f - backgroundColor[0], 1.25f - backgroundColor[1], 1.25f - backgroundColor[2]},
                            NULL);
            else if (i == 1)
                guiDrawText("Online multiplayer", (vec2) {1920 / 2 - 200 + 25, 1080 / 3 + 40 + i * 120},
                            (vec2) {0.35f, 0.35f},
                            (vec3) {1.25f - backgroundColor[0], 1.25f - backgroundColor[1], 1.25f - backgroundColor[2]},
                            NULL);
        }
    } else if (menuSubstate == localOptions){
        if (guiDrawButtonBox(1920/2 - 120 - 20, 1080 - 1080/7, 330, 100, backgroundColor)){
            menuSubstate = substatesNone;
            menuState = MENU_AVAILABLE_STATES_GAME;

            createNewGame(menuPlayerCount, menuPlayerCtrlLoc, (char[4][6]){0}, menuPlayerNames, 2, menuPlayerCount == 3 ? 15 : menuPlayerCount == 4 ? 20 : 10, menuPlayerCount == 3 ? 15 : menuPlayerCount == 4 ? 20 : 10);
            spawnBoard();
            boardPosVec3[0] = -board->fields[board->sizeX-1][board->sizeY-1]->sprite->position[0]/1.5f - 0.5f;
            boardPosVec3[1] = -board->fields[board->sizeX-1][board->sizeY-1]->sprite->position[1]/2.0f + 0.5f;
            boardPosVec3[2] = 0;
        }
        guiDrawSprite(playSprite, 1920/2 - 120 - 20, 1080 - 1080/7, 100, 100, (vec3) {1 - backgroundColor[0], 1 - backgroundColor[1], 1 - backgroundColor[2]});
        guiDrawText("PLAY", (vec2){1920/2 - 20, 1080 - 1080/7 + 20}, (vec2){0.75f, 0.75f}, (vec3) {1 - backgroundColor[0], 1 - backgroundColor[1], 1 - backgroundColor[2]}, NULL);
        for (int i = 0; i <= menuPlayerCount; ++i) {
            if (i == menuPlayerCount){
                if (menuPlayerCount < 4) {
                    if (guiDrawButtonBox(1920 / 2 - 37, 1080 / 3 - 37 - 20 + 170 * i, 75, 75, (vec3) {backgroundColor[0] + 0.25f, backgroundColor[1] + 0.25f, backgroundColor[2] + 0.25f}))
                        menuPlayerCount++;
                    guiDrawText("+", (vec2) {1920 / 2 - 37 + 8, 1080 / 3 - 43 - 20+ 170 * i}, (vec2) {1.0f, 1.0f}, NULL, NULL);
                }
                continue;
            }
            guiDrawBox(1920/3, 1080/3 + 150 * i, 100, 100, NULL);
            guiDrawSprite(skins[i][choosenSkins[i]], 1920/3 + 15, 1080/3 + 150 * i, 2/5.0f * 100 * 1.6f, 3/5.0f * 100 * 1.6f, NULL);

            if (guiDrawButtonBox(1920/3 - 60, 1080/3 + 25 + 150 * i, 50, 50, (vec3){backgroundColor[0] + 0.25f, backgroundColor[1] + 0.25f, backgroundColor[2] + 0.25f}))
                choosenSkins[i] = choosenSkins[i] - 1 < 0 ? 3 : choosenSkins[i] - 1;
            if(guiDrawButtonBox(1920/3 + 110, 1080/3 + 25 + 150 * i, 50, 50,  (vec3){backgroundColor[0] + 0.25f, backgroundColor[1] + 0.25f, backgroundColor[2] + 0.25f}))
                choosenSkins[i] = (choosenSkins[i] + 1) % 4;
            guiDrawText("<", (vec3){1920/3 - 60 + 10, 1080/3 + 25 + 3 + 150 * i}, (vec3){0.5f,0.5f}, NULL, NULL);
            guiDrawText(">", (vec3){1920/3 + 110 + 15, 1080/3 + 25 + 3 + 150 * i}, (vec3){0.5f,0.5f}, NULL, NULL);

            guiDrawSprite(humanSprite, 1920 - 1920/3.1f, 1080/3 + 150 * i, 60 * 0.9f, 60, NULL);
            if (guiDrawButtonBox(1920 - 1920/3.1f + 20, 1080/3 + 75 + 150 * i, 20, 20, NULL))
                menuPlayerCtrlLoc[i] = true;
            guiDrawSprite(robotSprite, 1920 - 1920/3.1f + 80, 1080/3 + 150 * i, 60, 60, NULL);
            if (guiDrawButtonBox(1920 - 1920/3.1f + 80 + 20, 1080/3 + 75 + 150 * i, 20, 20, NULL))
                menuPlayerCtrlLoc[i] = false;

            guiDrawBox((menuPlayerCtrlLoc[i] ? 1920 - (1920/3.1f - 22) : 1920 - 1920/3.1f + 80 + 22), 1080/3 + 77 + 150 * i, 16, 16, (vec3){0,0,0});

            if (guiOnClick(humanSprite))
                menuPlayerCtrlLoc[i] = true;

            if (guiOnClick(robotSprite))
                menuPlayerCtrlLoc[i] = false;

            guiDrawText(menuPlayerNames[i], (vec2){1920/3 + 180, 1080/3 + 30 + 150 * i}, (vec2){0.4f, 0.4f}, NULL, NULL);
        }
    }

}

// made by Hytham
void displayGameInfo(){
    float *boxPos = (vec2){1920 - 1920/4.25f,0};
    // here we are displaying in-game information about which player is playing now, scores
    guiDrawBox(boxPos[0], boxPos[1],1920/4.25f,1080, (vec3){backgroundColor[0] + 0.1f,backgroundColor[1] + 0.1f,backgroundColor[2] + 0.1f});

    //guiDrawText("Penguins V0.5", (vec2){1920 - 1920/4.25f + 20,20}, (vec3){1.0f/2, 1.0f/2, 1.0f}, hexColor("f2af3a"), NULL);
    //guiDrawText("KlivenINC, 2019", (vec2){1920 - 1920/4.25f + 20,100}, (vec3){1.0f/2.5f, 1.0f/2.5f, 1.0f}, hexColor("f2af3a"), NULL);

    guiDrawText("Currently playing:", (vec2) {boxPos[0] + 20, 50}, (vec2){0.35f, 0.35f}, NULL, NULL);
    // activePlayerName
    guiDrawText(menuPlayerNames[turn%playersCount], (vec2) {boxPos[0] + 20, boxPos[1] + 100}, (vec2) {.33f, .33f}, NULL, NULL);

    guiDrawText("Scores:", (vec2) {boxPos[0] + 20, 200}, (vec2){0.4f, 0.4f}, NULL, NULL);
    // all players in game
    for(int i = 0; i < playersCount; i++) {
        char scoreChar[4];
        sprintf(scoreChar, "%d", players[i]->score);//Pos[0], boxPos[1] + i*50}, (vec2) {.25f, .25f}, (vec3) {216, 30, 201}, NULL);
        //name of i'th player
        guiDrawText(menuPlayerNames[i], (vec2) {boxPos[0] + 20, boxPos[1] + 275 + i*60}, (vec2) {.3f, .3f}, NULL, NULL);
        guiDrawText(scoreChar, (vec2) {1920 - 75, boxPos[1] + 275 + i*60}, (vec2) {.3f, .3f}, NULL, NULL);
    }

    if (inputGetKeyDown(GLFW_KEY_ESCAPE))
        menuSubstate = menuSubstate == escapeMenu ? substatesNone : escapeMenu;

    // the escape menu
    if (menuSubstate == escapeMenu){
        guiDrawBox(1920/2 - 1920/2.5f/2, 1080/2 - 1080/2.5f/2, 1920/2.5f, 1080/2.5f, (vec3){backgroundColor[0] + 0.1f,backgroundColor[1] + 0.1f,backgroundColor[2] + 0.1f});
        if (guiDrawButtonBox(1920/2 - 1920/2.5f/2 + 200, 1080/2 - 1080/2.5f/2 + 100, 400, 100, (vec3){backgroundColor[0] + 0.2f,backgroundColor[1] + 0.2f,backgroundColor[2] + 0.2f})){
            glfwSetWindowShouldClose(window, true);
        }
        guiDrawText("Exit", (vec2){1920/2 - 1920/2.5f/2 + 200 + 140, 1080/2 - 1080/2.5f/2 + 100 + 25}, (vec2){0.5f, 0.5f}, NULL, NULL);
    }
}
