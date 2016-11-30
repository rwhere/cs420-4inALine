// cs420-4inALine.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <vector>
#define GRID_SIZE 8

using namespace std;

struct moveStats {
	int xAmount;
	int oAmount;
	int empties;

	moveStats()
	{
		xAmount = 0;
		oAmount = 0;
		empties = 0;
	}

	void increment(char charAmountToIncrement){
		if (charAmountToIncrement == 'X')xAmount++;
		else if (charAmountToIncrement == 'O')oAmount++;
		else if (charAmountToIncrement == '_')empties++;
	}

	int getCharAmount(char charAmountToGet) {
		if (charAmountToGet == 'X')return xAmount;
		else if (charAmountToGet == 'O')return oAmount;
		else if (charAmountToGet == '_')return empties;
		return -1; //something went wrong
	}
};

void initializeBoard();
void printBoard();
bool makeMoveHuman(string move);
int checkForWinner();
void getMove();
moveStats getMoveStats(string move);
int convertStringToMoveValueX(string move);
int convertStringToMoveValueY(string move);
string convertXYValuesToMoveString(int x, int y);
int min(int depth);
int max(int depth);
void checkGameOver();
void makeMove();

char board[GRID_SIZE][GRID_SIZE], maxdepth = 64;

int main()
{
	char c, d;
	initializeBoard();
	//create some phony data to make branching factor smaller
	for(int i = 0; i < GRID_SIZE-1; ++i)
	{
		c = i%2==0 ? 'X' : 'O';
		d = i%2==0 ? 'O' : 'X';
		for(int j = 0; j < GRID_SIZE; ++j)
			board[i][j] = j%2==0 ? c : d;
	}
	printBoard();
	while(1)
	{
		getMove();
		checkGameOver();
		makeMove();
		checkGameOver();
	}
	

	return 0;
}

void initializeBoard()
{
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
			board[i][j] = '_';
}

void printBoard()
{
	cout<<" ";
	for(int i = 1; i <= GRID_SIZE ; ++i)
		cout<<setw(2)<<i;
	cout<<"\n";
	for(int i = 0; i < GRID_SIZE; ++i)
	{
		cout<<(char)(i+65);
		for(int j = 0; j < GRID_SIZE; ++j)
			cout<<setw(2)<<board[i][j];
		cout<<"\n";
	}
}

bool makeMoveHuman(string move)
{
	if(toupper(move[0])-65 < 0 || toupper(move[0])-65 > 7 || move[1] - '0' - 1 < 0 || move[1] - '0' - 1 > 7 || move.size() > 2)
		return false;
	if(board[toupper(move[0]) - 65][move[1]- '0' - 1] != '_')
		return false;
	else
		board[toupper(move[0]) - 65][move[1]- '0' - 1] = 'O';
	return true;
}

void getMove()
{
	string move;
	cout<<"Choose your next move: ";
	cin>>move;
	while(!makeMoveHuman(move))
	{
		cout<<"Invalid move.\nChoose your next move: ";
		cin>>move;
	}
}

//currently, this gets the total amount of repeated o,x, or empties that are adjacent to the given move.
//It will likely have to be modified once a proper evaluation function is discovered.
//I'm not sure if it's important that we keep information on the direction of each repeat. If so, that can probably
//be easily accomplished via an array in the struct.
moveStats getMoveStats(string move) {
	int i = 1;
	int x = convertStringToMoveValueX(move);
	int y = convertStringToMoveValueY(move);
	char repeatChar;
	moveStats stats;

	if (x-1 > 0) {
		repeatChar = board[x-i][y];

		//Checking upward for repeats
		while (board[x-i][y] == repeatChar && x - i >= 0) {
			stats.increment(repeatChar);
			i++;
		}
	}
	if (y+1 < GRID_SIZE) {
		i = 1;
		repeatChar = board[x][y+1];

		//Checking right for repeats
		while (board[x][y+i] == repeatChar && y + i < GRID_SIZE) {
			stats.increment(repeatChar);
			i++;
		}
	}
	if (x + 1 < GRID_SIZE) {
		i = 1;
		repeatChar = board[x+1][y];

		//Checking down for repeats
		while (board[x+i][y] == repeatChar && x + i < GRID_SIZE) {
			stats.increment(repeatChar);
			i++;
		}
	}
	if (y - 1 > 0) {
		i = 1;
		repeatChar = board[x][y-1];

		//Checking left for repeats
		while (board[x][y-i] == repeatChar && y-i > 0) {
			stats.increment(repeatChar);
			i++;
		}
	}

	cout << "There are " << stats.xAmount << " " << " adjacent repeat Xs, " << stats.oAmount << 
		" adjacent repeat Os, and " << stats.empties << " adjacent repeat empties at " << move << ".";

	return stats;
}

int checkForWinner()
{
	int xCounter = 0, oCounter = 0;
	//check for horizontal wins
	for(int i = 0; i < GRID_SIZE; ++i)
	{
		xCounter = 0;
		oCounter = 0;
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[i][j]=='_')
			{
				xCounter = 0;
				oCounter = 0;
			}
			else if(board[i][j]=='O')
			{
				++oCounter;
				xCounter = 0;
			}
			if(board[i][j]=='X')
			{
				++xCounter;
				oCounter = 0;
			}
			if(oCounter==4)
				return -1000;
			if(xCounter==4)
				return 1000;
		}
	}
	//check for vertical wins
	for(int i = 0; i < GRID_SIZE; ++i)
	{
		xCounter = 0;
		oCounter = 0;
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[j][i]=='_')
			{
				xCounter = 0;
				oCounter = 0;
			}
			else if(board[j][i]=='O')
			{
				++oCounter;
				xCounter = 0;
			}
			if(board[j][i]=='X')
			{
				++xCounter;
				oCounter = 0;
			}
			if(oCounter==4)
				return -1000;
			if(xCounter==4)
				return 1000;
		}
	}
	//check if game is not over yet
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
			if(board[i][j]=='_')
				return 0;
	//its a draw
	return 1;
}

void makeMove()
{
	int best = 0x80000000, depth = maxdepth, score, movei, movej;
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[i][j] == '_')
			{
				board[i][j] = 'X';
				score = min(depth - 1);
				if(score > best)
				{
					movei = i;
					movej = j;
					best = score;
				}
				board[i][j] = '_';
			}
		}
	cout<<"my move is "<<movei <<" "<< movej<< endl;
	board[movei][movej] = 'X';
}

void checkGameOver()
{
	printBoard();
	int result = checkForWinner();
	if (result == -1000) {
		cout <<"Human wins!"<< endl;
		exit(0);
	}
	if (result == 1000) {
		cout <<"Computer wins!"<< endl;
		exit(0);
	}
	if (result == 1) {
		cout << "It's a draw!" << endl;
		exit(0);
	}
}

int min(int depth)
{
	int best = 0x7FFFFFFF, score;
	if(checkForWinner() != 0)
		return checkForWinner();
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[i][j] == '_')
			{
				board[i][j] = 'O';
				score = max(depth - 1);
				if(score < best)
					best = score;
				board[i][j] = '_';
			}
		}
	return best;
}
int max(int depth)
{
	int best = 0x80000000, score;
	if(checkForWinner() != 0)
		return checkForWinner();
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[i][j] == '_')
			{
				board[i][j] = 'X';
				score = min(depth - 1);
				if(score > best)
					best = score;
				board[i][j] = '_';
			}
		}
	return best;
}
//returns the move to be made next. Starts searching board from the provided move.
string findNextMove(string move) {
	int x = convertStringToMoveValueX(move);
	int y = convertStringToMoveValueY(move);

	for (int i = x; i < GRID_SIZE; i++) {
		for (int j = y; j < GRID_SIZE; j++) {
			if (board[x][y] == '_') {
				return convertXYValuesToMoveString(x, y);
			}
		}
	}

	return "-1";
}

//converts the move string into an x-value for row index
int convertStringToMoveValueX(string move) {
	return toupper(move[0]) - 65;
}

//converts the move string into a y-value for column index
int convertStringToMoveValueY(string move) {
	return move[1] - '0' - 1;
}


//convert two coordinates into a move string. Returns a capital string like A6, instead of a6.
string convertXYValuesToMoveString(int x, int y) {
	char xChar = x + 65;
	char yChar = y + '0' + 1;

	return string() + xChar + yChar;
}