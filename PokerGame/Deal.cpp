#include <iostream>

#include "GlobalConstants.h"
#include "Deal.h"
#include "Gameplay.h"
#include "SetDeck.h"
#include "Player.h"

void DisplayPlayersInDeal(Player* players)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Player& player = players[i];
		player_condition_type condition = player.isPlayerActive;

		if (IsPlayerInGame(condition))
		{
			std::cout << player.cardsAndPointsDisplay << std::endl << std::endl;
		}
	}
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
				player_condition_type ChoiceMade = PlayerCondition::Unactive;
				std::cout << "You have given: " << player.lastRaise << std::endl;
				std::cout << "Last raise is: " << deal.lastGameRaise << std::endl << std::endl;
				std::cout << player.cardsAndPointsDisplay << std::endl;

				if (isFirst)
				{
					std::cout << player.name << " raise or fold? r/f: ";
					ChoiceMade = PlayerCondition::Raise | PlayerCondition::Fold;
				}
				else if (deal.currentMaxRaise <= deal.lastGameRaise)
				{
					std::cout << player.name << " call or fold? c/f: ";
					ChoiceMade = PlayerCondition::Fold | PlayerCondition::Call;
				}
				else
				{
					std::cout << player.name << " raise, call or fold? r/c/f: ";
					ChoiceMade = PlayerCondition::Raise | PlayerCondition::Fold | PlayerCondition::Call;
				}

				std::string s;
				std::getline(std::cin, s);
				std::cout << std::endl;

				if ((s == "f" || s == "F") && (ChoiceMade & PlayerCondition::Fold) == PlayerCondition::Fold)
				{
					player.isPlayerActive = PlayerCondition::Fold;
					isCorrect = true;
				}
				else if ((s == "c" || s == "C") && !isFirst && (ChoiceMade & PlayerCondition::Call) == PlayerCondition::Call)
				{
					currentCall++;
					int lastPlayerRaise = player.lastRaise;
					int pays = deal.lastGameRaise - lastPlayerRaise;

					player.lastRaise = deal.lastGameRaise;

					player.chips -= pays;
					deal.pot += pays;
					isCorrect = true;
				}
				else if ((s == "r" || s == "R") && (ChoiceMade & PlayerCondition::Raise) == PlayerCondition::Raise)
				{
					bool isCorrectPay = false;
					int paymentAmount;

					while (!isCorrectPay)
					{
						std::cout << player.name << " pay: (" << deal.lastGameRaise + CHIP_VALUE << " - " << deal.currentMaxRaise << "): ";
						std::getline(std::cin, s);
						std::cout << std::endl;

						try
						{
							paymentAmount = stoi(s);
							if ((paymentAmount >= deal.lastGameRaise + CHIP_VALUE) && paymentAmount <= deal.currentMaxRaise)
							{
								isCorrectPay = true;
							}
						}
						catch (const std::exception&)
						{
							isCorrectPay = false;
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

void DeterminingWinner(Player players[], Deal& deal)
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

	int winnersCount = 0;
	int lastWinnerIdx = -1;

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
				lastWinnerIdx = i;
			}
		}
	}

	if (winnersCount > 1)
	{
		int halfPot = std::ceil(1.0 * deal.pot / 2);

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
						std::cout << player.name << " will you pay half pot? y/n:";
						std::string s;
						std::getline(std::cin, s);
						std::cout << std::endl << std::endl;

						if (s == "y" || s == "Y")
						{
							player.chips -= halfPot;
							deal.pot += halfPot;
							player.isPlayerActive = PlayerCondition::Active;
							player.lastRaise = 0;
							isCorrect = true;
						}
						else if (s == "n" || s == "N")
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
		Player& winner = players[lastWinnerIdx];

		winner.chips += deal.pot;
		winner.lastRaise = 0;
		winner.isPlayerActive = PlayerCondition::Active;
		deal.pot = 0;
		deal.lastGameRaise = 0;

		std::cout << winner.name << " is winner." << std::endl << std::endl;
		DisplayPlayersInDeal(players);
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
	Card* cardDeck = new Card[CARDS_IN_DECK];

	SetUpCardDeck(cardDeck);
	int currentDeckCount = CARDS_IN_DECK;
	int* currentDeckCountPtr = &currentDeckCount;

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

			std::cout << player.name << ": " << player.chips << std::endl << std::endl;

			SetCards(player, cardDeck, currentDeckCount);
		}
	}

	delete[] cardDeck;
	cardDeck = nullptr;

	return result;
}

void ClearDeal(Deal& deal)
{
	if (deal.dealFlag)
	{
		deal.pot = 0;
	}

	deal.lastGameRaise = 0;
}