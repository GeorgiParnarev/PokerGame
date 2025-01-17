#pragma once

#include "GlobalConstants.h"
#include "Player.h"

/// <summary>
/// Represents the state of the current deal in the card game.
/// </summary>
/// <remarks>
/// This struct contains information about the pot, raises, and the deal's continuation state.
/// It is used to track the progress of a single deal within the game.
/// </remarks>
struct Deal
{
	int pot;				/// The total amount of chips in the pot for the current deal.
	int lastGameRaise;		/// The amount of the last raise made during the current deal.
	int currentMaxRaise;	/// The maximum allowable raise in the current betting round.
	bool dealFlag;			/// Indicates whether the deal should continue or has ended.
};

/// <summary>
/// Main game loop that processes the deal, checks for the end of the game, and continues with new rounds if necessary.
/// </summary>
/// <param name="players">Array of players in the game.</param>
/// <param name="deal">The current deal being played.</param>
/// <returns>Current game condition after the loop ends.</returns>
GameCondition DealLoop(Player players[], Deal& deal);

/// <summary>
/// Resets the deal parameters for the next round, including resetting the pot and last raise.
/// </summary>
/// <param name="deal">The deal to be cleared.</param>
void ClearDeal(Deal&);