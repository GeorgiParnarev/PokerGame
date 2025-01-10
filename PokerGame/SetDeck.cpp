#pragma once

#include <iostream>

#include "SetDeck.h"
#include "GlobalConstants.h"

void SetUpCardDeck(Card cards[])
{
	int cardNum = 0;

	for (size_t suit = 0; suit < SUITS; suit++)
	{
		for (size_t pips = 0; pips < CARDS_COL; pips++)
		{
			Card card;
			card.card = pipsMatrix[0][pips] | pipsMatrix[1][pips] | suits[suit];

			cards[cardNum] = card;

			cardNum++;
		}
	}
}