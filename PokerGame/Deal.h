#pragma once

#include "GlobalConstants.h"
#include "Player.h"

struct Deal
{
	int pot;
	int lastGameRaise;
	int currentMaxRaise;
	bool dealFlag;
};

GameCondition DealLoop(Player[], Deal&);
bool DealStart(Player[], Deal&);
void DealPlay(Player[], Deal&);
void DeterminingWinner(Player[], Deal&);
void ClearDeal(Deal&);