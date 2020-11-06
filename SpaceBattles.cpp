// SpaceBattles.cpp - Wyatt Fisher - 16 October 2020
// This is a single-player, Star Wars knock-off, text-based game.
// There will be four characters the player can choose from that have slighty different abilities and health.
// The goal of the game is to find all the pieces of the plans for the Death Globe
// and return to the starting room after defeating Dirth Nader.

#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<ctime>
#include<Windows.h>
#include<vector>
using namespace std;


// global variables
string intInput;			// Global Variable
string strInput;
int DELAY = 30;				// delay time for scrolling text
const int CHARS = 5;
const int INTRO = 6;		// location of intro text in .txt file
bool deathGlobe = false;	// turns true once Dirth Nader finds the player
bool fromCombat = false;	// turns true when the player is just exiting combat
bool gameOver = false;


// structs
struct room { // Struct
	vector<string> descriptions; // An Array or a Vector
	vector<string> items;
	vector<string> directions;
	vector<int> adjRooms;
};

struct player {
	int health = 0;
	string character;
	vector<string> bodyParts = { "HAND", "HAND", "ARM", "ARM", "FOOT", "FOOT", "LEG", "LEG" };
	vector<string> inventory;	
	vector<string> wearing;
};

// initialize players using player struct
player player1, dirthNader;


// function prototypes
void changeColor(void);
void printLines(int num);
void getIntInput(int num, int options);
void getStrInput(void);
void clear(void);
string pickChar(string choice);
char mainMenu(char& gamePath);
char gameIntro(char& gamePath);
char helpMenu(char& gamePath, char prevCase);
char runActions(room rooms[], int& currentRoom, int& nextRoom, int& prevRoom, char& gamePath);
char midGameMenu(char& gamePath);
char statsInventory(char& gamePath, room rooms[], int num);
void createRoom(room rooms[], int num);
void enterRoom(room rooms[], int num, char& gamePath, int prevRoom, int& nextRoom);
void roomActions(room rooms[], int num, int& nextRoom);
void displayItems(room rooms[], int num, int a);
void printSpecial(string temp);
void fight(room rooms[], int num, int& nextRoom);


int main() {
	// change color of instructional text to match the Star Wars theme
	changeColor(); // Function Definition and Function Call

	// gamePath and prevCase are responsible for running of game and navigation between menus
	char gamePath = '0';
	char prevCase = '0';

	// always start in room 7, keep track of previous room, initialize array of 30 possible rooms
	int currentRoom = 7;
	int prevRoom = 8;
	int nextRoom = 7;
	const int TOTAL_ROOMS = 30;
	room rooms[TOTAL_ROOMS];

	// initialize other variables
	dirthNader.health = 50;
	
	while (!gameOver) {
		switch (gamePath) { // Switch / Cast Statement
		case '0':
			// print welcome screen and main menu
			prevCase = mainMenu(gamePath); // Function Definition with Passed by Reference Parameters and Function Call
			break;

		case '1':
			// display game intro and get player character choice
			prevCase = gameIntro(gamePath);
			break;

		case '2':
			// display help menu
			prevCase = helpMenu(gamePath, prevCase);
			break;

		case '3':
			// exit game
			gameOver = true;

		case '4':
			// if player enters room for first time, create it
			prevCase = runActions(rooms, currentRoom, nextRoom, prevRoom, gamePath);
			break;

		case '5':
			// print the mid-game menu and get user input
			prevCase = midGameMenu(gamePath);
			break;

		case'6':
			// displays the player's inventory and stats, then goes back to the mid-game menu
			prevCase = statsInventory(gamePath, rooms, currentRoom);
			// Function Definition with 1D or 2D Array as a Parameter and Function Call (above)
			break;
		}
	}
	
	return 0;
}


/**
changeColor Function - changes the color of the console output to match the Star Wars theme
*/
void changeColor(void) { // Function Definition and Function Call
	// from Stack Overflow
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int col = 14;
	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, col);
}


/**
printLines Function - searches .txt file with the story to find appropriate text for program to print to console
@param num - section number to search for in the file
*/
void printLines(int num) { // Function Definition with Parameters and Function Call
	ifstream text;
	string line;
	string searchBeg = to_string(num) + "Start";
	string searchEnd = to_string(num) + "End";
	int delay = 0;
	bool start = false;
	bool end = false;

	// read in information from the .txt file containing the main story of the game
	text.open("StoryText.txt"); // File Reading
	while (getline(text, line)) {
		// set start flag to true once starting keyword is found
		if (line.find(searchBeg, 0) != string::npos) { start = true; }

		// set end flag to true once ending keyword is found
		if (line.find(searchEnd, 0) != string::npos) { end = true; }

		if (start && !end) { // If Statement
			 // skip over starting keyword line
			if (delay == 1) { printSpecial(line); }
			else { delay++; }
		}
	}
	text.close();
}


/**
getIntInput Function - prints out instructions and options to the player and takes in only integer inputs
@param num - section number containing lines to be printed out
@param options - number of options the user has to choose between
*/
void getIntInput(int num, int options) {
	// temp char for verifying that the user is not just spamming the enter key
	char temp = '\n';

	// default case when only input needs to be verified (nothing printed to console)
	if (num == 1000) {
		while (true) {
			// reset the temp value so it is always read in
			temp = '\n';

			// keep taking in input until spammed enter keys have been ignored (if there were any)
			while (temp == '\n') { cin.get(temp); }

			// get the rest of the input once enter keys have been ignored
			getline(cin, intInput);

			// add temp char to the beginning of the input
			intInput = temp + intInput;

			if (options == 2) {
				if (intInput != "1" && intInput != "2") {
					printSpecial("Please enter either 1 or 2: ");
				}
				else { break; }
			}
			else if (options == 3) {
				if (intInput != "1" && intInput != "2" && intInput != "3") {
					printSpecial("Please enter either 1, 2 or 3: ");
				}
				else { break; }
			}
			else if (options == 4) {
				if (intInput != "1" && intInput != "2" && intInput != "3" && intInput != "4") {
					printSpecial("Please enter either 1, 2, 3 or 4: ");
				}
				else { break; }
			}
		}
	}

	// when lines need to be printed before player is prompted for input
	else {
		printLines(num); // Function Definition with Parameters and Function Call
		while (true) {
			// reset the temp value so it is always read in
			temp = '\n';

			// keep taking in input until spammed enter keys have been ignored (if there were any)
			while (temp == '\n') { cin.get(temp); }

			// get the rest of the input once enter keys have been ignored
			getline(cin, intInput);

			// add temp char to the beginning of the input
			intInput = temp + intInput;

			if (options == 2) {
				if (intInput != "1" && intInput != "2") {
					printSpecial("Please enter either 1 or 2: ");
				}
				else { break; }
			}
			else if (options == 3) {
				if (intInput != "1" && intInput != "2" && intInput != "3") {
					printSpecial("Please enter either 1, 2 or 3: ");
				}
				else { break; }
			}
			else if (options == 4) {
				if (intInput != "1" && intInput != "2" && intInput != "3" && intInput != "4") {
					printSpecial("Please enter either 1, 2, 3 or 4: ");
				}
				else { break; }
			}
		}
		printSpecial("-");
	}
}


/**
getStrInput Function - gets string input from user and converts to all uppercase characters
*/
void getStrInput(void) {
	// temp char for verifying that the user is not just spamming the enter key
	char temp = '\n';

	// keep taking in input until spammed enter keys have been ignored (if there were any)
	while (temp == '\n') { cin.get(temp); }

	// get the rest of the input once enter keys have been ignored
	getline(cin, strInput);

	// add temp char to the beginning of the input
	strInput = temp + strInput;

	// iterate through string to uppercase all chars
	for (int m = 0; m < strInput.length(); m++) { strInput[m] = toupper(strInput[m]); }
}


/**
clear Function - clears the console screen to give the appearance of changing locations, menus, etc.
*/
void clear(void) {
	// clears the screen for effect, from Stack Overflow
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}


/** // 2 Styles of Comments
pickChar Function - this function will use player input to determine which character the user wishes to play as
@param choice - the number selection the player made using getIntInput
@return the string name of the game character the player chooses
*/
string pickChar(string choice) { // Function Definition with Return Value and Function Call
	string name;

	// depending on player's choice, set their game character name (hard-coded for now)
	if (choice == "1") {
		name = "Luke Airjogger"; // Assignment Statement
		player1.health = 30;
	}
	else if (choice == "2") {
		name = "Han Single";
		player1.health = 40;
	}
	else if (choice == "3") {
		name = "R3-D5";

		// R3D5 does not have arms or legs to use in the game // 2 Styles of Comments
		player1.bodyParts.clear();
		player1.health = 60;
	}
	else if (choice == "4") {
		name = "Princess Layup";
		player1.health = 30;
	}

	return name;
}


/**
mainMenu Function - displays the main menu to the user
@param gamePath - sets the next step in the game path to what the player wants to do next
@return this case as the previous case ('0')
*/
char mainMenu(char& gamePath) { // Function Definition with Passed by Reference Parameters and Function Call
	getIntInput(0, 3);
	gamePath = intInput[0];

	return '0';
}


/**
gameIntro Function - sets up the beginning of the game and allows the player to pick a character
@param gamePath - sets the next step in the game path to what the player wants to do next
@return this case as the previous case ('2')
*/
char gameIntro(char& gamePath) {
	clear();

	// ask player to pick a character, then display intro text for game
	getIntInput(CHARS, 4);
	player1.character = pickChar(intInput); // Function Definition with Return Value and Function Call
	clear();
	printLines(INTRO);

	char temp = 'x';
	while (temp != '\n') {
		cout << "\nPress enter to continue... ";
		cin.get(temp);
	}
	clear();

	// set game path to 4... this is path where player navigates between rooms and does actions inside them
	gamePath = '4';

	return '1';
}


/**
helpMenu Function - displays the help menu to the user
@param gamePath - sets the next step in the game path to what the player wants to do next
@param prevCase - uses previous case to determine where to send game navigation next
@return this case as the previous case ('2')
*/
char helpMenu(char& gamePath, char prevCase) {
	clear();

	// display the help menu from .txt file
	printLines(255); // A Function that Calls Another Function (main not included)
	printSpecial("\n1. Return to menu\n2. Exit game\n");
	getIntInput(1000, 2);

	// check to see where player came from to determine if game should be resumed or started for first time
	if (intInput == "1" && prevCase == '0') { gamePath = '0'; clear(); }
	else if (intInput == "1" && prevCase == '5') { gamePath = '5'; clear(); }
	else { gamePath = '3'; }

	return '2';
}


/**
runActions Function - responsible for calling the creating and entering of rooms
@param rooms - struct for rooms in this game
@param currentRoom - room number the player is entering
@param nextRoom - room number the player will be entering next (based on their input)
@param prevRoom - room number the player is coming from
@param gamePath - sets the next step in the game path to what the player wants to do next
@return this case as the previous case ('4')
*/
char runActions(room rooms[], int& currentRoom, int& nextRoom, int& prevRoom, char& gamePath) {
	// create room if it is being entered for the first time
	if (rooms[currentRoom].adjRooms.size() == 0) {
		createRoom(rooms, currentRoom); // Function Definition with a Struct or Class Object as a Parameter and Function Call
	}

	// if player enters room 24, Dirth Nader will now start attacking and following
	if (currentRoom == 24) { deathGlobe = true; }

	// if player enters room 7 with all pieces of the plans and Dirth Nader has been defeated
	if (currentRoom == 7 && deathGlobe == false) {
		if (count(player1.inventory.begin(), player1.inventory.end(), "FIRST PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "SECOND PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "THIRD PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "FOURTH PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "FIFTH PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "SIXTH PIECE OF THE PLANS")) {

			// display the description of the room
			for (int i = 0; i < rooms[7].descriptions.size(); i++) { printSpecial(rooms[7].descriptions[i]); }

			// the player wins the game and the Rebel ship picks them up
			printLines(400);
			gameOver = true;
		}
	}

	// if player enters room 7 with all pieces of the plans and Dirth Nader has not been defeated
	if (currentRoom == 7 && deathGlobe == true) {
		if (count(player1.inventory.begin(), player1.inventory.end(), "FIRST PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "SECOND PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "THIRD PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "FOURTH PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "FIFTH PIECE OF THE PLANS")
			&& count(player1.inventory.begin(), player1.inventory.end(), "SIXTH PIECE OF THE PLANS")) {

			// display the description of the room
			clear();
			for (int i = 0; i < rooms[7].descriptions.size(); i++) { printSpecial(rooms[7].descriptions[i]); }

			// player must defeat Dirth Nader before they can escape
			printSpecial("\nDirth Nader found you trying to escape with the plans for the DEATH GLOBE!");
			fight(rooms, 7, nextRoom);

			// if the player defeats Dirth Nader
			if (deathGlobe == false) {
				// the player wins the game and the Rebel ship picks them up
				printLines(400);
				gameOver = true;
			}
		}
	}

	// player "enters" room... describe it and ask what they want to do
	if (!gameOver) {
		enterRoom(rooms, currentRoom, gamePath, prevRoom, nextRoom);
		prevRoom = currentRoom;
		currentRoom = nextRoom;

		// if the player loses while in one of the rooms that is not number 7
		if (gameOver) {
			// make the player confirm the end of the game
			char temp = 'x';
			while (temp != '\n') {
				cout << "\nPress enter to exit the game... ";
				cin.get(temp);
			}

			return '3';
		}

		// transition to the next room
		else{ return '4'; }
	}

	// if the game is over, return to the exit case
	else {
		// make the player confirm the end of the game
		char temp = 'x';
		while (temp != '\n') {
			cout << "\nPress enter to exit the game... ";
			cin.get(temp);
		}

		return '3';
	}
}


/**
midGameMenu Function - displays the mid-game menu to the user
@param gamePath - sets the next step in the game path to what the player wants to do next
@return this case as the previous case ('5')
*/
char midGameMenu(char& gamePath) {
	clear();

	// print mid-game menu and validate/check input
	getIntInput(199, 4);

	// handle next process for the game to run
	if (intInput[0] == '4') { gamePath = '3'; }

	// if user chooses to continue, pick up in room they were last in
	else if (intInput[0] == '1') { gamePath = '4'; }
	else if (intInput[0] == '3') { gamePath = '6'; }
	else { gamePath = '2'; }

	return '5';
}


/**
statsInventory Function - displays player inventory and stats from the mid-game menu
@param gamePath - sets the next step in the game path to what the player wants to do next
@param rooms - struct for rooms in this game
@param num - number of the room the player is in
@return this case as the previous case ('6')
*/
char statsInventory(char& gamePath, room rooms[], int num) {
	// Function Definition with 1D or 2D Array as a Parameter and Function Call (above)
	clear();

	// display player inventory and stats
	displayItems(rooms, num, 1);

	// make the player return to the mid-game menu
	char temp = 'x';
	while (temp != '\n') {
		cout << "\nPress enter to return to the menu... ";
		cin.get(temp);
	}

	clear();
	gamePath = '5';

	return '6';
}


/**
createRoom Function - searches the text file for information to "create" the room if it is the first time the player enters it
@param rooms - struct for rooms in this game
@param num - number of the room to be created
*/
void createRoom(room rooms[], int num) { // Function Definition with a Struct or Class Object as a Parameter and Function Call
	ifstream text;
	string line;

	// keywords for navigating .txt files for desired lines
	string searchBeg = to_string(num) + "Start"; // Convert Between Data Types with a Purpose
	string searchItems = "Items:";
	string searchMoves = "Next Moves:";
	string searchEnd = to_string(num) + "End";

	int begDelay = 0;
	int itemsDelay = 0;
	bool start = false;
	bool items = false;
	bool moves = false;
	bool end = false;

	// temporary variables for using cin with room vectors
	int tempInt = 0;
	string tempStr;

	text.open("StoryText.txt");
	while (getline(text, line)) {
		// if start keyword found, set flag true
		if (line.find(searchBeg, 0) != string::npos) { start = true; }
		if (start) {
			// if items keyword found, set flag true
			if (line.find(searchItems, 0) != string::npos) { items = true; }

			// if moves keyword found, set flag true
			if (line.find(searchMoves, 0) != string::npos) { moves = true; }

			// if end keyword found, set flag true
			if (line.find(searchEnd, 0) != string::npos) { end = true; }
		}
		
		// add information to rooms[num] to create that room
		if (start && !items) {
			if (begDelay == 1) { rooms[num].descriptions.push_back(line); }
			else { begDelay++; }
		}
		if (items && !moves) {
			if (itemsDelay == 1) { rooms[num].items.push_back(line); }
			else { itemsDelay++; }
		}
		if (moves && !end) {
			text >> tempStr >> tempInt;

			// make sure the program is reading in a line that it should actually use
			if (tempStr[0] != 'N' && tempStr[0] != 'E' && tempStr[0] != 'S' && tempStr[0] != 'W') { break; }

			// then assign temp variables to the actual room
			rooms[num].directions.push_back(tempStr);
			rooms[num].adjRooms.push_back(tempInt);
		}
	}
	text.close();
}


/**
enterRoom Function - uses room number and struct of rooms to print out a description of the room and allow player to pick up items
@param rooms - struct for rooms in this game
@param currentRoom - room number the player is entering
@param gamePath - sets the next step in the game path to what the player wants to do next
@param prevRoom - room the player is coming from
@param nextRoom - room number the player will be entering next (based on their input)
*/
void enterRoom(room rooms[], int currentRoom, char& gamePath, int prevRoom, int& nextRoom) {
	clear();

	// display the description of that room (previously read in from .txt file by createRoom function)
	for (int i = 0; i < rooms[currentRoom].descriptions.size(); i++) { printSpecial(rooms[currentRoom].descriptions[i]); }

	// give player options of things they can do in room and determine where to go next in program
	do {
		// reset the fromCombat bool
		fromCombat = false;

		// ask player to choose an option to do
		printSpecial("\nWhat would you like to do?");
		printSpecial("1. Do an action\n2. Move to another room\n3. Look at character stats and inventory\n4. Go to menu");
		getIntInput(1000, 4);

		// perform the option the player chooses
		if (intInput == "1") { roomActions(rooms, currentRoom, nextRoom); }
		else if (intInput == "2") {
			printSpecial("\nIn which direction do you wish to move? ");
			getStrInput();

			// check to see if there is a room desired direction
			for (int k = 0; k < rooms[currentRoom].directions.size(); k++) {
				if (strInput == rooms[currentRoom].directions[k]) {
					nextRoom = rooms[currentRoom].adjRooms[k];
				}
			}			

			// if there is no room in desired direction
			while (nextRoom == currentRoom) {
				printSpecial("It is not possible to go that direction in this room.\nPick another direction: ");
				getStrInput();
				for (int g = 0; g < rooms[currentRoom].directions.size(); g++) {
					if (strInput == rooms[currentRoom].directions[g]) {
						nextRoom = rooms[currentRoom].adjRooms[g];
					}
				}
			}
		}
		else if (intInput == "4") { gamePath = '5'; nextRoom = currentRoom; }
		else if (intInput == "3") { printSpecial("-"); displayItems(rooms, currentRoom, 1); }
	} while (intInput == "3" || (intInput == "1" && !fromCombat && !gameOver));
}


/**
roomActions Function - checks to see if the user enters a valid action to do and then executes the action
@param rooms - struct for rooms in this game
@param num - number of the room the player is in
@param nextRoom - room number the player will possibly be sent to if they escape during combat
*/
void roomActions(room rooms[], int num, int& nextRoom) {
	while (!gameOver) {
		// ask the user to enter an action (as a keyword)
		printSpecial("\nWhat action do you want to do? ");
		getStrInput();

		// if they use the PICK keyword
		if (strInput == "PICK") {
			printSpecial("What do you want to pick up? ");
			getStrInput();

			// check to see if they entered a valid keyword
			if (!count(rooms[num].items.begin(), rooms[num].items.end(), strInput)) {
				printSpecial("That is not a valid input. Try again.");
			}

			// see if the item they wish to pick up exists in the room, if not already in their inventory
			else {
				printSpecial("Item has been added to your inventory.");
				player1.inventory.push_back(strInput);

				// remove the item from the room
				for (int i = 0; i < rooms[num].items.size(); i++) {
					if (rooms[num].items[i] == strInput) { rooms[num].items[i].erase(); }
				}
			}

			// clear vector to revert size to zero if it is completely empty
			int counter = 0;
			for (int m = 0; m < rooms[num].items.size(); m++) {
				if (rooms[num].items[m] != "") { counter++; }
			}
			if (counter == 0) { rooms[num].items.clear(); }
		}
		else if (strInput == "SEARCH") {
			printSpecial("-"); printLines(200);
			if (rooms[num].items.size() > 0) {
				printSpecial("You find the following items by searching the room:");
				displayItems(rooms, num, 0);
			}
			else { printSpecial("\nYou find no items in the room."); }
		}
		else if ((strInput == "FIGHT" || strInput == "HIT" || strInput == "ATTACK") && deathGlobe) {
			fight(rooms, num, nextRoom);
			fromCombat = true;
			break;
		}
		else if (strInput == "EAT") {
			printSpecial("What do you want to eat? ");
			getStrInput();
			string temp = strInput;

			// if the player tries to eat something that is not in their inventory, tell them
			if (!count(player1.inventory.begin(), player1.inventory.end(), strInput)) {
				printSpecial("You do not have that item in your inventory.");
			}
			else {
				for (int i = 0; i < player1.inventory.size(); i++) {
					if (player1.inventory[i] == strInput) {
						for (int j = 0; j < player1.inventory[i].size(); j++) {
							temp[j] = tolower(player1.inventory[i][j]);
						}
						printSpecial("You just ate the " + temp + ".");
						if (temp == "bowl of olives" || temp == "moldy sandwich") {
							printSpecial("You gain 5 health!");
							player1.health += 5;
						}
						else if (temp == "emergency rations") {
							printSpecial("You gain 15 health!");
							player1.health += 15;
						}
						else if (temp == "high-tech pen") {
							printSpecial("The pen blows up in your mouth!\nYou lose 10 health.");
							player1.health -= 10;
							if (player1.health < 1) {
								printSpecial("\nUh oh, your health is below 1! You died.");
								printSpecial("Game over! Dirth Nader and the EMPIRE won.");
								gameOver = true;
								break;
							}
						}
						player1.inventory[i].erase();
					}
				}
			}
		}
		else if (strInput == "DRINK") {
			printSpecial("What do you want to drink? ");
			getStrInput();
			string temp = strInput;

			// if the player tries to drink something that is not in their inventory, tell them
			if (!count(player1.inventory.begin(), player1.inventory.end(), strInput)) {
				printSpecial("You do not have that item in your inventory.");
			}
			else if (strInput == "BANTHA MILK") {
				for (int i = 0; i < player1.inventory.size(); i++) {
					if (player1.inventory[i] == strInput) {
						for (int j = 0; j < player1.inventory[i].size(); j++) {
							temp[j] = tolower(player1.inventory[i][j]);
						}
						printSpecial("You just drank the " + temp + ".");
						printSpecial("You gain 10 health!");
						player1.health += 10;
						player1.inventory[i].erase();
					}
				}
			}
			else if (strInput == "CANISTER OF JET FUEL") {
				for (int i = 0; i < player1.inventory.size(); i++) {
					if (player1.inventory[i] == strInput) {
						for (int j = 0; j < player1.inventory[i].size(); j++) {
							temp[j] = tolower(player1.inventory[i][j]);
						}
						printSpecial("You just drank the " + temp + ".");
						printSpecial("You gain 40 health!");
						player1.health += 40;
						player1.inventory[i].erase();
					}
				}
			}
			else { printSpecial("You cannot drink that item."); }
		}
		else if (strInput == "WEAR") {
			printSpecial("What do you want to wear? ");
			getStrInput();
			string temp = strInput;

			// if the player tries to wear something that is not in their inventory, tell them
			if (!count(player1.inventory.begin(), player1.inventory.end(), strInput)) {
				printSpecial("You do not have that item in your inventory.");
			}
			else if (strInput == "BAR STOOL" || strInput == "HAZMAT SUIT" || strInput == "SURGICAL MASK"
				|| strInput == "TROOPER HELMET" || strInput == "PILOT GOGGLES" || strInput == "PARACHUTE BACKPACK"
				|| strInput == "FANCY CUFF LINKS") {
				for (int i = 0; i < player1.inventory.size(); i++) {
					if (player1.inventory[i] == strInput) {
						for (int j = 0; j < player1.inventory[i].size(); j++) {
							temp[j] = tolower(player1.inventory[i][j]);
						}
						printSpecial("You put on the " + temp + ".");
						printSpecial("You gain 5 health!");
						player1.health += 5;
						player1.wearing.push_back(player1.inventory[i]);
						player1.inventory[i].erase();
					}
				}
			}
			else { printSpecial("You cannot wear that item."); }
		}
		else if ((strInput == "UNLOCK" || strInput == "OPEN") && num == 13 &&
			!count(player1.inventory.begin(), player1.inventory.end(), "FIFTH PIECE OF THE PLANS")) {

			// only allow the player to unlock the cells if they are in the room and do not already have the map piece
			// this means that they have not unlocked the cells yet
			printSpecial("What item do you want to use to try to open up the cells? ");
			getStrInput();
			if (count(player1.inventory.begin(), player1.inventory.end(), strInput)) {
				if (strInput == "RUBBER DUCK") {
					printSpecial("You use the rubber duck to unlock the holding cells!");
					printSpecial("One of the Rebel prisoners runs up to you and hands you the fifth piece of the plans!");
					player1.inventory.push_back("FIFTH PIECE OF THE PLANS");
				}
				else {
					string temp;
					for (int b = 0; b < strInput.length(); b++) {
						temp[b] = tolower(strInput[b]);
					}
					printSpecial("You try unlocking the holding cells with the " + temp + ".");
					printSpecial("Nothing seems to have happened...");
				}
			}
			else {
				printSpecial("You do not have that item in your inventory.");
			}
		}
		else if (strInput == "BACK" || strInput == "EXIT" || strInput == "ESCAPE" || strInput == "NONE") { break; }
		else { printSpecial("That is not a valid action. Try again."); }
	}
}


/**
displayItems Function - displays either items present in the current room or player inventory/stats
@param rooms - struct for rooms in this game
@param num - number of the room the player is in
@param a - integer telling the function whether to print out room items (0) or player inventory/stats (1)
*/
void displayItems(room rooms[], int num, int a) {
	string tempItems; // Local Variable
	string tempWearing;
	string tempInventory;

	// print out the items in the room
	if (a == 0) {
		for (int i = 0; i < rooms[num].items.size(); i++) { // Using ++ or --
			tempItems = rooms[num].items[i];
			for (int j = 1; j < rooms[num].items[i].length(); j++) { // Nested Loops
				tempItems[j] = tolower(rooms[num].items[i][j]); // Using a String like an Array
			}
			if (tempItems != "") { printSpecial(tempItems); }
		}
	}
	// print out the items in the player's inventory
	else if (a == 1) {
		printSpecial("Name: " + player1.character);
		printSpecial("Health: " + to_string(player1.health));

		// only print items the player is wearing if they are wearing any
		if (player1.wearing.size() > 0) {
			printSpecial("\nWearing:");
			for (int f = 0; f < player1.wearing.size(); f++) {
				tempWearing = player1.wearing[f];
				for (int g = 1; g < player1.wearing[f].length(); g++) {
					tempWearing[g] = tolower(player1.wearing[f][g]);
				}
				printSpecial(tempWearing);
			}
		}

		// print out the items in the inventory, even if there are none
		printSpecial("\nInventory:");
		for (int m = 0; m < player1.inventory.size(); m++) {
			tempInventory = player1.inventory[m];
			for (int n = 1; n < player1.inventory[m].length(); n++) {
				tempInventory[n] = tolower(player1.inventory[m][n]);
			}
			printSpecial(tempInventory);
		}
		if (player1.inventory.size() == 0) { printSpecial("No items"); }
	}
}


/**
printSpecial Function - prints out the passed string character by character for effect
@param temp - string to be printed out
*/
void printSpecial(string temp) {
	// special case for when I actually want to print a blank line
	if (temp != "-") {
		// loop through the string one character at a time
		for (int i = 0; i < temp.length(); i++) { // Code that “Walks” through the Contents of an Array or Vector
			cout << temp[i];

			// wait for a bit for effect
			Sleep(DELAY);
		}
	}

	// if the last char of the passed string is a space, this means the user will enter something on the same line
	if (temp != "" && temp[temp.length() - 1] != ' ' && temp != "-") { printSpecial("-"); }
	else if (temp == "-") { cout << endl; }
}


/**
fight Function - allows the player to fight against Dirth Nader
@param rooms - struct for rooms in this game
@param num - number of the room the player is in
@param nextRoom - room number the player will be randomly sent to if they escape Dirth Nader
*/
void fight(room rooms[], int num, int& nextRoom) {
	// include time to randomly decide who attacks first
	srand(time(0));
	bool combat = true;
	bool defending = true;
	bool block = false;
	string temp;

	while (combat) { // While Loop
		// use RNG to decide who gets to attack
		int turn = rand() % 2;

		// clear vectors to revert size to zero if they are completely empty
		int counter = 0;
		for (int m = 0; m < player1.inventory.size(); m++) {
			if (player1.inventory[m] != "") { counter++; }
		}
		if (counter == 0) { player1.inventory.clear(); }

		counter = 0;
		for (int n = 0; n < player1.bodyParts.size(); n++) {
			if (player1.bodyParts[n] != "") { counter++; }
		}
		if (counter == 0) { player1.bodyParts.clear(); }

		// if there is nothing in player's inventory and they have no body parts left, they cannot attack
		if (player1.inventory.size() == 0 && player1.bodyParts.size() == 0) {
			printSpecial("\nYou have nothing to attack with!");
			
			// only Dirth Nader can attack
			turn = 0;
		}

		if (turn == 0) {
			do { // Do While Loop
				printSpecial("\nDirth Nader is attacking you! What do you want to do? ");
				getStrInput();

				// make sure player only enters valid action input
				while (strInput != "BLOCK" && strInput != "DODGE" && strInput != "ESCAPE") {
					printSpecial("\nThat is not a valid action. Try again: ");
					getStrInput();
				}

				if (strInput == "BLOCK" && (player1.bodyParts.size() > 0 || player1.inventory.size() > 0)) {
					block = false;
					
					// only let player enter valid item to block themselves with (see if it is in their inventory)
					do {
						printSpecial("What do you want to block with? ");
						getStrInput();

						// check to see if the player has the body parts left to block, if that is what they entered
						if (strInput == "ARM" || strInput == "HAND" || strInput == "LEG" || strInput == "FOOT") {
							if (!count(player1.bodyParts.begin(), player1.bodyParts.end(), strInput)) {
								// lower the string for outputting to console
								for (int q = 0; q < strInput.length(); q++) { temp[q] = tolower(strInput[q]); }

								if (strInput == "FOOT") { printSpecial("You have no more feet left to block with!\n"); }
								else if (strInput == "ARM") { printSpecial("You have no more arms left to block with!\n"); }
								else if (strInput == "HAND") { printSpecial("You have no more hands left to block with!\n"); }
								else if (strInput == "LEG") { printSpecial("You have no more legs left to block with!\n"); }
							}
							else { break; }
						}

						// check to see if the player has that item in their inventory
						else if (!count(player1.inventory.begin(), player1.inventory.end(), strInput)) {
							printSpecial("You do not have that item in your inventory.\n");
						}
						else { break; }
					} while (true);

					for (int i = 0; i < player1.inventory.size(); i++) {
						// find that item in the player's inventory
						if (player1.inventory[i] == strInput) {
							temp = player1.inventory[i];

							// lower the item casing for outputting
							for (int j = 0; j < player1.inventory[i].length(); j++) {
								temp[j] = tolower(player1.inventory[i][j]);
							}
							printSpecial("You block yourself with the " + temp + ".\n");
							printSpecial("The " + temp + " disentegrates after being sliced by Dirth Nader's lightsword!\n");
							player1.inventory[i].erase();
							defending = false;
							break;
						}
					}

					for (int k = 0; k < player1.bodyParts.size(); k++) {
						// find the body part the player wants to attack with
						if (player1.bodyParts[k] == strInput) {
							temp = player1.bodyParts[k];

							// lower word for outputting
							for (int m = 0; m < player1.bodyParts[k].length(); m++) {
								temp[m] = tolower(player1.bodyParts[k][m]);
							}
							printSpecial("You block yourself with your " + temp + ".\n");
							printSpecial("Your " + temp + " falls off after being sliced by Dirth Nader's lightsword!\n");

							// if player loses an arm or leg, they lose a foot or hand with it (assumes they are on same limb)
							if (player1.bodyParts[k] == "ARM") {
								for (int n = 0; n < player1.bodyParts.size(); n++) { // For Loop
									if (player1.bodyParts[n] == "HAND") { // Nested If Statement
										player1.bodyParts[n].erase();
										break;
									}
								}
							}
							else if (player1.bodyParts[k] == "LEG") {
								for (int p = 0; p < player1.bodyParts.size(); p++) {
									if (player1.bodyParts[p] == "FOOT") {
										player1.bodyParts[p].erase();
										break;
									}
								}
							}

							player1.bodyParts[k].erase();
							defending = false;
							break;
						}
					}
				}
				else if (strInput == "BLOCK" && player1.bodyParts.size() == 0 && player1.inventory.size() == 0) {
					printSpecial("You have nothing to block with. Pick another action.");
					defending = true;
				}
				else if (strInput == "DODGE") {
					printSpecial("Which way do you want to dodge? ");
					getStrInput();
					while (strInput != "LEFT" && strInput != "RIGHT" && strInput != "DUCK") {
						printSpecial("Sorry, you can't dodge that way! Try again: ");
						getStrInput();
					}

					// generate random attack from Dirth Nader
					int attackDirection = rand() % 3;
					if (attackDirection == 0 && strInput != "LEFT") {
						printSpecial("You tried to dodge, but Dirth Nader still hit you with his lightsword! You lost 5 health.");
						player1.health -= 5;
						defending = false;
					}
					else if (attackDirection == 1 && strInput != "DUCK") {
						printSpecial("You tried to dodge, but Dirth Nader still hit you with his lightsword! You lost 5 health.");
						player1.health -= 5;
						defending = false;
					}
					else if (attackDirection == 3 && strInput != "RIGHT") {
						printSpecial("You tried to dodge, but Dirth Nader still hit you with his lightsword! You lost 5 health.");
						player1.health -= 5;
						defending = false;
					}
					else { printSpecial("Nice! Dirth Nader tried to hit you, but he missed."); defending = false; }
				}
				else if (strInput == "ESCAPE") {
					// use RNG to see if escape is successful
					bool escape = rand() % 2;

					if (escape) {
						printSpecial("You successfully escaped!");
						char leave = 'x';
						while (leave != '\n') {
							cout << "\nPress enter to continue... ";
							cin.get(leave);
						}
						int jumpTo = rand() % rooms[num].adjRooms.size();
						nextRoom = rooms[num].adjRooms[jumpTo];
						combat = false;
						defending = false;
					}
					else {
						printSpecial("Bummer! Dirth Nader saw that you were trying to escape and got in your way!");
						printSpecial("He attacks you and you lose 5 health.");
						player1.health -= 5;
						defending = false;
					}
				}
			} while (defending);
		}
		else {
			do {
				printSpecial("It is your turn to attack.");

				// only let player enter valid item to attack with (see if it is in their inventory)
				do {
					printSpecial("What do you want to attack with? ");
					getStrInput();

					// check to see if the player has the body parts left to block, if that is what they entered
					if (strInput == "ARM" || strInput == "HAND" || strInput == "LEG" || strInput == "FOOT") {
						if (!count(player1.bodyParts.begin(), player1.bodyParts.end(), strInput)) {
							// lower the string for outputting to console
							for (int q = 0; q < strInput.length(); q++) { temp[q] = tolower(strInput[q]); }

							if (strInput == "FOOT") { printSpecial("You have no more feet left to attack with!\n"); }
							else if (strInput == "ARM") { printSpecial("You have no more arms left to attack with!\n"); }
							else if (strInput == "HAND") { printSpecial("You have no more hands left to attack with!\n"); }
							else if (strInput == "LEG") { printSpecial("You have no more legs left to attack with!\n"); }
						}
						else { break; }
					}

					// check to see if the player has that item in their inventory
					else if (!count(player1.inventory.begin(), player1.inventory.end(), strInput)) {
						printSpecial("You do not have that item in your inventory.\n");
					}
					else { break; }
				} while (true);

				temp = strInput;

				// use RNG to see if attack hits or misses
				bool hitMiss = rand() % 2;

				// lower the input for outputting to console
				for (int b = 0; b < strInput.length(); b++) {
					temp[b] = tolower(strInput[b]);
				}

				// see if the input is something in player's inventory, a body part or neither
				if (count(player1.inventory.begin(), player1.inventory.end(), strInput)) {
					printSpecial("You attack Dirth Nader with the " + temp + ".");
					if (hitMiss) {
						if (strInput == "OLIVES" || strInput == "BOWL OF OLIVES") {
							printSpecial("He is incredibly allergic to olives!");
							dirthNader.health = 0;
							rooms[24].descriptions.clear();
							rooms[24].descriptions.push_back("This room is eerily quiet now that Dirth Nader is not here...");
							deathGlobe = false;
							combat = false;
							break;
						}
						else {
							printSpecial("Your attack was successful! Dirth Nader lost 5 health.");
							dirthNader.health -= 5;
						}
					}
					else { printSpecial("Your attack was not successful. Dirth Nader did not lose health."); }
					break;
				}
				else if (count(player1.bodyParts.begin(), player1.bodyParts.end(), strInput)) {
					printSpecial("You attack Dirth Nader with your " + temp + ".");
					if (hitMiss) {
						printSpecial("Your attack was successful! Dirth Nader lost 3 health.");
						dirthNader.health -= 3;
					}
					else { printSpecial("Your attack was not successful. Dirth Nader did not lose health."); }
					break;
				}
			} while (true);
		}

		// if the player's health goes below 1, they are dead and the game is over
		if (player1.health < 1) {
			printSpecial("\nUh oh, your health is below 1! You died.");
			printSpecial("Game over! Dirth Nader and the EMPIRE won.");
			combat = false;
			gameOver = true;
		}

		// if the player defeats Dirth Nader
		if (dirthNader.health < 1) {
			printSpecial("\nYou defeated Dirth Nader!\nDoctors rush in to take him to the operating room.");
			rooms[24].descriptions.clear();
			rooms[24].descriptions.push_back("This room is eerily quiet now that Dirth Nader is not here...");
			deathGlobe = false;
			combat = false;
		}
	}
}