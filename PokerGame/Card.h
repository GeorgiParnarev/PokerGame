#pragma once

#include <string>

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