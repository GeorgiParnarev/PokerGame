#include <iostream>

#include "GlobalConstants.h"
#include "Deal.h"
#include "Gameplay.h"
#include "Player.h"

int CalcHalf(int pot);

void DisplayPlayer(Player& player, int playerIndex);

int CalcHalf(int pot)
{
	int half = pot / 2;
	if ((half % 10) == 5)
	{
		half += 5;
	}

	return half;
}

void DisplayPlayer(Player& player, int playerIndex)
{
	std::cout << "Player" << playerIndex << ' ' << player.cardsDisplay << player.points << std::endl << std::endl;
}

void DisplayPlayersInDeal(Player players[])
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player.isPlayerActive;

		if (IsPlayerInGame(condition))
		{
			DisplayPlayer(player, i + 1);
		}
	}
}

GameCondition DealLoop(Player players[], Deal& deal)
{
	int activeCount = ActivePlayersInDealCount(players);

	if (activeCount == 1)
	{
		return GameCondition::DealEnd;
	}

	if (DealStart(players, deal))
	{
		DealPlay(players, deal);
	}

	DeterminingWinner(players, deal);

	deal.dealFlag = CONTINUE_DEAL;

	return DealLoop(players, deal);
}

bool DealStart(Player players[], Deal& deal)
{
	ClearDeal(deal);
	Card cardDeck[CARDS_IN_DECK];

	SetUpCardDeck(cardDeck);
	int currentDeckCount = CARDS_IN_DECK;

	bool result = DEAL_PLAY;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player.isPlayerActive;

		if (IsPlayerInDeal(condition))
		{
			player.chips -= CHIP_VALUE;
			deal.pot += CHIP_VALUE;

			result = result && ((player.chips > 0) ? DEAL_PLAY : NOT_DEAL_PLAY);

			std::cout << "Player" << i + 1 << ": " << player.chips << std::endl << std::endl;

			SetCards(player, cardDeck, currentDeckCount);
		}
	}

	return result;
}

void DealPlay(Player players[], Deal& deal)
{
	int currentCall = 0;
	int currentPlayerIndex = 0;
	bool isFirst = true;
	int activePlayers = ActivePlayersInDealCount(players);

	while ((activePlayers > 1) && currentCall < activePlayers - 1)
	{
		deal.currentMaxRaise = CalcMaxRaise(players);

		Player& player = players[currentPlayerIndex];
		player_condition_type condition = player.isPlayerActive;
		if (IsPlayerInDeal(condition))
		{
			std::cout << "Pot: " << deal.pot << std::endl << std::endl;

			bool isCorrect = false;

			while (!isCorrect)
			{
				player_condition_type ChoiceMade = PlayerCondition::Inactive;
				std::cout << "You have given: " << player.lastRaise / CHIP_VALUE << " chips" << std::endl;
				std::cout << "Last raise is: " << deal.lastGameRaise / CHIP_VALUE << " chips" << std::endl << std::endl;
				DisplayPlayer(player, currentPlayerIndex + 1);

				if (isFirst)
				{
					std::cout << "Player" << currentPlayerIndex + 1 << " raise or fold? r/f: ";
					ChoiceMade = PlayerCondition::Raise | PlayerCondition::Fold;
				}
				else if (deal.currentMaxRaise <= deal.lastGameRaise)
				{
					std::cout << "Player" << currentPlayerIndex + 1 << " call or fold? c/f: ";
					ChoiceMade = PlayerCondition::Fold | PlayerCondition::Call;
				}
				else
				{
					std::cout << "Player" << currentPlayerIndex + 1 << " raise, call or fold? r/c/f: ";
					ChoiceMade = PlayerCondition::Raise | PlayerCondition::Fold | PlayerCondition::Call;
				}

				char ch;
				std::cin >> ch;
				while (std::cin.get() != '\n');

				std::cout << std::endl;

				if ((ch == 'f' || ch == 'F') && (ChoiceMade & PlayerCondition::Fold) == PlayerCondition::Fold)
				{
					player.isPlayerActive = PlayerCondition::Fold;
					isCorrect = true;
				}
				else if ((ch == 'c' || ch == 'C') && !isFirst && (ChoiceMade & PlayerCondition::Call) == PlayerCondition::Call)
				{
					currentCall++;
					int lastPlayerRaise = player.lastRaise;
					int pays = deal.lastGameRaise - lastPlayerRaise;

					player.lastRaise = deal.lastGameRaise;

					player.chips -= pays;
					deal.pot += pays;
					isCorrect = true;
				}
				else if ((ch == 'r' || ch == 'R') && (ChoiceMade & PlayerCondition::Raise) == PlayerCondition::Raise)
				{
					bool isCorrectPay = false;
					int paymentAmount;

					while (!isCorrectPay)
					{
						std::cout << "Player" << currentPlayerIndex + 1 << " pay: (" << (deal.lastGameRaise + CHIP_VALUE) / CHIP_VALUE << " - " << deal.currentMaxRaise / CHIP_VALUE << " chips): ";

						if (!(std::cin >> paymentAmount))
						{
							std::cin.clear();
							std::cin.ignore(INT_MAX, '\n');
							std::cout << std::endl << WARNING << std::endl;
							std::cout << std::endl;
						}
						else
						{
							paymentAmount *= CHIP_VALUE;

							if ((paymentAmount >= deal.lastGameRaise + CHIP_VALUE) && paymentAmount <= deal.currentMaxRaise)
							{
								isCorrectPay = true;
							}
							else
							{
								std::cout << std::endl << WARNING << std::endl;
								isCorrectPay = false;
							}
						}
					}

					isFirst = false;

					player.lastRaise = paymentAmount;
					player.chips -= paymentAmount;

					deal.pot += paymentAmount;
					deal.lastGameRaise = paymentAmount;

					isCorrect = true;
					currentCall = 0;
				}
			}
		}

		currentPlayerIndex = (++currentPlayerIndex) % MAX_PLAYERS;

		activePlayers = ActivePlayersInDealCount(players);
	}
}

int CalcMaxPoints(Player players[])
{
	int maxPoint = 0;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player.isPlayerActive;

		if (IsPlayerInDeal(condition) && player.points > maxPoint)
		{
			maxPoint = player.points;
		}
	}

	return maxPoint;
}

void DeterminingWinner(Player players[], Deal& deal)
{
	int maxPoint = CalcMaxPoints(players);

	int winnersCount = 0;
	int lastWinnerIndex = -1;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player.isPlayerActive;
		if (IsPlayerInDeal(condition))
		{
			if (player.points != maxPoint)
			{
				player.isPlayerActive = PlayerCondition::Hold;
			}
			else
			{
				winnersCount++;
				lastWinnerIndex = i;
			}
		}
	}

	if (winnersCount > 1)
	{
		int halfPot = CalcHalf(deal.pot);

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			Player& player = players[i];
			player_condition_type condition = player.isPlayerActive;

			if (condition == PlayerCondition::Active)
			{
				if (player.chips < CHIP_VALUE)
				{
					player.chips = CHIPS_ADD_VALUE * CHIP_VALUE;
				}
			}
			else if (condition == PlayerCondition::Hold)
			{
				if (player.chips < (halfPot + CHIP_VALUE))
				{
					player.isPlayerActive = PlayerCondition::Fold;
				}
				else
				{
					bool isCorrect = false;

					while (!isCorrect)
					{
						std::cout << "Player" << i + 1 << " will you pay half pot? y/n:";

						char ch;
						std::cin >> ch;
						while (std::cin.get() != '\n');

						std::cout << std::endl << std::endl;

						if (ch == 'y' || ch == 'Y')
						{
							player.chips -= halfPot;
							deal.pot += halfPot;
							player.isPlayerActive = PlayerCondition::Active;
							player.lastRaise = 0;
							isCorrect = true;
						}
						else if (ch == 'n' || ch == 'N')
						{
							player.isPlayerActive = PlayerCondition::Fold;
							player.lastRaise = 0;
							isCorrect = true;
						}
					}
				}
			}
		}
	}
	else
	{
		Player& winner = players[lastWinnerIndex];

		winner.chips += deal.pot;
		winner.lastRaise = 0;
		winner.isPlayerActive = PlayerCondition::Active;
		deal.pot = 0;
		deal.lastGameRaise = 0;

		std::cout << "Player" << lastWinnerIndex + 1 << " is winner." << std::endl << std::endl;
		DisplayPlayersInDeal(players);
	}
}

void ClearDeal(Deal& deal)
{
	if (deal.dealFlag)
	{
		deal.pot = 0;
	}

	deal.lastGameRaise = 0;
}