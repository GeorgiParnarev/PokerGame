#include "Card.h"
#include "GlobalConstants.h"

std::string CardToString(card_type card)
{
	std::string result = "";

	switch (card & Pip::PipMask)
	{
	case Pip::A:
		result.append("A");
		break;

	case Pip::K:
		result.append("K");
		break;

	case Pip::Q:
		result.append("Q");
		break;

	case Pip::J:
		result.append("J");
		break;

	case Pip::N10:
		result.append("10");
		break;

	case Pip::N9:
		result.append("9");
		break;

	case Pip::N8:
		result.append("8");
		break;

	case Pip::N7:
		result.append("7");
		break;
	default:
		break;
	}

	switch (card & Suit::SuitMask)
	{
	case Suit::Hearts:
		result.append("H");
		break;

	case Suit::Diamonds:
		result.append("D");
		break;

	case Suit::Clubs:
		result.append("C");
		break;

	case Suit::Spades:
		result.append("S");
		break;
	default:
		break;
	}

	return result;
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