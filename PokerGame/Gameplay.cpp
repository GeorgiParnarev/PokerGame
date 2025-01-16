#include <iostream>
#include <fstream>

#include "GlobalConstants.h"
#include "GamePlay.h"
#include "Player.h"
#include "Deal.h"

/// <summary>
/// Updates the status of each player, marking them as active or inactive based on their chip count.
/// </summary>
/// <param name="players">Array of players to update.</param>
void ActualizePlayers(Player players[]);

/// <summary>
/// Main game loop where the game continues until one player remains or the user opts to end the game.
/// </summary>
/// <param name="players">Array of players in the game.</param>
/// <returns>Current game condition after the loop ends.</returns>
GameCondition GameLoop(Player players[]);

/// <summary>
/// Reads player data from a file and updates their chip counts and statuses.
/// </summary>
/// <param name="players">Array of players to populate from the file.</param>
/// <returns>File condition after attempting to read the data.</returns>
FileCondition GameReadFromFile(Player players[]);

/// <summary>
/// Prompts the user to either continue an existing game or start a new one.
/// </summary>
/// <param name="players">Array of players to initialize based on the choice.</param>
void GameChoiceNewGame(Player players[]);

/// <summary>
/// Counts how many players are currently active in the game (those who are still in the game).
/// </summary>
/// <param name="players">Array of players to check for active status.</param>
/// <returns>The number of active players in the game.</returns>
int ActivePlayersCount(Player players[]);

/// <summary>
/// Initializes the players with default values such as chips and active status based on the number of players.
/// </summary>
/// <param name="players">Array of players to initialize.</param>
/// <param name="playersNum">Number of players to initialize.</param>
void GameInitPlayers(Player players[], int playersNum);

/// <summary>
/// Resets all players' data to its initial empty state.
/// </summary>
/// <param name="players">Array of players to clear.</param>
void GameClear(Player players[]);

/// <summary>
/// Asks the user for the number of players participating in the game.
/// </summary>
/// <returns>The number of players chosen by the user.</returns>
int GameSetPlayersNum();

/// <summary>
/// Saves the current state of the game (player chip counts) to a file.
/// </summary>
/// <param name="players">Array of players whose data is saved.</param>
/// <returns>The result of the file save operation.</returns>
FileCondition GameSaveToFile(Player[]);

void ActualizePlayers(Player players[])
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];

		if (player.isPlayerActive != PlayerCondition::Inactive)
		{
			if (player.chips < CHIP_VALUE)
			{
				player.isPlayerActive = PlayerCondition::Inactive;
			}
			else
			{
				player.isPlayerActive = PlayerCondition::Active;
				player.lastRaise = 0;
			}
		}
	}
}

GameCondition GameLoop(Player players[])
{
	Deal deal;
	deal.dealFlag = FIRST_DEAL;
	ClearDeal(deal);
	GameCondition condition = GameCondition::Continue;

	while (condition == GameCondition::Continue)
	{
		if (DealLoop(players, deal) == GameCondition::DealEnd)
		{
			ActualizePlayers(players);

			if (ActivePlayersCount(players) == 1)
			{
				condition = GameCondition::Win;
			}
			else
			{
				std::cout << "Play again? y/n: ";
				char ch;
				std::cin >> ch;
				while (std::cin.get() != '\n');

				if (ch == 'n' || ch == 'N')
				{
					condition = GameCondition::End;
				}
			}
		}
	}

	return condition;
}

FileCondition GameReadFromFile(Player players[])
{
	FileCondition result = FileCondition::Error;

	std::ifstream f(FILE_NAME);

	if (f.is_open())
	{
		int playerChips;

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			f >> playerChips;

			players[i].chips = playerChips;
			players[i].isPlayerActive = PlayerCondition::Active;
		}

		result = FileCondition::OK;
	}

	f.close();

	ActualizePlayers(players);

	return result;
}

void GameChoiceNewGame(Player players[])
{
	std::cout << "Game Continue" << std::endl << "New Game" << std::endl << "Choice c/n: ";

	char ch;
	std::cin >> ch;
	while (std::cin.get() != '\n');

	GameClear(players);
	FileCondition f = GameReadFromFile(players);

	bool choiceFlag = (ch == 'c' || ch == 'C') && (f == FileCondition::OK);

	if (!choiceFlag)
	{
		int playersNum = GameSetPlayersNum();
		GameInitPlayers(players, playersNum);
	}
}

void GameRun()
{
	Player players[MAX_PLAYERS];

	GameCondition condition = GameCondition::Win;

	while (condition == GameCondition::Win)
	{
		GameChoiceNewGame(players);

		condition = GameLoop(players);

		if (condition == GameCondition::End)
		{
			GameSaveToFile(players);
		}
	}
}

int ActivePlayersInDealCount(Player players[])
{
	int activeCount = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		activeCount += IsPlayerInDeal(players[i].isPlayerActive) ? 1 : 0;
	}

	return activeCount;
}

int ActivePlayersCount(Player players[])
{
	int activeCount = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		activeCount += IsPlayerInGame(players[i].isPlayerActive) ? 1 : 0;
	}

	return activeCount;
}

void GameInitPlayers(Player players[], int playersNum)
{
	GameClear(players);

	for (int i = 0; i < playersNum; i++)
	{
		players[i].chips = CHIP_VALUE * NUMBER_OF_CHIPS;
		players[i].isPlayerActive = PlayerCondition::Active;
	}
}

void GameClear(Player players[])
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		InitEmptyPlayer(players[i]);
	}
}

int GameSetPlayersNum()
{
	bool isChoiceCorect = false;

	int playersNum;

	while (!isChoiceCorect)
	{
		std::cin.clear();

		std::cout << "How many players are going to play("
			<< MIN_PLAYERS
			<< " - "
			<< MAX_PLAYERS
			<< ")? ";

		if (!(std::cin >> playersNum))
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			std::cout << std::endl << WARNING << std::endl;
			isChoiceCorect = false;
		}
		else
		{
			std::cin.ignore(INT_MAX, '\n');

			bool correctNum = (playersNum >= MIN_PLAYERS && playersNum <= MAX_PLAYERS);
			if (correctNum)
			{
				isChoiceCorect = true;
			}
		}
	}

	return playersNum;
}

FileCondition GameSaveToFile(Player players[])
{
	FileCondition result = FileCondition::OK;

	std::ofstream f(FILE_NAME);

	if (!f.is_open())
	{
		result = FileCondition::Error;
	}
	else
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			Player& player = players[i];

			if (!(f << player.chips << " "))
			{
				result = FileCondition::Error;
				break;
			}
		}
	}

	f.close();

	return result;
}

bool IsPlayerInGame(player_condition_type condition)
{
	return (condition != PlayerCondition::Inactive);
}