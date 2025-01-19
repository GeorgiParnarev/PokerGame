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
* <file with gameplay global functions>
*
*/

#pragma once

#include "GlobalConstants.h"
#include "Card.h"
#include "Player.h"

/// <summary>
/// Manages the overall game loop including saving game progress at the end of the session.
/// </summary>
void GameRun();

/// <summary>
/// Counts how many players are currently active in the deal (those participating in the round).
/// </summary>
/// <param name="players">Array of players to check for active status.</param>
/// <returns>The number of active players in the deal.</returns>
int ActivePlayersInDealCount(Player players[]);

/// <summary>
/// Checks if a player is in the game based on their condition.
/// </summary>
/// <param name="condition">The condition of the player.</param>
/// <returns>True if the player is active, false otherwise.</returns>
bool IsPlayerInGame(player_condition_type condition);
