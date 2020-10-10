// MP3.cpp - Wyatt Fisher - 1 October 2020
// This program will allow two people to play the game of tic-tac-toe.
// The program will ask for moves alternately from each player, one of whom always plays the “crosses” (‘X’)
// while the other always plays the “circles” (‘O’) regardless of the number of games played.
// The program will display the gameboard positions, which are similar to the telephone keypad layout.

#include<iostream>
#include<string>
using namespace std;

// global variables
const int SIZE = 3;


// function prototypes and definitions

/**
gameSetup Function - displays a welcome message to the console and has players enter user names
@param player1Name - takes in player1Name to rewrite with player 1 input
@param player2Name - takes in player2Name to rewrite with player 2 input
*/
void gameSetup(string &player1Name, string &player2Name) {
	// display greeting message
	cout << "Welcome to Tic-Tac-Toe!" << endl << endl;
	cout << "Please enter user names to begin." << endl << "---------------------------------" << endl;

	// ask for players to enter user names
	cout << "Player 1: ";
	getline(cin, player1Name);
	cout << "Thanks, " << player1Name << "! Your game piece will be 'x'." << endl << endl;
	cout << "Player 2: ";
	getline(cin, player2Name);
	cout << "Thanks, " << player2Name << "! Your game piece will be 'o'." << endl;
}


/**
getPlayerInput Function - gets the int value for the location the specific player wants to place their game piece
@param playerName - takes in the player name to know which game piece to use and whose turn it is
@return playerInput - returns the int location (between 1-9, inclusive) where the player wishes to place their game piece
*/
int getPlayerInput(string playerName) {
	int playerInput = 0;
	do {
		cout << playerName << ", please enter where you would like to place your game piece: ";
		cin >> playerInput;
	} while (playerInput < 1 && playerInput > 9);

	return playerInput;
}


/**
isLegalMove Function - checks to see if the move the player wishes to make is a legal move (sees if that location on the board is open)
@param board[SIZE][SIZE] - takes in the current board setup array
@param location - takes in the player's selected piece location
@return legalMove - returns true if the indicated location on the board yet not yet been played, false otherwise
*/
bool isLegalMove(char board[SIZE][SIZE], int location) {
	int tempLocation = location - 1;
	char arrayVal = board[tempLocation / 3][tempLocation % 3];
	if (arrayVal == 'x' || arrayVal == 'o') {
		return false;
	}
	else {
		return true;
	}
}


/**
placeMarkOnBoard Function - places the mark at the specified board location, assuming it is legal
@param board[SIZE][SIZE] - takes in the current board array
@param playerMark - takes in which mark belongs to the player wishing to place their mark on the board
@param location - takes in the desired location for the mark to be placed
*/
void placeMarkOnBoard(char board[SIZE][SIZE], char playerMark, int location) {
	int tempLocation = location - 1;
	board[tempLocation / 3][tempLocation % 3] = playerMark;
}


/**
clearBoard Function - resets the board so that it will display the numbers 1-9 to begin a new game
@param board[SIZE][SIZE] - takes in the size of the board to be displayed
*/
void clearBoard(char board[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			int boardVal = ((i * 3) + j + 1);
			string boardStr = to_string(boardVal);
			char boardChar = boardStr[0];
			board[i][j] = { boardChar };
		}
	}
}


/**
hasThreeInRow Function - checks to see if either player has three marks in a row (to see if someone has won the game)
@param board[SIZE][SIZE] - takes in the board in its current state
@param playerMark - takes in the current player's mark to see if they have won
@return hasThreeInRow - returns true if the current player has three marks in a row and therefore won the game, false otherwise
*/
bool hasThreeInRow(char board[SIZE][SIZE], char playerMark) {
	int horizontalMatches = 0;
	int verticalMatches = 0;
	int upToDownDiagonalMatches = 0;
	int downToUpDiagonalMatches = 0;
	bool threeInRow = false;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (playerMark == board[i][j]) { horizontalMatches++; }
			if (playerMark == board[j][i]) { verticalMatches++; }
		}

		if (playerMark == board[i][i]) { upToDownDiagonalMatches++; }
		if (playerMark == board[i][-i + 2]) { downToUpDiagonalMatches++; }

		if (horizontalMatches == 3) { threeInRow = true; }
		else if (verticalMatches == 3) { threeInRow = true; }
		else if (upToDownDiagonalMatches == 3 || downToUpDiagonalMatches == 3) { threeInRow = true; }

		horizontalMatches = 0;
		verticalMatches = 0;
	}

	return threeInRow;
}


/**
displayBoard Function - displays the game board to the console
@param board[SIZE][SIZE] - takes in the size/layout of the board to be displayed
*/
void displayBoard(char board[SIZE][SIZE]) {
	cout << endl;
	for (int i = 0; i < SIZE; i++) {
		if (i > 0) {
			for (int k = 0; k < SIZE; k++) {
				if (k == 0) {
					cout << "---";
				}
				else {
					cout << "+---";
				}
			}
			cout << endl;
		}
		for (int j = 0; j < SIZE; j++) {
			if (j == 0) {
				cout << " " << board[i][j] << " ";
			}
			else {
				cout << "| " << board[i][j] << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
}


/**
displayGameStats Function - prints the game stats to the console (games tied, games won)
@param ties - number of ties so far
@param player1Score - number of games player 1 has won
@param player2Score - number of games player 2 has won
@param player1Name - user name of player 1
@param player2Name - user name of player 2
*/
void displayGameStats(int ties, int player1Score, int player2Score, string player1Name, string player2Name) {
	cout << endl << "Game Stats" << endl << "__________________________" << endl;
	cout << "Number of ties: " << ties << endl;
	cout << player1Name << " - number of wins: " << player1Score << endl;
	cout << player2Name << " - number of wins: " << player2Score << endl;
}


int main() {
	// initialize game board array with given size (3 by 3)
	char board[SIZE][SIZE] = {};

	// keep track of when each game finishes to keep score and see if players want to play again
	bool gameFinished = false;
	char continuePlaying = 'Y';

	// keep track of game and turn counts to know whose turn it is
	int gameCount = 0;
	int oddOrEvenGame = 0;
	int turnCount = 0;
	int oddOrEvenTurn = 0;

	// keep track of score (ties and games won)
	int gameTies = 0;
	int player1Score = 0;
	int player2Score = 0;

	// initialize player names and game pieces
	string player1Name = " ";
	string player2Name = " ";
	char currentPlayerMark[2][2] = { { 'x', 'o' }, {'o', 'x'} };

	// initialize game board and get user names
	gameSetup(player1Name, player2Name);

	// use an array to keep track of whose turn it is (alternates between turns and switches each game)
	string currentPlayerName[2][2] = { {player1Name, player2Name}, {player2Name, player1Name} };

	do {
		// game just starting, so initialize this bool to false
		gameFinished = false;

		// reset and display the game board
		clearBoard(board);
		displayBoard(board);

		do {
			// find out if the turn is odd or even (used to determine whose turn it is)
			oddOrEvenTurn = (turnCount % 2);

			bool legalMove;
			int pieceLocation;

			do { // get player input until they enter a legal location
				pieceLocation = getPlayerInput(currentPlayerName[oddOrEvenGame][oddOrEvenTurn]);
				legalMove = isLegalMove(board, pieceLocation);
			} while (!legalMove);

			placeMarkOnBoard(board, currentPlayerMark[oddOrEvenGame][oddOrEvenTurn], pieceLocation);

			displayBoard(board);

			// determine if that move was the winning move
			if (hasThreeInRow(board, currentPlayerMark[oddOrEvenGame][oddOrEvenTurn])) {
				gameFinished = true;
				if ((oddOrEvenGame + oddOrEvenTurn) == 1) { // if player 2 wins, congratulate them and update the score
					player2Score++;
					cout << "Congratulations, " << player2Name << "! You won!" << endl;
				}
				else { // if player 1 wins, congratulate them and update the score
					player1Score++;
					cout << "Congratulations, " << player1Name << "! You won!" << endl;
				}
			}

			// initialize the total number of possible legal moves
			int legalMovesLeft = 9;

			// check to see if there are legal moves (open spaces on the board) left
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					if (board[i][j] == 'x' || board[i][j] == 'o') { legalMovesLeft--; }
				}
			}

			// if the board is full (no possible moves left), the game is over and counts as a tie
			if (legalMovesLeft == 0) {
				gameFinished = true;
				gameTies++;
				cout << endl << "No more legal moves left. Tie game!" << endl;
			}

			// update the turn count after each turn
			turnCount++;

		} while (!gameFinished);

		displayGameStats(gameTies, player1Score, player2Score, player1Name, player2Name);

		// reset the turn count for another game
		turnCount = 0;

		// update the game count
		gameCount++;
		oddOrEvenGame = (gameCount % 2);

		do { // ask the players if they want to play another until they enter a valid input
			cout << endl << "Play another? (Y/N): ";
			cin >> continuePlaying;
			continuePlaying = toupper(continuePlaying);
		} while (continuePlaying != 'Y' && continuePlaying != 'N');

	} while (continuePlaying == 'Y');
	
	cout << endl << "Thanks for playing!" << endl;

	return 0;
}