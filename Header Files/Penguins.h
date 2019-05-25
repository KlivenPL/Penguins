#ifndef PENGUINS_PENGUINS_H
#define PENGUINS_PENGUINS_H
#include "Structures.h"
void gameLoop();
void fixedGameLoop();
typedef enum GameStage{placementGS, playingGS, gameOverGS} GameStage;
void initGame(Board *_board, Player **_players, int _playersCount, GameStage _gameStage);
void createNewGame(int playersCount, bool *controlledLocally, char ids[4][6], char names[4][16], int penguinsCount, int boardSizeX, int boardSizeY);
void spawnBoard();

Penguin *getActivePenguin();
Player *getActivePlayer();
Player *findMyPlayer();
int turn;

Board *board;
Player **players;
int playersCount;
#endif //PENGUINS_PENGUINS_H
