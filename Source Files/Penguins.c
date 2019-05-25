#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include "../Header Files/Penguins.h"
#include "../Header Files/Utilities.h"
#include "../Header Files/Structures.h"
#include <stdbool.h>
#include <uchar.h>
#include "../Header Files/GameWindow.h"
#include "cglm.h"
#include "../Header Files/Input.h"
#include "../Header Files/Physics.h"
#include "../Header Files/Movement.h"
#include "../Header Files/GUI.h"
#include "../Header Files/Menu.h"
#include <sys/time.h>


//#define INTERACTIVE   //Uncomment this line to compile an Interactive mode.

const char *APP_ID="kliveninc";
void draw();
int loadGame(GameStage stage, int penguinsPlacementCount, char *inputBoardFilePath);
int saveGame(char outputFilePath[FILENAME_MAX]);
int makeMyMove(int placementPenguinsCount, Player *_player);
int playAutonomously(GameStage tmpGameStage, char inputBoardFile[FILENAME_MAX], int placementPenguinsCount);
void destroyField(Field *field);
void placePenguin(Penguin *penguin, Field *field);

GameStage gameStage;

Board *board;
int playersCount;
Player **players; //one-dimensional array of pointers to players

Player *activePlayer;
Penguin *activePenguin;
int turn = -1;

char outputBoardFile[FILENAME_MAX];


bool checkIfPlayerAlive(Player *player){
    int deadPenguins = 0;
    for (int i = 0; i < player->penguinsCount ; ++i) {
        if (!player->penguins[i]->alive)
            deadPenguins++;
    }
    if (deadPenguins == player->penguinsCount)
        return false;

    return true;
}

int getAlivePlayersCount(){
    int count = 0;
    for (int i = 0; i < playersCount; ++i) {
        if (checkIfPlayerAlive(players[i]))
            count++;
    }
    return count;
}

void nextTurn(){
//    if (activePlayer)
//        activePlayer->activePenguinId = (activePlayer->activePenguinId + 1) % activePlayer->penguinsCount;
    if (playersCount == 0)
        return;
    activePlayer = players[++turn%playersCount];
    activePenguin = NULL;

#ifdef INTERACTIVE
    if (getAlivePlayersCount() <= 1){
        printf("GAME OVER");
        gameStage = gameOverGS;
        return;
    }
#endif

    for(int i = 0; i < playersCount; i++) {
        for(int j = 0; j < players[i]->penguinsCount; j++) {
            Penguin *penguin = players[i]->penguins[j];
            if (penguin->alive == false)
                continue;

            if (penguin->field == NULL)
                continue;

            bool isBlockedByOther = false;
            Field **avFields = getAvailableFields(penguin, &isBlockedByOther);
#ifdef INTERACTIVE
            if (isBlockedByOther)
                continue;
#endif
            if (avFields[0] != NULL) {
                free(avFields);
                continue;
            }
            if (penguin->sprite)
                glm_vec3_copy((vec3) {1, 0, 0}, penguin->sprite->color);

            //activePlayer->score += penguin->field->fishCount;
            destroyField(penguin->field);

            penguin->alive = false;
            free (avFields);
        }
    }
}

/*Penguin *getActivePenguin(){
    if (!activePlayer)
        return NULL;

    Penguin *penguin = activePlayer->penguins[activePlayer->activePenguinId];
    if (penguin->alive)
        return penguin;


    while (true){
        if (activePlayer->penguins[activePlayer->activePenguinId = (activePlayer->activePenguinId + 1) % activePlayer->penguinsCount])
            return activePlayer->penguins[activePlayer->activePenguinId];
    }
}*/

Penguin *getActivePenguin(){
    return activePenguin;
}

bool checkIfPenguinAvailable(Penguin *penguin, bool *outBlockedByOtherPenguin){
    outBlockedByOtherPenguin[0] = false;
    if (penguin->alive == false)
        return false;
    Field **avFields = getAvailableFields(penguin, outBlockedByOtherPenguin);
    if (avFields[0] == NULL)
        return false;
    return true;
}

Player *getActivePlayer(){
    return activePlayer;
};


int main(int paramsCount, char **params) {
    srand(time(NULL));

#ifdef INTERACTIVE
    glCreateWindow();
    return 0;
#endif


    /*phase=phase_mark – phase_mark can take value placement or movement
     penguins=N, where N is a number of penguins that player has; this parameter is only used if phase=placement;
     inputboardfile – name of the file that contains current game state
     outputboardfile – name of the file in which the new game state should be stored
     id – request to display player’s ID end exit the program.*/

    if (paramsCount == 1) {
        printf("Not enough parameters!");
        return 3;
    }

    GameStage tmpGameStage;
    int tmpPenguins = 0;
    char inputBoardFile[FILENAME_MAX];

    inputBoardFile[0] = '\0';
    outputBoardFile[0] = '\0';

    if (strcmp(params[1], "id") == 0) {
        printf("%s", APP_ID);
        return 0;
    }

    for (int i = 1; i < paramsCount; ++i) {
        if (strstr(params[i], "phase") != NULL){
            if (strstr(params[i], "placement") != NULL)
                tmpGameStage = placementGS;
            else if (strstr(params[i], "movement") != NULL)
                tmpGameStage = playingGS;
            else {
                printf("Incorrect parameter: %s", params[i]);
                return 2;
            }
        } else if (strstr(params[i], "penguins") != NULL) {
            char *ptr = params[i] + 9;
            int tmpPeng = atoi(ptr);
            if (tmpPeng == 0){
                printf("Incorrect parameter: %s", params[i]);
                return 2;
            }
            tmpPenguins = tmpPeng;
        } else {
            if (inputBoardFile[0] == '\0')
                strcpy(inputBoardFile, params[i]);
            else if (outputBoardFile[0] == '\0')
                strcpy(outputBoardFile, params[i]);
        }
    }

    return playAutonomously(tmpGameStage, inputBoardFile, tmpPenguins);

}

void createNewGame(int playersCount, bool *controlledLocally, char ids[4][6], char names[4][16], int penguinsCount, int boardSizeX, int boardSizeY){
    Player **_players = malloc(sizeof(Player) * playersCount);
    for (int i = 0; i < playersCount; ++i) {
        _players[i] = createNewPlayer(names[i], i, ids ? ids[i] : NULL, controlledLocally[i]);
        addPenguinsToPlayer(_players[i], penguinsCount);
    }
    Board *_board = createRandomBoard(boardSizeX, boardSizeY);

    initGame(_board, _players, playersCount, placementGS);
}

int playAutonomously(GameStage tmpGameStage, char inputBoardFile[FILENAME_MAX], int placementPenguinsCount){
    int retCode = 0;
    retCode = loadGame(tmpGameStage, placementPenguinsCount, inputBoardFile);
    if (retCode != 0)
        return retCode;
   // draw();
    retCode = makeMyMove(placementPenguinsCount, NULL);
    if (retCode != 0)
        return retCode;
    //draw();
    retCode = saveGame(outputBoardFile);
    return retCode;
}

int loadGame(GameStage stage, int penguinsPlacementCount, char *inputBoardFilePath){

    FILE *file;
    file = fopen(inputBoardFilePath, "r");

    if (file == NULL){
        printf("Could not open file: %s\n", inputBoardFilePath);
        return 2;
    }

    int sizeY = 0, sizeX = 0;
    /*while(fscanf(file, "%[^\n]%*c", linia) != EOF) {
        sscanf
    }*/

    fscanf(file, "%d %d", &sizeY, &sizeX);

    if (sizeY <= 0 || sizeX <= 0){
        fclose(file);
        printf("Board size must be positive (m >= 0 && n >= 0)");
        return 2;
    }

    Board *tmpBoard = createEmptyBoard(sizeX, sizeY);
    Player *tmpPlayers[10];
    int tmpPenguinsXs[100], tmpPenguinsYs[100];

    for (int i = 0; i < 100; ++i) {
        tmpPenguinsXs[i] = -1;
        tmpPenguinsYs[i] = -1;
    }

    for (int k = 0; k < 10; ++k) {
        tmpPlayers[k] = NULL;
    }
    //char ***rawBoard = malloc(sizeof(char) * m);

    //m = sizeY
    //n = sizeX
    for (int y = 0; y < sizeY; ++y) {
        //rawBoard[i] = malloc(sizeof(char) * n);
        for (int x = 0; x < sizeX; ++x) {
            //rawBoard[i][j] = malloc(sizeof(char) * 3);
            char fieldStr[3];
            fscanf(file, "%s", fieldStr);

            if (atoi(fieldStr) != 0){
                tmpBoard->fields[y][x]->fieldType = iceFloeFT;
                char c1[2], c2[2];
                c1[0] = fieldStr[0];
                c1[1] = '\0';

                c2[0] = fieldStr[1];
                c2[1] = '\0';

                int fishCount = atoi(c1);
                if (fishCount > 3){
                    fclose(file);
                    printf("Cannot place %d fish on the field (x = %d, y = %d).",  fishCount, x, y);
                    return 2;
                }
                tmpBoard->fields[y][x]->fishCount = fishCount;

                int playerInt = atoi(c2);

                if (playerInt > 0){
                    if (tmpPlayers[playerInt - 1] == NULL) {
                        tmpPlayers[playerInt - 1] = createNewPlayer("Player", playerInt, "iksde", true);
                        tmpPlayers[playerInt - 1]->penguinsCount = 0;
                    }
                    tmpPenguinsXs[(playerInt - 1) * 10 + (tmpPlayers[(playerInt - 1)]->penguinsCount)] = x;
                    tmpPenguinsYs[(playerInt - 1) * 10 + (tmpPlayers[(playerInt - 1)]->penguinsCount)] = y;
                    tmpPlayers[(playerInt - 1)]->penguinsCount++;
                    tmpBoard->fields[y][x]->fishCount = 0;
                }
            }
        }
    }


    char line[10000];
    for(int i = 0; (fscanf(file, "%[^\n]%*c", line) != EOF); i++) {
        char playerIdStr[1000];
        fscanf(file, "%s", playerIdStr);
        int playerId;
        fscanf(file, "%d", &playerId);
        fscanf(file, "%d", &tmpPlayers[playerId - 1]->score);

        int tmpIdStrLength = 0;
        for (int i = 0; i < playerIdStr[i]; ++i) {
            tmpIdStrLength++;
        }
        tmpPlayers[playerId - 1]->idStr = malloc(sizeof(char) * (tmpIdStrLength + 1));
        //tmpPlayers[playerId - 1]->id = playerId;
        strcpy(tmpPlayers[playerId - 1]->idStr,playerIdStr);
    }

    fclose(file);

    int tmpPlayersCount = 0;
    for (int i = 0; tmpPlayers[i] != NULL; ++i) {
        tmpPlayersCount++;
        int placedPenguinsCount = tmpPlayers[i]->penguinsCount;
        addPenguinsToPlayer(tmpPlayers[i], (stage == placementGS ? penguinsPlacementCount : tmpPlayers[i]->penguinsCount));
        for (int j = 0; j < placedPenguinsCount; ++j) {
           // printf("%d", tmpBoard->fields[tmpPenguinsMs[i * 10 + j]][tmpPenguinsNs[i * 10 + j]]->posX == 1);
            tmpPlayers[i]->penguins[j]->field = tmpBoard->fields[tmpPenguinsYs[i * 10 + j]][tmpPenguinsXs[i * 10 + j]];
        }
    }

    Player **tmpPlayersFinal = malloc(sizeof(Player) * (tmpPlayersCount + 1));
    /*for (int i = 0; i < tmpPlayersCount; ++i) {
        tmpPlayersFinal[i] = &tmpPlayers[i];
    }*/
    memcpy(tmpPlayersFinal, tmpPlayers, sizeof(Player) * (tmpPlayersCount + 1));
    initGame(tmpBoard, tmpPlayersFinal, tmpPlayersCount, stage);
    return 0;

   // }else {

   // }
}

int saveGame(char outputFilePath[FILENAME_MAX]){
    FILE *file = fopen(outputFilePath, "w");
    if (!file){
        printf("Could not open file %s to save the game!", outputFilePath);
        return 2;
    }

    fprintf(file, "%d %d\n", board->sizeY, board->sizeX);
    for (int y = 0; y < board->sizeY; ++y) {
        for (int x = 0; x < board->sizeX; ++x) {
            Field *field = board->fields[y][x];
            bool penguin = false;
            for (int i = 0; i < playersCount; ++i) {
                for (int k = 0; k < players[i]->penguinsCount; ++k) {
                    if (players[i]->penguins[k]->field == field) {
                        fprintf(file, "%d%d ", board->fields[y][x]->fishCount, players[i]->id);
                        penguin = true;
                    }
                }
            }
            if (penguin)
                continue;

            if (field->fieldType == iceFloeFT) {
                fprintf(file, "%d%d ", board->fields[y][x]->fishCount, 0);

            } else if (field->fieldType == emptyFT)
                fprintf(file, "%d%d ", 0, 0);
        }
        fprintf(file, "\n");
    }
    //Player *myPlayer = findMyPlayer();
    for (int i = 0; i < playersCount; ++i) {
        fprintf(file, "%s %d %d\n", players[i]->idStr, players[i]->id, players[i]->score);
    }
    fclose(file);
    return 0;
}

void initGame(Board *_board, Player **_players, int _playersCount, GameStage _gameStage){
    gameStage = _gameStage;
    board = _board;
    players = _players;
    playersCount = _playersCount;

    nextTurn();
}

int makeMyMove(int placementPenguinsCount, Player *_player){
    if (_player == NULL) { // does not happen in interactive mode
        if (findMyPlayer() == NULL) {
            players[playersCount] = createNewPlayer((char *) APP_ID, playersCount + 1, (char *) APP_ID, true);
            addPenguinsToPlayer(players[playersCount++], placementPenguinsCount);
        }
    }
    Player *myPlayer = _player == NULL ? findMyPlayer() : _player;

    if (gameStage == placementGS) {
        Field **avFields = getAvailableFields(NULL, NULL);
        if (avFields[0] == NULL) {
            printf("No enough places to place another penguin!");
            return 3;
        }
        int avCount = 0;
        for (int i = 0; avFields[i] != NULL; ++i) {
            avCount++;
        }
        Field *placeField = NULL;
        while (placeField == NULL){
            placeField = avFields[randInt(0, avCount)];
        }
        for (int i = 0; i < myPlayer->penguinsCount; ++i) {
            if (myPlayer->penguins[i]->field == NULL) {
                placePenguin(myPlayer->penguins[i], placeField);
                //nextTurn();
                return 0;
            }
        }
        //printf("All kliveninc penguins have been placed, cannot make move.");
        return 1;
    } else {
        Penguin *myPenguin = NULL;

        if (checkIfPlayerAlive(myPlayer) == false)
            return 1;
        bool blockedByOther = false;
        Field **avFields = NULL;
        int randId = 0;

        while (myPenguin == NULL || myPenguin->alive == false) {
            randId = randInt(0, myPlayer->penguinsCount);
            myPenguin = myPlayer->penguins[randId];
        }

        avFields = getAvailableFields(myPenguin, &blockedByOther);
        Field *placeField = NULL;
        int avCount = 0;
        for (int i = 0; avFields[i] != NULL; ++i) {
            avCount++;
        }
        if (randId == 0) {
            int tmpMax = 0;
            for (int i = 0; i < avCount; ++i) {
                //
                if (avFields[i]->fishCount > tmpMax) {
                    placeField = avFields[i];
                    tmpMax = avFields[i]->fishCount;
                    if (tmpMax == 3)
                        break;
                }
            }
        } else if (randId == 1){
            placeField = avFields[avCount >= 4 ? randInt(0, 4) : 0];
        } else {
            placeField = avFields[randInt(0, avCount)];
        }

        if (_player)
            destroyField(myPenguin->field);
        placePenguin(myPenguin, placeField);
        return 0;
    }
}

Player *findMyPlayer(){
    for (int i = 0; i < playersCount; ++i) {
        if (strcmp(players[i]->idStr, APP_ID) == 0)
            return players[i];
    }
    return NULL;
}


void spawnBoard(){
    float offsetX = 0, offsetY = 0;
    float tileHalfSizeX = 0.7f;
    float tileHalfSizeY = 0.35f;
    for (int j = 0; j < board->sizeY; ++j) {
        for (int i = 0; i < board->sizeX; ++i) {
            float posX = offsetX + (i) * tileHalfSizeX;
            float posY = offsetY - (i) * tileHalfSizeY;
            AvailableTextures tex;
            if (board->fields[i][j]->fishCount == 1)
                tex = AVAILABLE_TEXTURES_ICE_FLOE1;
            else if (board->fields[i][j]->fishCount == 2)
                tex = AVAILABLE_TEXTURES_ICE_FLOE2;
            else if (board->fields[i][j]->fishCount == 3)
                tex = AVAILABLE_TEXTURES_ICE_FLOE3;
            board->fields[i][j]->sprite = createNewSprite((vec2){posX, posY}, (vec2){1,1}, 2+(int)((board->sizeY+posY)*10.0f), resGetTexture(tex), NULL, true);
            if (board->fields[i][j]->fieldType == emptyFT)
                board->fields[i][j]->sprite->visible = false;
            // printf("(%d, %d) x: %f y: %f\n", i, j, board->fields[i][j]->sprite->position[0], board->fields[i][j]->sprite->position[1]);
        }
        offsetX += tileHalfSizeX;
        offsetY += tileHalfSizeY;
    }



    /*cursor = malloc(sizeof(Cursor));
    cursor->posX = 0;
    cursor->posY = 0;*/
}

void boardMovement(){
    for (int i = 0; i < board->sizeY; ++i) {
        for (int j = 0; j < board->sizeX; ++j) {
            glm_vec3_copy(((vec3){1,1,1}), board->fields[i][j]->sprite->color);
            if (board->fields[i][j]->randMovement == -2137)
                board->fields[i][j]->randMovement = board->fields[i][j]->posY*0.75f + randFloat(-.25f, .25f);

            if (board->fields[i][j]->sprite->visible && board->fields[i][j]->fieldType == emptyFT)
                board->fields[i][j]->sprite->position[1]+=(board->fields[i][j]->randMovement-= 9.81f * fixedDeltaTime * fixedDeltaTime);
            else
                board->fields[i][j]->sprite->position[1]+=((float)cos(board->fields[i][j]->randMovement+=fixedDeltaTime))/3000.0f;
        }
    }
}

Sprite *spawnPenguin(Penguin *penguin, Texture *skin){
    if (penguin->sprite){
        printf("That penguin is already spawned!");
        return NULL;
    }
    penguin->sprite = createNewSprite((vec2){0, 0}, (vec2){2/5.0f * 2, 3/5.0f * 2}, 0, skin, NULL, true);
    return penguin->sprite;
}

void penguinUpdate(Penguin *penguin){
    if (penguin->sprite) {
        glm_vec3_dup(((vec3) {penguin->field->sprite->position[0] + 0.1f, penguin->field->sprite->position[1] + 0.9f, 1}), penguin->sprite->position);
    }
}

void placementUpdate(){
    bool allPlaced = true;
    for (int i = 0; i < playersCount; ++i) {
        int tmpPnguinCount = 0;
        for (int j = 0; j < players[i]->penguinsCount && players[i]->penguins[j]->sprite; ++j)
            tmpPnguinCount++;

        if (tmpPnguinCount != players[i]->penguinsCount) {
            allPlaced = false;
            break;
        }

    }
    if (allPlaced){
        gameStage = playingGS;
        return;
    }
    if (getActivePlayer()->controlledLocally == false){
        int arrId = 0;
        for (int i = 0; activePlayer->penguins[i]->sprite; ++i)
            arrId++;

        spawnPenguin(activePlayer->penguins[arrId], skins[activePlayer->id][choosenSkins[activePlayer->id]]->texture);

        makeMyMove(2, getActivePlayer());
        nextTurn();
        return;
    }
    Field **avFields = getAvailableFields(NULL, NULL);
    //if (getActivePlayer()->controlledLocally == false)
    vec3 hitPoint;
    Sprite *hitSprite = NULL;
    bool fieldIsAvailable = false;
    physicsMouseRaycast(inputMousePos[0], inputMousePos[1], hitPoint, &hitSprite);
    for (int i = 0; avFields[i]!=NULL; ++i) {
        vec3 green = {0.5,1,0.5};
        vec3 yellow = {1,1,0.5};
        glm_vec3_copy((hitSprite == avFields[i]->sprite ? yellow : green), avFields[i]->sprite->color);
        if (fieldIsAvailable == false)
            fieldIsAvailable = hitSprite == avFields[i]->sprite;
    }
    free(avFields);

    if (fieldIsAvailable && inputGetMouseKeyDown(GLFW_MOUSE_BUTTON_1)){
        int arrId = 0;
        for (int i = 0; activePlayer->penguins[i]->sprite; ++i)
            arrId++;

        spawnPenguin(activePlayer->penguins[arrId], skins[activePlayer->id][choosenSkins[activePlayer->id]]->texture);
        Field *placeField = NULL;
        for (int i = 0; i < board->sizeY; ++i)
            for (int j  = 0; j < board->sizeX; ++j)
                if (board->fields[i][j]->sprite == hitSprite)
                    placeField  = board->fields[i][j];

        placePenguin(activePlayer->penguins[arrId], placeField);
        nextTurn();
    }
}

void playingUpdate(){
    vec3 hitPoint;
    Sprite *hitSprite = NULL;
    bool blockedByOther = false;
    if (getActivePenguin() == NULL){
        if (checkIfPlayerAlive(activePlayer) == false) {
            nextTurn();
            return;
        }

        bool skipsRound = true;
        for (int j = 0; j < getActivePlayer()->penguinsCount; ++j) {
            if (checkIfPenguinAvailable(getActivePlayer()->penguins[j], &blockedByOther)) {
                skipsRound = false;
                break;
            }
        }
        if (skipsRound) {
            nextTurn();
            return;
        }

        if (getActivePlayer()->controlledLocally == false){
            makeMyMove(2, getActivePlayer());
            nextTurn();
            return;
        }

        physicsMouseRaycast(inputMousePos[0], inputMousePos[1], hitPoint, &hitSprite);
        if (inputGetMouseKeyDown(GLFW_MOUSE_BUTTON_1)) {
            for (int j = 0; j < getActivePlayer()->penguinsCount; ++j) {
                if (hitSprite == getActivePlayer()->penguins[j]->sprite &&
                    checkIfPenguinAvailable(getActivePlayer()->penguins[j], &blockedByOther)) {
                    activePenguin = getActivePlayer()->penguins[j];
                    return;
                }
            }
        }
        return;
    }
    Field **avFields = getAvailableFields(getActivePenguin(), &blockedByOther);
    bool fieldIsAvailable = false;
    physicsMouseRaycast(inputMousePos[0], inputMousePos[1], hitPoint, &hitSprite);
    for (int i = 0; avFields[i]!=NULL; ++i) {
        vec3 green = {0.5,1,0.5};
        vec3 yellow = {1,1,0.5};
        glm_vec3_copy((hitSprite == avFields[i]->sprite ? yellow : green), avFields[i]->sprite->color);
        if (fieldIsAvailable == false)
            fieldIsAvailable = hitSprite == avFields[i]->sprite;
    }
    free(avFields);

    if (fieldIsAvailable && inputGetMouseKeyDown(GLFW_MOUSE_BUTTON_1)) {
        Field *placeField = NULL;
        for (int i = 0; i < board->sizeY; ++i) {
            for (int j = 0; j < board->sizeX; ++j)
                if (board->fields[i][j]->sprite == hitSprite)
                    placeField = board->fields[i][j];
        }

        destroyField(getActivePenguin()->field);
        placePenguin(getActivePenguin(), placeField);
        nextTurn();
    }
}

void placePenguin(Penguin *penguin, Field *field){
    penguin->field = field;

    for (int i = 0; i < playersCount; ++i) {
        for (int j = 0; j < players[i]->penguinsCount; ++j) {
            if (players[i]->penguins[j] == penguin){
                players[i]->score += penguin->field->fishCount;
                penguin->field->fishCount = 0;
                break;
            }
        }
    }
}

void destroyField(Field *field){
    field->randMovement = 0;
    field->fieldType = emptyFT;
}


/*void input() {
    int origX = cursor->posX;
    int origY = cursor->posY;
    if (inputGetKey(GLFW_KEY_W)){
        cursor->posY++;
    } else if (inputGetKey(GLFW_KEY_S)){
        cursor->posY--;
    } else if (inputGetKey(GLFW_KEY_A)){
        cursor->posX--;
    } else if (inputGetKey(GLFW_KEY_D)) {
        cursor->posX++;
    }
    if (!checkField(cursor->posX, cursor->posY)){
        cursor->posX = origX;
        cursor->posY = origY;
    }

    Field **avFields = getAvailableFields(players[0]->penguins[0]);

    //{
        vec3 hitPoint;
        Sprite *hitSprite;
        physicsMouseRaycast(inputMousePos[0], inputMousePos[1], hitPoint, &hitSprite);

   // }
    for (int i = 0; avFields[i]!=NULL; ++i) {
        vec3 green = {0.5,1,0.5};
        vec3 yellow = {1,1,0.5};
       //glm_vec3_copy(((cursor->posY == avFields[i]->posY && cursor->posX == avFields[i]->posX) ? yellow : green), avFields[i]->sprite->color);
        glm_vec3_copy((hitSprite == avFields[i]->sprite ? yellow : green), avFields[i]->sprite->color);
    }


    //if (inputGetMouseKeyDown(GLFW_MOUSE_BUTTON_1)) {
     //   physicsMouseRaycast(inputMousePos[0], inputMousePos[1], 10);

    //}

    if (inputAnyKey()){
        activePlayer->penguins[0]->field = board->fields[cursor->posX][cursor->posY];

        draw();
    }


}*/


void draw() {
    clearConsole();
    for (int y = -1; y <= board->sizeY; y++) {
        for (int x = -1; x <= board->sizeX; x++) {
            if (x == -1 || y == -1 || x == board->sizeX || y == board->sizeY){
                printf (BG_COLOR_GRAY"  "COLOR_RESET);
                continue;
            }
            Field *field = board->fields[y][x];
            /*if (cursor->posX == x && cursor->posY == y){
                printf(BG_COLOR_BLUE "  " COLOR_RESET);
                continue;
            }*/
            bool penguin = false;
            for (int i = 0; i < playersCount; ++i) {
                for (int k = 0; k < players[i]->penguinsCount; ++k) {
                    if (players[i]->penguins[k]->field == field) {
                        printf(i % 2 == 0 ? BG_COLOR_GREEN "  " : BG_COLOR_YELLOW "  " COLOR_RESET);
                        penguin = true;
                    }
                }
            }
            if (penguin)
                continue;

            if (field->fieldType == iceFloeFT) {
                if (field->fishCount > 0)
                    printf(BG_COLOR_LIGHT_WHITE F_COLOR_BLACK" %d" COLOR_RESET, field->fishCount);
                else
                    printf(BG_COLOR_LIGHT_WHITE "  " COLOR_RESET);

            } else if (field->fieldType == emptyFT)
                printf(BG_COLOR_LIGHT_BLUE"  "COLOR_RESET);
        }
        printf("\n");
    }
}


void gameLoop() {
    if (!board)
        return;
        if (gameStage == placementGS)
            placementUpdate();
        else if (gameStage == playingGS)
            playingUpdate();

}

void fixedGameLoop(){
    if (!board)
        return;
    boardMovement();
    for (int i = 0; i < playersCount; ++i)
        for (int j = 0; j < players[i]->penguinsCount; ++j)
            penguinUpdate(players[i]->penguins[j]);
}

