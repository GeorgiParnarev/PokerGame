#pragma once

#include "GlobalTypes.h"
#include "GlobalConstants.h"
#include "Card.h"

struct Player
{
	char cardsDisplay[CARDS_DISPLAY_MAX_SIZE];
	int chips;							///< The amount of chips the player has.
	int points;							///< The player's current points.
	int lastRaise;
	player_condition_type isPlayerActive;
};

/// <summary>
/// Initializes a player object with default values for an empty player.
/// </summary>
/// <param name="player">The player to initialize.</param>
void InitEmptyPlayer(Player&);

int CalculatePoints(Card[]);

bool ThreeSevens(Card[]);

bool ThreeOfKind(Card[]);

bool ThreeOfSameSuit(Card[]);

bool PairOfAces(Card[]);

bool PairOfSevens(Card[]);

bool PairWithSevenOfClubs(int&, Card[]);

bool PairOfSuits(int&, Card[]);

int CalcHighCard(Card[]);

bool CardsShareSameKind(const Card, const Card);

bool CardsShareSameSuit(const Card, const Card);

bool IsPlayerInDeal(player_condition_type);

int CalcMaxRaise(Player[]);

void SetCards(Player&, Card[], int&);

