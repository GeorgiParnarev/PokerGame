#include <ctime>

#include "Player.h"
#include "Card.h"
#include "GlobalConstants.h"

void InitEmptyPlayer(Player& player)
{
	player.name = "";
	player.cardsAndPointsDisplay= "";
	player.isPlayerActive = PlayerCondition::Unactive;
	player.chips = 0;
	player.lastRaise = 0;
	player.points = 0;
	player.id = 0;
}

void AddChips(Player& player, int chips)
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
	player.cardsAndPointsDisplay = CardsToString(cards, player.points);
}

std::string CardsToString(Card cards[], int points)
{
	std::string result = "";

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		result.append(CardToString(cards[i].card)).append(" ");
	}

	return result.append(std::to_string(points));
}

bool IsPlayerInDeal(player_condition_type condition)
{
	return (condition != PlayerCondition::Unactive) && ((condition & PlayerCondition::Active) == PlayerCondition::Active);
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