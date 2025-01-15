#pragma once

#include <string>

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
/// Calculates the points based on the player's hand.
/// </summary>
/// <returns>The calculated points.</returns>
int CalculatePoints(Card[]);

/// <summary>
/// Checks if the player has three Sevens in their hand.
/// </summary>
/// <returns>True if the player has three Sevens; otherwise, false.</returns>
bool ThreeSevens(Card[]);

/// <summary>
/// Checks if the player has three cards of the same rank.
/// </summary>
/// <returns>True if the player has three of the same rank; otherwise, false.</returns>
bool ThreeOfKind(Card[]);

/// <summary>
/// Checks if the player has three cards of the same suit.
/// </summary>
/// <returns>True if the player has three of the same suit; otherwise, false.</returns>
bool ThreeOfSameSuit(Card[]);

/// <summary>
/// Checks if the player has a pair of Aces.
/// </summary>
/// <returns>True if the player has a pair of Aces; otherwise, false.</returns>
bool PairOfAces(Card[]);

/// <summary>
/// Checks if the player has a pair of Sevens.
/// </summary>
/// <returns>True if the player has a pair of Sevens; otherwise, false.</returns>
bool PairOfSevens(Card[]);

/// <summary>
/// Checks if the player has a pair of cards based on the rank of two cards and the Seven of Clubs, and adds points accordingly.
/// </summary>
/// <param name="scoredPoints">The accumulated score to be updated if a valid pair with the Seven of Clubs is found.</param>
/// <returns>True if the player has a pair of cards based on rank and the Seven of Clubs; otherwise, false.</returns>
bool PairWithSevenOfClubs(int&, Card[]);

/// <summary>
/// Checks if the player has a pair of cards from the same suit and adds points accordingly.
/// </summary>
/// <param name="scoredPoints">The accumulated score to be updated.</param>
/// <returns>True if the player has a pair of cards from the same suit; otherwise, false.</returns>
bool PairOfSuits(int&, Card[]);

/// <summary>
/// Calculates the points based on the highest card in the player's hand.
/// </summary>
/// <returns>The points for the highest card.</returns>
int CalcHighCard(Card[]);

/// <summary>
/// Checks if two cards share the same rank.
/// </summary>
/// <param name="card1">The first card.</param>
/// <param name="card2">The second card.</param>
/// <returns>True if the cards share the same rank; otherwise, false.</returns>
bool CardsShareSameKind(const Card, const Card);

/// <summary>
/// Checks if two cards share the same suit.
/// </summary>
/// <param name="card1">The first card.</param>
/// <param name="card2">The second card.</param>
/// <returns>True if the cards share the same suit; otherwise, false.</returns>
bool CardsShareSameSuit(const Card, const Card);

void AddChips(Player&, int);

bool IsPlayerInDeal(player_condition_type);

int CalcMaxRaise(Player[]);

void SetCards(Player&, Card[], int&);

void InitEmptyPlayer(Player&);