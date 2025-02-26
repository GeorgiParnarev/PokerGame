/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Georgi Parnarev
* @idnumber 0MI0600547 * @compiler VC
*
* <file with player logic>
*
*/

#include <ctime>
#include <iostream>

#include "Player.h"
#include "Card.h"
#include "GlobalConstants.h"

/// <summary>
/// Generates a string representation of the player's cards and stores it in the player object.
/// </summary>
/// <param name="player">The player whose card display string will be updated.</param>
/// <param name="cards">The cards held by the player.</param>
void GeneratePlayerString(Player& player, Card cards[]);

/// <summary>
/// Determines if all the cards in the array are Sevens.
/// </summary>
/// <param name="cards">The array of cards to check.</param>
/// <returns>True if all cards are Sevens, otherwise false.</returns>
bool ThreeSevens(Card cards[]);

/// <summary>
/// Checks if all cards in the array are of the same kind (rank).
/// </summary>
/// <param name="cards">The array of cards to check.</param>
/// <returns>True if all cards are of the same kind, otherwise false.</returns>
bool ThreeOfKind(Card cards[]);

/// <summary>
/// Checks if all cards in the array share the same suit.
/// </summary>
/// <param name="cards">The array of cards to check.</param>
/// <returns>True if all cards are of the same suit, otherwise false.</returns>
bool ThreeOfSameSuit(Card cards[]);

/// <summary>
/// Checks if the card array contains exactly a pair of Aces.
/// </summary>
/// <param name="cards">The array of cards to check.</param>
/// <returns>True if there is a pair of Aces, otherwise false.</returns>
bool PairOfAces(Card cards[]);

/// <summary>
/// Checks if the card array contains exactly a pair of Sevens.
/// </summary>
/// <param name="cards">The array of cards to check.</param>
/// <returns>True if there is a pair of Sevens, otherwise false.</returns>
bool PairOfSevens(Card cards[]);

/// <summary>
/// Checks if two cards share the same kind (rank).
/// </summary>
/// <param name="card1">The first card to compare.</param>
/// <param name="card2">The second card to compare.</param>
/// <returns>True if the cards share the same kind, otherwise false.</returns>
bool CardsShareSameKind(const Card card1, const Card card2);

/// <summary>
/// Checks if the cards contain a pair that includes the Seven of Clubs.
/// </summary>
/// <param name="scoredPoints">The reference to the current score to update if a pair is found.</param>
/// <param name="cards">The array of cards to check.</param>
/// <returns>True if a pair with the Seven of Clubs exists, otherwise false.</returns>
bool PairWithSevenOfClubs(int& scoredPoints, Card cards[]);

/// <summary>
/// Checks if two cards share the same suit.
/// </summary>
/// <param name="card1">The first card to compare.</param>
/// <param name="card2">The second card to compare.</param>
/// <returns>True if the cards share the same suit, otherwise false.</returns>
bool CardsShareSameSuit(const Card card1, const Card card2);

/// <summary>
/// Checks if the cards contain a pair of the same suit.
/// </summary>
/// <param name="scoredPoints">The reference to the current score to update if a pair is found.</param>
/// <param name="cards">The array of cards to check.</param>
/// <returns>True if a pair of suits exists, otherwise false.</returns>
bool PairOfSuits(int& scoredPoints, Card cards[]);

/// <summary>
/// Calculates the highest card value from the array of cards.
/// </summary>
/// <param name="cards">The array of cards to evaluate.</param>
/// <returns>The value of the highest card.</returns>
int CalcHighCard(Card cards[]);

/// <summary>
/// Calculates the total points based on the card combination in the array.
/// </summary>
/// <param name="cards">The array of cards to evaluate.</param>
/// <returns>The total points calculated for the card combination.</returns>
int CalculatePoints(Card cards[]);

void GeneratePlayerString(Player& player, Card cards[])
{
	int index = 0;

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		int cardIndex = 0;
		while (cards[i].cardString[cardIndex] != '\0')
		{
			player.cardsDisplay[index++] = cards[i].cardString[cardIndex++];
		}

		player.cardsDisplay[index++] = ' ';
	}

	player.cardsDisplay[index] = '\0';
}

void InitEmptyPlayer(Player& player)
{
	player.isPlayerActive = PlayerCondition::Inactive;
	player.chips = 0;
	player.lastRaise = 0;
	player.points = 0;
}

bool ThreeSevens(Card cards[])
{
	bool result = true;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		result = result && ((cards[i].card & Pip::PipMask) == Pip::N7);
	}

	return result;
}

bool ThreeOfKind(Card cards[])
{
	card_type cardKind = cards[0].card & Pip::PipMask;

	bool result = true;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		result = result && ((cards[i].card & Pip::PipMask) == cardKind);
	}

	return result;
}

bool ThreeOfSameSuit(Card cards[])
{
	card_type cardSuit = cards[0].card & Suit::SuitMask;

	bool result = true;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		result = result && ((cards[i].card & Suit::SuitMask) == cardSuit);
	}

	return result;
}

bool PairOfAces(Card cards[])
{
	int aceCount = 0;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		if ((cards[i].card & Pip::PipMask) == Pip::A)
		{
			aceCount++;
		}
	}

	if (aceCount == PAIR_CARD_AMOUNT)
	{
		return IS_ACE_OR_SEVEN_PAIR;
	}

	return IS_NOT_ACE_OR_SEVEN_PAIR;
}

bool PairOfSevens(Card cards[])
{
	int sevenCount = 0;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		if ((cards[i].card & Pip::PipMask) == Pip::N7)
		{
			sevenCount++;
		}
	}

	if (sevenCount == PAIR_CARD_AMOUNT)
	{
		return IS_ACE_OR_SEVEN_PAIR;
	}

	return IS_NOT_ACE_OR_SEVEN_PAIR;
}

bool CardsShareSameKind(const Card card1, const Card card2)
{
	return (card1.card & Pip::PipMask) == (card2.card & Pip::PipMask);
}

bool PairWithSevenOfClubs(int& scoredPoints, Card cards[])
{
	if (!IsSevenClubsPresent(cards))
	{
		return SEVEN_CLUBS_NOT_PRESENT;
	}

	if (CardsShareSameKind(cards[0], cards[1]))
	{
		scoredPoints += (cards[0].card & Rank::RankMask) + (cards[1].card & Rank::RankMask);

		return IS_SAME_KIND;
	}

	if (CardsShareSameKind(cards[1], cards[2]))
	{
		scoredPoints += (cards[1].card & Rank::RankMask) + (cards[2].card & Rank::RankMask);

		return IS_SAME_KIND;
	}

	if (CardsShareSameKind(cards[0], cards[2]))
	{
		scoredPoints += (cards[0].card & Rank::RankMask) + (cards[2].card & Rank::RankMask);

		return IS_SAME_KIND;
	}

	return IS_NOT_SAME_KIND;
}

bool CardsShareSameSuit(const Card card1, const Card card2)
{
	return (card1.card & Suit::SuitMask) == (card2.card & Suit::SuitMask);
}

bool PairOfSuits(int& scoredPoints, Card cards[])
{
	int clubsCount = 0;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		if ((cards[i].card & Suit::SuitMask) == Suit::Clubs)
		{
			clubsCount++;
		}
	}

	if (IsSevenClubsPresent(cards) && clubsCount == 2)
	{
		return false;
	}

	if (CardsShareSameSuit(cards[0], cards[1]))
	{
		scoredPoints += (cards[0].card & Rank::RankMask) + (cards[1].card & Rank::RankMask);

		return IS_SAME_SUIT;
	}

	if (CardsShareSameSuit(cards[1], cards[2]))
	{
		scoredPoints += (cards[1].card & Rank::RankMask) + (cards[2].card & Rank::RankMask);

		return IS_SAME_SUIT;
	}

	if (CardsShareSameSuit(cards[0], cards[2]))
	{
		scoredPoints += (cards[0].card & Rank::RankMask) + (cards[2].card & Rank::RankMask);

		return IS_SAME_SUIT;
	}

	return IS_NOT_SAME_SUIT;
}

int CalcHighCard(Card cards[])
{
	int points = 0;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		if (!cards[i].isSevenClubs && points < (cards[i].card & Rank::RankMask))
		{
			points = cards[i].card & Rank::RankMask;
		}
	}

	return points;
}

int CalculatePoints(Card cards[])
{
	int points = 0;

	if (ThreeSevens(cards))
	{
		return MAX_POINTS;
	}

	if (ThreeOfKind(cards))
	{
		return 3 * (cards[0].card & Rank::RankMask);
	}

	if (ThreeOfSameSuit(cards))
	{
		for (size_t i = 0; i < CARDS_COUNT; i++)
		{
			points += (cards[i].card & Rank::RankMask);
		}

		return points;
	}

	if (PairOfAces(cards))
	{
		int handScore = PAIR_ACE_POINTS + (IsSevenClubsPresent(cards) ? SEVEN_OF_CLUBS_POINTS : 0);

		if (points < handScore)
		{
			points = handScore;
		}
	}

	if (PairOfSevens(cards))
	{
		if (points < PAIR_SEVEN_POINTS)
		{
			points = PAIR_SEVEN_POINTS;
		}
	}

	int pairPoints = 0;

	if (PairWithSevenOfClubs(pairPoints, cards))
	{
		int handScore = pairPoints + SEVEN_OF_CLUBS_POINTS;

		if (points < handScore)
		{
			points = handScore;
		}

		pairPoints = 0;
	}

	if (PairOfSuits(pairPoints, cards))
	{
		int handScore = (pairPoints + (IsSevenClubsPresent(cards) ? SEVEN_OF_CLUBS_POINTS : 0));

		if (points < handScore)
		{
			points = handScore;
		}
	}

	if (points == 0)
	{
		points = CalcHighCard(cards) + (IsSevenClubsPresent(cards) ? SEVEN_OF_CLUBS_POINTS : 0);
	}

	return points;
}

void SetCards(Player& player, Card cardsDeck[], int& currentDeckSize)
{
	srand(time(0));

	Card cards[CARDS_COUNT];

	for (int i = 0; i < CARDS_COUNT; i++)
	{
		int randomIndex = rand() % currentDeckSize;

		cards[i] = cardsDeck[randomIndex];
		cardsDeck[randomIndex] = cardsDeck[currentDeckSize - 1];
		currentDeckSize--;
	}

	player.points = CalculatePoints(cards);
	GeneratePlayerString(player, cards);
}

bool IsPlayerInDeal(player_condition_type condition)
{
	return (condition != PlayerCondition::Inactive) && ((condition & PlayerCondition::Active) == PlayerCondition::Active);
}

int CalcMaxRaise(Player players[])
{
	int maxRaise = INT_MAX;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		player_condition_type condition = players[i].isPlayerActive;
		int playerChips = players[i].chips;
		if (IsPlayerInDeal(condition) && playerChips < maxRaise)
		{
			maxRaise = playerChips;
		}
	}

	return maxRaise;
}