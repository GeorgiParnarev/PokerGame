#include <ctime>

#include "Player.h"
#include "Card.h"
#include "GlobalConstants.h"

void AddChips(Player player, int chips)
{
	player.chips += chips;
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

bool CardsShareSameKind(const Card& card1, const Card& card2)
{
	return (card1.card & Pip::PipMask) == (card2.card & Pip::PipMask);
}

bool PairWithSevenOfClubs(int& scoredPoints, Player player, Card cards[])
{
	if (!player.isHasSevenClubs)
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

bool CardsShareSameSuit(const Card& card1, const Card& card2)
{
	return (card1.card & Suit::SuitMask) == (card2.card & Suit::SuitMask);
}

bool PairOfSuits(int& scoredPoints, Player player, Card cards[])
{
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

int CalculatePoints(Player player, Card cards[])
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
		int handScore = PAIR_ACE_POINTS + (player.isHasSevenClubs ? SEVEN_OF_CLUBS_POINTS : 0);

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

	if (PairWithSevenOfClubs(pairPoints, player, cards))
	{
		int handScore = pairPoints + SEVEN_OF_CLUBS_POINTS;

		if (points < handScore)
		{
			points = handScore;
		}

		pairPoints = 0;
	}

	if (PairOfSuits(pairPoints, player, cards))
	{
		int handScore = (pairPoints + (player.isHasSevenClubs ? SEVEN_OF_CLUBS_POINTS : 0));

		if (points < handScore)
		{
			points = handScore;
		}
	}

	if (points == 0)
	{
		points = CalcHighCard(cards) + (player.isHasSevenClubs ? SEVEN_OF_CLUBS_POINTS : 0);
	}

	return points;
}

