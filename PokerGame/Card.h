#pragma once

#include <string>

#include "GlobalTypes.h"

struct MyStruct
{
	card_type card;
	bool isSevenClubs;
};

/// <summary>
/// Converts the card into a human-readable string representation.
/// </summary>
/// <returns>A string describing the card (e.g., "7C" for Seven of Clubs).</returns>
std::string CardToString(card_type card);