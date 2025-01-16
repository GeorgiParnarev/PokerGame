#pragma once

#include "GlobalTypes.h"

enum PlayerCondition
{
	Inactive = 0,
	Active = 0b00000001,
	Fold = Active << 1,
	Call = Fold << 1,
	Raise = Call << 1,
	Hold = Raise << 1
};

enum GameCondition
{
	Win,
	Continue,
	DealEnd,
	End
};

enum FileCondition
{
	OK,
	Error
};

const int POT_STARTING_AMOUNT = 0;
const int NO_RAISE_AMOUNT = 0;

/// <summary>
/// Enum representing the pip (rank) of a card.
/// These are bitwise values used for encoding and identifying card ranks.
/// </summary>
enum Pip
{
	N7 = 0b100000000,       ///< Represents the 7 pip (rank) in the card encoding.
	N8 = N7 << 1,           ///< Represents the 8 pip.
	N9 = N8 << 1,           ///< Represents the 9 pip.
	N10 = N9 << 1,          ///< Represents the 10 pip.
	J = N10 << 1,           ///< Represents the Jack pip.
	Q = J << 1,             ///< Represents the Queen pip.
	K = Q << 1,             ///< Represents the King pip.
	A = K << 1,             ///< Represents the Ace pip.
	PipMask = 0b1111111100000000 ///< Mask for extracting the pip value from a card.
};

/// <summary>
/// Enum representing the suit of a card.
/// These are bitwise values used for encoding and identifying card suits.
/// </summary>
enum Suit
{
	Clubs = 0b10000,        ///< Represents the Clubs suit.
	Diamonds = Clubs << 1,  ///< Represents the Diamonds suit.
	Hearts = Diamonds << 1, ///< Represents the Hearts suit.
	Spades = Hearts << 1,   ///< Represents the Spades suit.
	SuitMask = 0b11110000   ///< Mask for extracting the suit value from a card.
};

/// <summary>
/// Enum representing the score or rank of a card.
/// These values are used in scoring and comparisons during gameplay.
/// </summary>
enum Rank
{
	Ace = 11,               ///< Value of an Ace card in points.
	King = 10,              ///< Value of a King card in points.
	Queen = 10,             ///< Value of a Queen card in points.
	Jack = 10,              ///< Value of a Jack card in points.
	Ten = 10,               ///< Value of a Ten card in points.
	Nine = 9,               ///< Value of a Nine card in points.
	Eight = 8,              ///< Value of an Eight card in points.
	Seven = 7,              ///< Value of a Seven card in points.
	RankMask = 0b1111,      ///< Mask for extracting the rank value from a card.
	NegativeRankMask = ~RankMask ///< Mask for clearing the rank portion of a card.
};

// Constants for game configuration and rules.

/// <summary>Minimum number of players required for the game.</summary>
const int MIN_PLAYERS = 2;

/// <summary>Maximum number of players allowed in the game.</summary>
const int MAX_PLAYERS = 9;

const char WARNING[] = "Enter correct data!";
const char FILE_NAME[] = "pockergame.txt";

const bool DEAL_PLAY = true;
const bool NOT_DEAL_PLAY = false;

const bool FIRST_DEAL = true;
const bool CONTINUE_DEAL = false;

/// <summary>Value of a single chip in the game.</summary>
const int CHIP_VALUE = 10;

/// <summary>Total number of chips each player starts with in the game.</summary>
const int NUMBER_OF_CHIPS = 100;

const int CHIPS_ADD_VALUE = 50;

/// <summary>Default points assigned to a player based on their initial set of cards.</summary>
const int STARTING_POINTS = 0;

// Flags for game states and properties.
const bool SEVEN_CLUBS_PRESENT = true;       ///< Indicates if the Seven of Clubs is present in the deck.
const bool SEVEN_CLUBS_NOT_PRESENT = false; ///< Indicates if the Seven of Clubs is not present in the deck.

const bool ACTIVE_PLAYER = true;   ///< Indicates an active player.
const bool INACTIVE_PLAYER = false; ///< Indicates an inactive player.

// Scoring and game logic constants.

const int MAX_POINTS = 34;          ///< Maximum points a player can achieve.

const bool IS_ACE_OR_SEVEN_PAIR = true; ///< Indicates if a pair consists of Aces or Sevens.
const bool IS_NOT_ACE_OR_SEVEN_PAIR = false; ///< Indicates if a pair does not consist of Aces or Sevens.

const int PAIR_CARD_AMOUNT = 2;     ///< Number of cards in a pair.
const int PAIR_ACE_POINTS = 22;     ///< Points for a pair of Aces.
const int PAIR_SEVEN_POINTS = 23;   ///< Points for a pair of Sevens.

const int SEVEN_OF_CLUBS_POINTS = 11; ///< Points for the Seven of Clubs.

const bool IS_SAME_SUIT = true;      ///< Indicates if two cards are of the same suit.
const bool IS_NOT_SAME_SUIT = false; ///< Indicates if two cards are not of the same suit.

const bool IS_SAME_KIND = true;      ///< Indicates if two cards are of the same rank.
const bool IS_NOT_SAME_KIND = false; ///< Indicates if two cards are not of the same rank.

/// <summary>
/// Number of rows in the pips matrix (2 rows: one for the pip values, one for the rank values).
/// </summary>
const size_t PIPS_ROW = 2;

/// <summary>
/// Number of columns in the pips matrix (8 columns: one for each card rank, 7-ace).
/// </summary>
const size_t CARDS_COL = 8;

/// <summary>
/// Number of suits in the game (4 suits: Clubs, Diamonds, Hearts, Spades).
/// </summary>
const size_t SUITS = 4;

const int CARDS_COUNT = 3;						   ///< Number of cards dealt to players.
const int CARDS_IN_DECK = SUITS * CARDS_COL;       ///< Number of cards in the deck.

const int CARDS_DISPLAY_MAX_SIZE = 20;
const int CARD_STRING_MAX_SIZE = 4;

/// <summary>
/// Represents a matrix of pips and corresponding rank values.
/// The first row contains bitwise pip values, and the second row contains rank values.
/// Used for initializing the deck of cards before each dealing.
/// </summary>
const card_type pipsMatrix[PIPS_ROW][CARDS_COL] =
{
	{Pip::N7, Pip::N8, Pip::N9, Pip::N10, Pip::J, Pip::Q, Pip::K, Pip::A},
	{Rank::Seven, Rank::Eight, Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen, Rank::King, Rank::Ace}
};

/// <summary>
/// Represents an array of suits used in the game.
/// Used for initializing the deck of cards before each dealing.
/// </summary>
const card_type suits[SUITS] = { Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades };