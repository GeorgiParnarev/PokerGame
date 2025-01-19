/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Georgi Parnarev
* @idnumber 0MI0600547 * @compiler VC
*
* <file with card global functions>
*
*/

#pragma once

#include "GlobalTypes.h"
#include "GlobalConstants.h"

/// <summary>
/// Represents a playing card with metadata and display information.
/// </summary>
/// <remarks>
/// This struct contains details about a card, including its type, 
/// whether it is the Seven of Clubs, and a human-readable string representation.
/// </remarks>
struct Card
{
	card_type card;								/// The type of the card (e.g., suit and rank).
	bool isSevenClubs;							/// Indicates if the card is the Seven of Clubs.
	char cardString[CARD_STRING_MAX_SIZE];		/// A string representation of the card (e.g., "7C" for Seven of Clubs).
};

/// <summary>
/// Checks if the "Seven of Clubs" card is present in an array of cards.
/// </summary>
/// <param name="cards">The array of Card objects to search through.</param>
/// <returns>
/// True if the "Seven of Clubs" card is found in the array; otherwise, false.
/// </returns>
/// <remarks>
/// This method iterates through the card array and checks the isSevenClubs field 
/// of each Card object to determine if the "Seven of Clubs" is present.
/// </remarks>
bool IsSevenClubsPresent(Card cards[]);

/// <summary>
/// Initializes a complete deck of cards, assigning each card its rank, suit, and metadata.
/// </summary>
/// <param name="cards">An array of Card objects to be populated as the deck of cards.</param>
/// <remarks>
/// Each card in the deck is given a unique combination of rank and suit. 
/// The "Seven of Clubs" flag is set for the corresponding card, 
/// and a string representation is generated for each card.
/// </remarks>
void SetUpCardDeck(Card cards[]);