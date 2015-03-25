#include "stdafx.h"
#include <iostream>
#include <ctime>		/* time */
#include <string>		/* string */
#include <sstream>		/* stringstream */
#include <vector>		/* vector */
#include <algorithm>	/* sort */

using namespace std;

/* Global Constants */
const int MAX_DIE = 5;
const int MAX_TURNS = 13;
const int MAX_ROUNDS = 8;
const int SCORE_CATEGORIES = 14; /* upper scores [0]-[5], lower scores [6]-[12], total score [13] */

/* welcome function. Welcomes player(s) to the game */
void welcome(void){
	cout << "=============================================" << endl;
	cout << "==-----------------------------------------==" << endl;
	cout << "=|\t         Yahtzee                   |=" << endl;
	cout << "==-----------------------------------------==" << endl;
	cout << "=============================================" << endl;
	system("pause");
	system("cls");
}

/* getPlayerCount function. Gets the number of players for the game */
int getPlayerCount(void){
	int input;
	bool valid = false;

	/* Loop until a valid input is received */
	do{
		cout << "Enter the number of players: ";
		cin >> input;

		/* Test input */
		if (cin.good() && input > 0){
			valid = true;
		}
		else {
			/* Invalid input. Reset and empty the buffer */
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input! Try again..." << endl << endl;
		}
	} while (!valid);
	system("cls");
	return input;
}

/* getName function. Gets a players name from input and trims to 7 characters */
string getName(int i){
	string input;
	bool validName = false;
	
		/* Loop until a valid name is received */
		do{
			cout << "Enter Player " << i + 1 << "'s name: ";
			cin >> input;

			/* Trim long names to 7 characters, for appearances */
			if (input.length()> 7)
				input.erase(input.begin() + 7, input.end());

			/* Test input */
			if (cin.good() && input != ""){

				/* name was accepted. Exit loop */
				validName = true;
			}
			else {

				/* Invalid name. Reset and empty the buffer */
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid name! Try again..." << endl << endl;
			}
		} while (!validName);        /* loop if name not valid */

	system("cls");
	return input;
}

/* rollDie function. Generates values for 5 die,
allows user to reroll any or all die, up to 2 times. */
void rollDie(int die[])
{
	const int MAX_TURNS = 3;

	/* Generate initial die values */
	cout << "First roll: ";
	for (int i = 0; i < MAX_DIE; ++i)
	{
		die[i] = rand() % 6 + 1;
		cout << die[i] << " ";
	}

	/* Determine if any die should be rerolled */
	for (int turn = 1; turn < MAX_TURNS; ++turn)
	{
		cout << "\nDo you want to reroll any dice? (y/n)" << endl;
		char reroll; cin >> reroll;

		/* Determine which die to reroll */
		if (toupper(reroll) == 'Y')
		{
			for (int i = 0; i < MAX_DIE; ++i)
			{
				cout << "Reroll Die " << i + 1 << " [" << die[i] << "]? (y/n)";
				char rerollDie; cin >> rerollDie;

				/* Reroll die */
				if (toupper(rerollDie) == 'Y') die[i] = rand() % 6 + 1;
			}

			/* Display new/current roll, if a reroll was selected */
			cout << "\nNew roll: ";
			for (int i = 0; i < MAX_DIE; ++i)
				cout << die[i] << " ";
		}

		/* If a reroll is not requested, end turn */
		else turn = MAX_TURNS;
	}
}

/* placeScore function. Select scoring catagory then determine score value based off dice roll */
void placeScore(int die[], vector<vector<int>> &scoreCard, int player){

	/* Check if yahtzee bonus applies */
	bool yahtzee = false;
	if (scoreCard[player][11] >= 50){
		int temp[6];
		for (int i = 0; i < 6; ++i){
			temp[i] = count(die, die + MAX_DIE, i + 1);
			if (temp[i] == 5){
				yahtzee = true;
			}
		}
	}

	/* Category Strings */
	string category[13] = { "Ones", "Twos", "Threes", "Fours", "Fives", "Sixes",
		"3 of a Kind", "4 of a Kind", "Full House", "Small Straight", "Large Straight", "Yahtzee", "Chance" };

	/* Scoring category selection. Repeats if selected category is used or if invalid selection is entered */
	bool repeat;
	do{
		
		repeat = false;
	
		/* Display Roll to score */
		cout << "\nRoll to Score: ";
		for (int i = 0; i < MAX_DIE; ++i)
			cout << die[i] << " ";

		/* Remove used categories from list */
		for (int i = 0; i < 13; ++i){
			if (scoreCard[player][i] != 0)
				category[i] = "----";
		}


		/* Display score options */
		cout << "\nSelect a Score Catagory:" << endl;
		cout << "1. " << category[0] << " \t 7. " << category[6] << endl;
		cout << "2. " << category[1] << " \t 8. " << category[7] << endl;
		cout << "3. " << category[2] << " \t 9. " << category[8] << endl;
		cout << "4. " << category[3] << " \t 10. " << category[9] << endl;
		cout << "5. " << category[4] << " \t 11. " << category[10] << endl;
		cout << "6. " << category[5] << " \t 12. " << category[11] << endl;
		cout << "\t\t 13. " << category[12] << endl;
		cout << "Enter Selection:";
		int option;
		cin >> option;
		cin.ignore();
		cout << option;

		/* scoring option, switch cases */
		switch (option){

			case 1:		/* Ones */

				/* Check if category is empty */
				if (scoreCard[player][0] == 0){
					int tempScore = 0;

					/* calculate score to be entered */
					for (int i = 0; i < MAX_DIE; ++i){
						if (die[i] == 1)
							tempScore += die[i];
					}

					/* Assign score to players scorecard*/
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][0] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][0] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}
		
			case 2:		/* Twos */

				/* Check if category is empty */
				if (scoreCard[player][1] == 0){
					int tempScore = 0;

					/* calculate score to be entered */
					for (int i = 0; i < MAX_DIE; ++i){
						if (die[i] == 2)
							tempScore += die[i];
					}

					/* Assign score to players scorecard*/
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][1] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][1] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 3:		/* Threes */

				/* Check if category is empty */
				if (scoreCard[player][2] == 0){
					int tempScore = 0;

					/* calculate score to be entered */
					for (int i = 0; i < MAX_DIE; ++i){
						if (die[i] == 3)
							tempScore += die[i];
					}

					/* Assign score to players scorecard*/
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][2] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][2] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 4:		/* Fours */

				/* Check if category is empty */
				if (scoreCard[player][3] == 0){
					int tempScore = 0;

					/* calculate score to be entered */
					for (int i = 0; i < MAX_DIE; ++i){
						if (die[i] == 4)
							tempScore += die[i];
					}

					/* Assign score to players scorecard*/
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][3] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][3] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 5:		/* Fives */

				/* Check if category is empty */
				if (scoreCard[player][4] == 0){
					int tempScore = 0;

					/* calculate score to be entered */
					for (int i = 0; i < MAX_DIE; ++i){
						if (die[i] == 5)
							tempScore += die[i];
					}

					/* Assign score to players scorecard*/
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][4] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][4] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 6:		/* Sixes */

				/* Check if category is empty */
				if (scoreCard[player][5] == 0){
					int tempScore = 0;

					/* calculate score to be entered */
					for (int i = 0; i < MAX_DIE; ++i){
						if (die[i] == 6)
							tempScore += die[i];
					}

					/* Assign score to players scorecard*/
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][5] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][5] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 7:		/* 3 of a kind */

				/* Check if category is empty */
				if (scoreCard[player][6] == 0){
					int temp[6];
					int tempScore = 0;

					/* Determine if 3 of a kind was rolled */
					for (int i = 0; i < 6; ++i){
						temp[i] = count(die, die + MAX_DIE, i + 1);
						if (temp[i] == 3){

							/* add up dice values */
							for (int j = 0; j < MAX_DIE; ++j)
								tempScore += die[j];
						}
					}

					/* Assign score to players scorecard */
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][6] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][6] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 8:		/* 4 of a kind */

				/* Check if category is empty */
				if (scoreCard[player][7] == 0){
					int temp[6];
					int tempScore = 0;

					/* Determine if 4 of a kind was rolled */
					for (int i = 0; i < 6; ++i){
						temp[i] = count(die, die + MAX_DIE, i + 1);
						if (temp[i] == 4){

							/* add up dice values */
							for (int j = 0; j < MAX_DIE; ++j)
								tempScore += die[j];
						}
					}
					/* Assign score to players scorecard */
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][7] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][7] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 9:		/* Full House */

				/* Check if category is empty */
				if (scoreCard[player][8] == 0){
					int temp[6];
					int fullHouse = 0;

					/* Determine if a full house was rolled */
					for (int i = 0; i < 6; ++i){
						temp[i] = count(die, die + MAX_DIE, i + 1);
						if (temp[i] == 3)
							++fullHouse;
						else if (temp[i] == 2)
							++fullHouse;
					}
					/* If fullhouse was rolled, assign score */
					if (fullHouse == 2){
						scoreCard[player][8] = 25;
						break;
					}
					else {
						scoreCard[player][8] = -1;
						break;
					}

				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][8] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 10:	/* Small Straight */

				/* Check if category is empty */
				if (scoreCard[player][9] == 0){
					int count = 0;

					/* sort die array */
					sort(die, die + MAX_DIE);

					/* check if there is a straight */
					for (int i = 1; i < MAX_DIE; ++i){
						if (die[i] == die[i - 1] + 1)
							++count;
					}

					/* assign score to players scorecard, if a straight was rolled */
					if (count > 3){
						scoreCard[player][9] = 30;
						break;
					}
					else {
						scoreCard[player][9] = -1;
						break;
					}
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][9] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 11:	/* Large Straight */

				/* Check if category is empty */
				if (scoreCard[player][10] == 0){
					int count = 0;

					/* sort die array */
					sort(die, die + MAX_DIE);

					/* check if there is a straight */
					for (int i = 1; i < MAX_DIE; ++i){
						if (die[i] == die[i - 1] + 1)
							++count;
					}

					/* assign score to players scorecard, if a straight was rolled */
					if (count > 4){
						scoreCard[player][10] = 30;
						break;
					}
					else {
						scoreCard[player][10] = -1;
						break;
					}
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][10] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 12:	/* Yahtzee */

				/* Check if category is empty */
				if (scoreCard[player][11] == 0){
					int temp[6];
					int tempScore = 0;
	
					/* Determine if a yahtzee was rolled */
					for (int i = 0; i < 6; ++i){
						temp[i] = count(die, die + MAX_DIE, i + 1);
						if (temp[i] == 5)
							tempScore = 50;	
					}

					/* Assign score to players scorecard */
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][11] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][11] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			case 13:	/* Chance */

				/* Check if category is empty */
				if (scoreCard[player][12] == 0){
					int tempScore = 0;

					/* add up dice values */
					for (int i = 0; i < MAX_DIE; ++i)
						tempScore += die[i];

					/*assign score to players scorecard */
					if (tempScore == 0){
						tempScore = -1;
					}
					scoreCard[player][12] = tempScore;
					break;
				}

				/* Category was previously used, exit and reselect category */
				else if (scoreCard[player][12] != 0){
					cout << "Category already used, pick another." << endl;
					repeat = true;
					break;
				}

			default:
				cout << "Invalid Choice...Try again." << endl;
				repeat = true;
		}

	} while (repeat);

exit_switch:;

	/* Apply yahtzee bonus */
	if (yahtzee) {
		scoreCard[player][11] += 100;
		cout << "Received Yahtzee Bonus!" << endl;
	}
	system("cls");
}

/* calcScore function. Calculates the players score and determines if any bonuses apply */
void calcScore(vector<vector<int>> &scoreCard, int player){
	
	int upperScore = 0; /* Categories 1 thru 6 (index 0-5) */
	int lowerScore = 0; /* Categories 7 thru 13 (index 6-12) */

	/* Reset any negative values back to 0 */
	for (int i = 0; i < SCORE_CATEGORIES; ++i){
		if (scoreCard[player][i] == -1)
			scoreCard[player][i] == 0;
	}

	/* Calc upper score */
	for (int i = 0; i < 5; ++i){
		upperScore += scoreCard[player][i];
	}
	
	/* Check if bonus score applies */
	if (upperScore > 62){
		upperScore += 35;
		cout << "Recieved Bonus Score!" << endl;
	}
		
	/* Calc lower score */
	for (int i = 6; i < 12; ++i){
		lowerScore += scoreCard[player][i];
	}

	/* Assign players score */
	scoreCard[player][13] = upperScore + lowerScore;
}

/* sortScore function.  Used to sort the second column of a vector, in descending order */
bool sortScore(const vector<string>& i, const vector<string>& j) {
	return i[1] > j[1];
}

/* scoreRanking function. Combines the playerName and scoreCard vector into one, sorts by the score in descending order,
then prints the vector, displaying all players score and their placement */
void scoreRanking(vector <vector<int>> &scoreCard, vector<string> &playerName, int MAX_PLAYERS){
	
	/* Create vector playerRank, to combine name and score vectors */
	vector<vector<string>> playerRank;
	for (int i = 0; i < MAX_PLAYERS; ++i){

		/* Create temp vectors to hold row data */
		vector<string> temp;
		for (int j = 0; j < 2; ++j){

			/* Load names into first column */
			if (j == 0)
				temp.push_back(playerName[i]);

			/* Load scores into second column */
			if (j == 1)
				temp.push_back(static_cast<ostringstream*>(&(ostringstream() << scoreCard[i][13]))->str());
		}
		/* Push temp vector to playerRank vector */
		playerRank.push_back(temp);
	}	

	/* Sort playerRank vector by score, in descending order */
	sort(playerRank.begin(), playerRank.end(), sortScore);

	cout << playerRank[0][0] << " wins!" << endl << endl;

	/* Display the playerRank vector */
	cout << "Player Rankings: " << endl;
	for (unsigned int i = 0; i<playerRank.size(); ++i) {
		cout << i + 1 << ") ";
		for (unsigned int j = 0; j<playerRank[i].size(); ++j) {
			cout << playerRank[i][j] << "   ";
		} cout << endl;
	} cout << endl;
}

/* Main Function */
int _tmain(int argc, _TCHAR* argv[]){

	/* Welcomes player to the game */
	welcome();

	/* Get the number of players */
	const int MAX_PLAYERS = getPlayerCount();

	/* Input names for each player */
	vector<string> playerName(MAX_PLAYERS);
	for (int i = 0; i < MAX_PLAYERS; ++i){
		playerName[i] = getName(i);
	}

	/* Initialize the roundScores vector and fill elements with 0 */
	vector<vector<int>> roundScore(MAX_ROUNDS, vector<int>(MAX_PLAYERS,0));

	int round = 0;
	bool repeat = false;
	do {

		/* Initialize the scoreCard vector and fills elements with 0 */
		vector<vector<int>> scoreCard(MAX_PLAYERS, vector<int>(SCORE_CATEGORIES, 0));
		
		/* Seed rand() with time */
		srand(time(NULL));

		/* Initialize die array */
		int die[MAX_DIE];

		/* Play until all turns are completed */
		for (int turn = 0; turn < MAX_TURNS; ++turn){
			
			/* Have each player roll and place a score each turn */
			for (int player = 0; player < MAX_PLAYERS; ++player){

				cout << "Turn " << turn + 1 << endl;
				cout << " | "<< playerName[player] << "'s Turn.\n" << endl;

				/* Initialize die array and get values */
				rollDie(die);

				/* Score player's roll */
				placeScore(die, scoreCard, player);

			}
		} cout << "Round Over!" << endl;
		
		/* Calculate Scores */
		for (int i = 0; i < MAX_PLAYERS; ++i)
			calcScore(scoreCard, i);

		/* Rank scores and display winner */
		scoreRanking(scoreCard, playerName, MAX_PLAYERS);

		
		/* Collect final scores for round per player */
		for (int i = 0; i < MAX_PLAYERS; ++i){
			roundScore[round][i] = scoreCard[i][13];
		}

		/* Ask to display round scores */
		cout << "Display All Round Scores? (y/n)";
		char option;
		cin >> option;
		if (toupper(option) == 'Y'){

			/* Print played round numbers */
			for (int i = 0; i < round + 1; ++i){
				cout << "\tRound " << round + 1;
			} cout << endl;

			/* print player names and their final score for each round played */
			for (int i = 0; i < MAX_PLAYERS; ++i){
				cout << playerName[i] << "\t";
				for (int j = 0; j < round+1; ++j)
					cout << roundScore[j][i] << "\t";
				cout << endl;
			} cout << endl;
		}

		/* Ask to play another round, if max rounds not played */
		if (round < MAX_ROUNDS){
			cout << "Play another round? (y/n)";
			char replay;
			cin >> replay;
			if (toupper(replay) == 'Y'){
				repeat = true;
				++round;
			}	
		}
		else{	 /* Max rounds played, end game */
			repeat = false;
		}

		system("cls");
	} while (repeat);

	cout << "Game Over!" << endl;

	/* Pause and Return */
	system("pause");
	return 0;
}

