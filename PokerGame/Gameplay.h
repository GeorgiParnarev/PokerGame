#pragma once

#include "GlobalConstants.h"
#include "Card.h"
#include "Player.h"

bool IsPlayerInGame(player_condition_type);
int ActivePlayersCount(Player[]);
int ActivePlayersInDealCount(Player[]);
void ActualizePlayers(Player[]);
void GameInitPlayers(Player[], int);

void GameClear(Player[]);
int GameSetPlayersNum();
FileCondition GameSaveToFile(Player[]);
GameCondition GameLoop(Player[]);
FileCondition GameReadFromFile(Player[]);
void GameChoisNewGame(Player[]);
void GameRun();