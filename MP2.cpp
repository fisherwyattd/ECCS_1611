// MP2.cpp - Wyatt Fisher - 1 October 2020
// This program is a virtual version of the two player dice game "Pig."
// The computer will be playing against a human who will input whether or not they want to keep rolling during their turn.
// The first person (or computer) to reach 100 points first wins (following the regular scoring and rules of the dice game).

#include<iostream>
#include<iomanip>
#include<cmath>
#include<string>
#include<math.h>
#include<Windows.h>
using namespace std;

// Function prototypes:
void beginGame(void);
void runGame(void);
int rollDie(void);
bool isTurnScoreLost(int die1value, int die2value);
bool isGameScoreLost(int die1value, int die2value);
char getUserInput(void);
int computerTurn(void);
int playerTurn(void);
void displayRollVals(int die1value, int die2value);

// Global variable declarations:
int computerScore = 0;
int playerScore = 0;
int die1value = 0;
int die2value = 0;
int rollCount = 0;
char userInput = 'z';
char whichTurn = 'P';


int main() {
	// include random number generator
	srand(time(0));

	// call to begin the game
	beginGame();

	// call the function in charge of running the game
	runGame();	

	return 0;
}


/**
beginGame Function - perform initial startup of game (print out a welcome message)
*/

void beginGame(void) {
	// print out welcome message
	cout << "Hello and welcome to Roll 'em Pigs!" << endl;
	cout << endl << "-------------------------------------" << endl;
}


/**
runGame Function - perform the running of the game, essentially a switch statement for human vs. computer turn
*/

void runGame(void) {
	while (computerScore < 100 && playerScore < 100) { // while no one has 100 or more points (no one won the game yet)
		switch (whichTurn) {
		case 'P':
			// this is the player's turn
			playerScore = playerTurn();
			cout << endl << "-------------------------------------" << endl;

			if (playerScore < 100) { // if the player did not reach 100 or more points in that turn
				// it is the computer's turn then
				whichTurn = 'C';

				// pause so the player can read the output
				Sleep(2000);
			}
			else { // if the player won the game in that turn
				// the game is over
				break;
			}

		case 'C':
			// this is the computer's turn
			computerScore = computerTurn();
			cout << endl << "-------------------------------------" << endl;

			if (computerScore < 100) { // if the computer did not win the game in that turn
				// it is the player's turn then
				whichTurn = 'P';
			}
			else { // if the computer won the game in that turn
				// the game is over
				break;
			}
		}
	}
}


/**
rollDie Function - perform the rolling of an individual die (from 1 to 6) using a random number generator
@return rollResult - returns the resulting integer of the "roll"
*/

int rollDie(void) {
	int rollResult = 0;
	
	// generate random number between 1-6
	rollResult = (1 + rand() % 6);
	return rollResult;
}


/**
isTurnScoreLost Function - takes in both dice values and determines if either causes the player to lose their turn score
@param die1value - value of the first die
@param die2value - value of the second die
@return turnScoreLost - returns a boolean value of whether or not the turn score has been lost
*/

bool isTurnScoreLost(int die1value, int die2value) {
	// initialize the bool to false
	bool turnScoreLost = false;
	if ((die1value == 1 && die2value != 1) || (die2value == 1 && die1value != 1)) { // if only one of the dice is a one
		// the player/computer loses its turn score
		turnScoreLost = true;
	}

	return turnScoreLost;
}


/**
isGameScoreLost Function - takes in both dice values and determines if they cause the player to lose their game score
@param die1value - value of the first die
@param die2value - value of the second die
@return gameScoreLost - returns a boolean value of whether or not the turn score has been lost
*/

bool isGameScoreLost(int die1value, int die2value) {
	// initialize the bool to false
	bool gameScoreLost = false;
	if (die1value == 1 && die2value == 1) { // if the values of both die are one
		// the player/computer loses its game score
		gameScoreLost = true;
	}

	return gameScoreLost;
}


/**
getUserInput Function - gets input from the user as to whether or not they want to keep rolling the dice
@return userChar - returns either 'r' or 'R' to continue rolling or 's' or 'S' to stop
*/

char getUserInput(void) {
	if (rollCount == 0) { // if this is the beginning of the player's turn, they must roll
		do { // ask for user input until they enter 'R' to roll
			cout << endl << "Press 'R' to begin your turn: ";
			cin >> userInput;

			// force user input to uppercase
			userInput = toupper(userInput);
		} while (userInput != 'R');

		if (userInput == 'R') {
			// roll the dice and add one to the roll count (not the beginning of the turn anymore)
			rollCount++;
		}
	}
	else if (rollCount > 0) { // if it is not the beginning of the player's turn, they can choose to roll or stop
		do { // ask for user input until they enter either a 'R' or 'S'
			cout << endl << "Continue rolling? Press 'R' to continue, 'S' to stop: ";
			cin >> userInput;

			// force user input to uppercase
			userInput = toupper(userInput);
		} while (userInput != 'S' && userInput != 'R');
	}

	return userInput;
}


/**
computerTurn Function - this is the function that will allow the computer to play the game
@return compTurnScore - returns the score the computer got during that turn
*/

int computerTurn(void) {
	// initialize turn score
	int compTurnScore = 0;

	// initialize temp score to determine the potential game score if the turn is stopped
	int tempComputerScore = 0;

	while (compTurnScore < 25) { // the computer must stop rolling once it gets 25 or more points in a turn
		// roll the dice
		die1value = rollDie();
		die2value = rollDie();

		// print the resulting die values to the console
		displayRollVals(die1value, die2value);

		// check to see if either the turn or game scores are lost
		bool compTurnScoreLost = isTurnScoreLost(die1value, die2value);
		bool compGameScoreLost = isGameScoreLost(die1value, die2value);

		// update the turn score
		compTurnScore = compTurnScore + die1value + die2value;

		// calculate the game score if the computer would stop its turn now
		tempComputerScore = computerScore + compTurnScore;

		if (compTurnScoreLost) { // if the computer rolled a one
			// the computer loses its turn score
			compTurnScore = 0;
			cout << endl << "I rolled a one. I lost my turn score." << endl;
			break;
		}
		else if (compGameScoreLost) { // if the computer rolled two ones
			// the computer loses its turn and game scores
			compTurnScore = 0;
			computerScore = 0;
			cout << endl << "I rolled two ones. I lost my game score." << endl;
			break;
		}
		else if (tempComputerScore >= 100) { // if the computer gets 100 or more total points
			// its game score equals the potential score
			computerScore = tempComputerScore;

			// force the computer to end its turn and win the game
			cout << endl << "My final score is " << computerScore << ". I win!" << endl;
			cout << "You had " << playerScore << " points.";
			break;
		}
		// pause to let the player read the console
		Sleep(1500);
	}

	if (tempComputerScore < 100) { // if the computer cannot win in this turn
		// add the turn score to the total score
		computerScore += compTurnScore;
		cout << endl << "I got " << compTurnScore << " points that turn! My total score is " << computerScore << ". Your turn!" << endl;
		return computerScore;
	}
}


/**
playerTurn Function - this is the function that will allow the human to play the game
@return playerTurnScore - returns the score the human got during that turn
*/

int playerTurn(void) {
	// initialize turn score
	int playerTurnScore = 0;

	// initialize temp score to determine the potential game score if the turn is stopped
	int tempPlayerScore = 0;

	// clear the score lost bools
	bool playerTurnScoreLost = false;
	bool playerGameScoreLost = false;

	// ask the user to roll
	userInput = getUserInput();

	while (userInput == 'R') { // while the user wants to roll the dice
		// roll the dice
		die1value = rollDie();
		die2value = rollDie();

		// print the resulting die values to the console
		displayRollVals(die1value, die2value);

		// check to see if either the turn or game scores are lost
		playerTurnScoreLost = isTurnScoreLost(die1value, die2value);
		playerGameScoreLost = isGameScoreLost(die1value, die2value);

		// update the turn score
		playerTurnScore = playerTurnScore + die1value + die2value;

		// calculate the game score if the player would stop their turn now
		tempPlayerScore = playerScore + playerTurnScore;

		if (playerTurnScoreLost) { // if the player rolled a one
			// the player loses their turn score
			playerTurnScore = 0;
			cout << endl << "You rolled a one. You lost your turn score." << endl;
			break;
		}
		else if (playerGameScoreLost) { // if the player rolled two ones
			// the player loses their turn and game scores
			playerTurnScore = 0;
			playerScore = 0;
			cout << endl << "You rolled two ones. You lost your game score." << endl;
			break;
		}
		else if (tempPlayerScore >= 100) { // if the player gets 100 or more total points
			// their game score equals the potential score
			playerScore = tempPlayerScore;

			// force the player to end their turn and win the game
			cout << endl << "Your final score is " << playerScore << ". You win!" << endl;
			cout << "I had " << computerScore << " points.";
			break;
		}
		// ask the user if they want to roll again
		userInput = getUserInput();
	}

	if (tempPlayerScore < 100) { // if the player cannot win this turn
		// add the turn score to the total score
		playerScore += playerTurnScore;
		cout << endl << "You got " << playerTurnScore << " points that turn! Your total score is " << playerScore << ". My turn!" << endl;
		rollCount = 0;
		return playerScore;
	}
}


/**
displayRollVals Function - this function will display the values "rolled" by the dice
@param die1value - value of the first die
@param die2value - value of the second die
*/

void displayRollVals(int die1value, int die2value) {
	// print "Rolling..." and pause for dramatic effect
	cout << endl << "Rolling..." << endl;
	Sleep(1000);
	cout << endl << "Die one value: " << die1value << endl;
	Sleep(500);
	cout << "Die two value: " << die2value << endl;
}