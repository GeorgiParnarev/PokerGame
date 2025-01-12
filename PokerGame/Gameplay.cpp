#include <iostream>
#include <fstream>

#include "Gameplay.h"
#include "Deal.h"

int ActivePlayersInDealCount(Player players[])
{
	int activeCount = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		activeCount += IsPlayerInDeal(players[i].isPlayerActive) ? 1 : 0;
	}

	return activeCount;
}

bool IsPlayerInGame(player_condition_type condition)
{
	return (condition != PlayerCondition::Unactive);
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

void ActualizePlayers(Player players[])
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];

		if (player.isPlayerActive != PlayerCondition::Unactive)
		{
			if (player.chips < CHIP_VALUE)
			{
				player.isPlayerActive = PlayerCondition::Unactive;
			}
			else
			{
				player.isPlayerActive = PlayerCondition::Active;
				player.lastRaise = 0;
			}
		}
	}
}

void GameInitPlayers(Player players[], int playersNum)
{
	GameClear(players);

	for (int i = 0; i < playersNum; i++)
	{
		int plaiersId = i + 1;
		std::string playerName = std::string().append("Player").append(std::to_string(plaiersId));

		players[i].name = playerName;
		players[i].chips = CHIP_VALUE * NUMBER_OF_CHIPS;
		players[i].id = plaiersId;
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
	bool isChoisCorect = false;

	int playersNum;

	while (!isChoisCorect)
	{
		std::string s;

		std::cout << "How many players are going to play("
			<< MIN_PLAYERS
			<< " - "
			<< MAX_PLAYERS
			<< ")? ";

		std::getline(std::cin, s);

		try
		{
			playersNum = stoi(s);
			bool correctNum = (playersNum < MIN_PLAYERS || playersNum > MAX_PLAYERS);

			if (correctNum)
			{
				throw std::runtime_error("");
			}

			isChoisCorect = true;
		}
		catch (const std::exception&)
		{
			std::cout << WARNING << std::endl;
		}
	}

	return playersNum;
}

FileCondition GameSaveToFile(Player players[])
{
	FileCondition result = FileCondition::OK;

	std::ofstream f(FILE_NAME);

	try
	{
		if (f.is_open())
		{
			int playerCount = ActivePlayersCount(players);
			f << playerCount << std::endl;

			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				Player& player = players[i];
				bool condition = IsPlayerInGame(player.isPlayerActive);
				if (condition)
				{
					f << player.name << std::endl;
					f << player.chips << std::endl;
					f << player.id << std::endl;

				}

			}
		}
	}
	catch (const std::exception&)
	{
		result = FileCondition::Error;
	}

	return result;
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
				std::string s;
				std::cout << "Play again? y/n: ";
				std::getline(std::cin, s);

				if (s == "n" || s == "N")
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
		std::string s;
		while (getline(f, s))
		{
			int playersNum = stoi(s);

			for (int i = 0; i < playersNum; i++)
			{
				getline(f, s);
				std::string name = s;

				getline(f, s);
				int chips = stoi(s);

				getline(f, s);
				int id = stoi(s);

				players[id].name = name;
				players[id].chips = chips;
				players[id].id = id;
				players[id].isPlayerActive = PlayerCondition::Active;
			}
		}
		result = FileCondition::OK;
	}

	f.close();

	return result;
}

void GameChoisNewGame(Player players[])
{
	std::string s;
	std::cout << "Game Continue" << std::endl << "New Game" << std::endl << "Choice c/n: ";
	std::getline(std::cin, s);

	GameClear(players);
	FileCondition f = GameReadFromFile(players);

	bool choisFlag = (s == "c" || s == "C") && (f == FileCondition::OK);

	if (!choisFlag)
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
		GameChoisNewGame(players);

		condition = GameLoop(players);

		if (condition == GameCondition::End)
		{
			GameSaveToFile(players);
		}
	}
}