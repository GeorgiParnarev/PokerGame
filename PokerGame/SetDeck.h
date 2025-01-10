#pragma once

#include "GlobalTypes.h"
#include "Card.h"

/// <summary>
/// Sets up the deck of cards by initializing it with all possible cards 
/// (combination of suits and pips) and adding them to the provided array.
/// </summary>
/// <param name="v">An array where the deck will be stored.</param>
void SetUpCardDeck(Card[]);