#pragma once

#include "GlobalTypes.h"
#include "GlobalConstants.h"
#include "Card.h"

/// <summary>
/// Represents a player in the card game, including their current state, points, and chips.
/// </summary>
/// <remarks>
/// This struct contains information about a player's cards, chips, points, 
/// last betting action, and their active/inactive status in the game.
/// </remarks>
struct Player
{
	char cardsDisplay[CARDS_DISPLAY_MAX_SIZE];	/// A string representation of the player's cards in human-readable format.
	int chips;									/// The amount of chips the player currently holds.
	int points;									/// The player's total points based on their card hand.
	int lastRaise;								/// The last amount the player raised during betting.
	player_condition_type isPlayerActive;		/// Indicates whether the player is currently active in the game.
};

/// <summary>
/// Initializes a player object with default values for an empty player.
/// </summary>
/// <param name="player">The player to initialize.</param>
void InitEmptyPlayer(Player&);

/// <summary>
/// Assigns random cards from the deck to the player and calculates their points.
/// </summary>
/// <param name="player">The player receiving the cards.</param>
/// <param name="cardsDeck">The deck of cards to draw from.</param>
/// <param name="currentDeckSize">The current size of the deck, which will be updated.</param>
void SetCards(Player& player, Card cardsDeck[], int& currentDeckSize);

/// <summary>
/// Checks if the player is currently active in the deal.
/// </summary>
/// <param name="condition">The condition of the player to evaluate.</param>
/// <returns>True if the player is active, otherwise false.</returns>
bool IsPlayerInDeal(player_condition_type condition);

/// <summary>
/// Calculates the maximum raise allowed based on the chips available for all active players.
/// </summary>
/// <param name="players">The array of players in the game.</param>
/// <returns>The maximum raise value that can be made.</returns>
int CalcMaxRaise(Player players[]);

