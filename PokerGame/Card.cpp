#include "Card.h"
#include "GlobalConstants.h"

void CardToString(Card& card)
{
	for (size_t i = 0; i < CARD_STRING_MAX_SIZE; i++)
	{
		card.cardString[i] = '\0';
	}

	switch (card.card & Pip::PipMask)
	{
	case Pip::A:
		card.cardString[0] = 'A';
		break;

	case Pip::K:
		card.cardString[0] = 'K';
		break;

	case Pip::Q:
		card.cardString[0] = 'Q';
		break;

	case Pip::J:
		card.cardString[0] = 'J';
		break;

	case Pip::N10:
		card.cardString[0] = '1';
		card.cardString[1] = '0';
		break;

	case Pip::N9:
		card.cardString[0] = '9';
		break;

	case Pip::N8:
		card.cardString[0] = '8';
		break;

	case Pip::N7:
		card.cardString[0] = '7';
		break;
	default:
		break;
	}

	int index = (card.cardString[0] == '1' ? 2 : 1);

	switch (card.card & Suit::SuitMask)
	{
	case Suit::Hearts:
		card.cardString[index] = 'H';
		break;

	case Suit::Diamonds:
		card.cardString[index] = 'D';
		break;

	case Suit::Clubs:
		card.cardString[index] = 'C';
		break;

	case Suit::Spades:
		card.cardString[index] = 'S';
		break;
	default:
		break;
	}
}

bool IsSevenClubsPresent(Card cards[])
{
	bool isSevenOfClubsPresent = false;

	for (size_t i = 0; i < CARDS_COUNT; i++)
	{
		if (cards[i].isSevenClubs)
		{
			isSevenOfClubsPresent = true;

			break;
		}
	}

	return isSevenOfClubsPresent;
}

void SetUpCardDeck(Card cards[])
{
	int cardNum = 0;

	for (size_t suit = 0; suit < SUITS; suit++)
	{
		for (size_t pips = 0; pips < CARDS_COL; pips++)
		{
			Card card;
			card.card = pipsMatrix[0][pips] | pipsMatrix[1][pips] | suits[suit];

			bool isSevenClubs = (card.card & (card_type)Suit::SuitMask) == Suit::Clubs;
			isSevenClubs = isSevenClubs && (card.card & Pip::PipMask) == Pip::N7;

			card.isSevenClubs = isSevenClubs;

			CardToString(card);

			cards[cardNum] = card;

			cardNum++;
		}
	}
}