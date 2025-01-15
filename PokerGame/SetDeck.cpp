#pragma once

#include "SetDeck.h"
#include "GlobalConstants.h"
#include "Card.h"

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