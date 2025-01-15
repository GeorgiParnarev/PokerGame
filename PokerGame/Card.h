#pragma once

#include "GlobalTypes.h"
#include "GlobalConstants.h"

struct Card
{
	card_type card;
	bool isSevenClubs;
	char cardString[CARD_STRING_MAX_SIZE];
};

/// <summary>
/// Converts the card into a human-readable string representation.
/// </summary>
/// <returns>A string describing the card (e.g., "7C" for Seven of Clubs).</returns>
void CardToString(Card&);

bool IsSevenClubsPresent(Card[]);

/// <summary>
/// Sets up the deck of cards by initializing it with all possible cards
/// (combination of suits and pips) and adding them to the provided array.
/// </summary>
/// <param name="v">An array where the deck will be stored.</param>
void SetUpCardDeck(Card[]);