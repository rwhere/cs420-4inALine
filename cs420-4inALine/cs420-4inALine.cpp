// cs420-4inALine.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <vector>
#define GRID_SIZE 8
#define X 'X'
#define O 'O'

using namespace std;

struct moveStats {
	int xAmount = 0;
	int oAmount = 0;
	int empties = 0;

	void increment(char charAmountToIncrement){
		if (charAmountToIncrement == 'X')xAmount++;
		else if (charAmountToIncrement == 'O')oAmount++;
		else if (charAmountToIncrement == '_')empties++;
	}

	int getCharAmount(char charAmountToGet) {
		if (charAmountToGet == 'X')return xAmount;
		else if (charAmountToGet == 'O')return oAmount;
		else if (charAmountToGet == '_')return empties;
	}
};

struct TreeNode{
	TreeNode *parent;
	vector<TreeNode *> children;
	char board[GRID_SIZE][GRID_SIZE];
	int viability;
	string move;

	TreeNode() {

	}

	TreeNode(char copyFromBoard[GRID_SIZE][GRID_SIZE]) {
		arrayCopy(board, copyFromBoard);
	}

	TreeNode(char copyFromBoard[GRID_SIZE][GRID_SIZE], string move, TreeNode *parent) {
		arrayCopy(board, copyFromBoard);
		this->move = move;
		this->parent = parent;
	}

	TreeNode::~TreeNode();

	void arrayCopy(char copyTo[GRID_SIZE][GRID_SIZE] ,char copyFrom[GRID_SIZE][GRID_SIZE]){
		for (int i = 0; i < GRID_SIZE; i++) {
			for (int j = 0; j < GRID_SIZE; j++) {
				copyTo[i][j] = copyFrom[i][j];
			}
		}
	}

};

void initializeBoard(char board[][GRID_SIZE]);
void printBoard(char board[][GRID_SIZE]);
bool makeMove(char board[][GRID_SIZE], char player, string move);
int checkForWinner(char board[][GRID_SIZE]);
void getMove(char board[][GRID_SIZE]);
moveStats getMoveStats(char board[][GRID_SIZE], string move);
int convertStringToMoveValueX(string move);
int convertStringToMoveValueY(string move);
string convertXYValuesToMoveString(int x, int y);

int main()
{
	char board[GRID_SIZE][GRID_SIZE];
	initializeBoard(board);
	printBoard(board);
	makeMove(board, X, "a4");
	makeMove(board, X, "a5");
	makeMove(board, O, "a6");
	makeMove(board, O, "b6");
	makeMove(board, O, "c6");
	printBoard(board);
	getMoveStats(board, "a6");

	cout << convertXYValuesToMoveString(0, 5);

	getMove(board);
	printBoard(board);

	if(checkForWinner(board) != 0)
		cout<<"we have a winner!\n";
	else
		cout<<"no winner\n";
	return 0;
}

void initializeBoard(char board[][GRID_SIZE])
{
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
			board[i][j] = '_';
}
void printBoard(char board[][GRID_SIZE])
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
bool makeMove(char board[][GRID_SIZE], char player, string move)
{
	if(toupper(move[0])-65 < 0 || toupper(move[0])-65 > 7 || move[1] - '0' - 1 < 0 || move[1] - '0' - 1 > 7 || move.size() > 2)
		return false;
	if(board[toupper(move[0]) - 65][move[1]- '0' - 1] != '_')
		return false;
	else
		board[toupper(move[0]) - 65][move[1]- '0' - 1] = player;
	return true;
}
void getMove(char board[][GRID_SIZE])
{
	string move;
	cout<<"Choose your next move: ";
	cin>>move;
	while(!makeMove(board, O, move))
	{
		cout<<"Invalid move.\nChoose your next move: ";
		cin>>move;
	}
}


//currently, this gets the total amount of repeated o,x, or empties that are adjacent to the given move.
//It will likely have to be modified once a proper evaluation function is discovered.
//I'm not sure if it's important that we keep information on the direction of each repeat. If so, that can probably
//be easily accomplished via an array in the struct.
moveStats getMoveStats(char board[][GRID_SIZE], string move) {
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

int checkForWinner(char board[][GRID_SIZE])
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
				return 1;
			if(xCounter==4)
				return -1;
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
				return 1;
			if(xCounter==4)
				return -1;
		}
	}
	return 0;
}

void solverAI(){
	TreeNode root;

	
	
}

//returns the move to be made next. Starts searching board from the provided move.
string findNextMove(char board[][GRID_SIZE], string move) {
	int x = convertStringToMoveValueX(move);
	int y = convertStringToMoveValueY(move);

	for (int i = x; i < GRID_SIZE; i++) {
		for (int j = y; j < GRID_SIZE; j++) {
			if (board[x][y] == '_') {
				return convertXYValuesToMoveString(x, y);
			}
		}
	}
}


//converts the move string into an x-value for row index
int convertStringToMoveValueX(string move) {
	return toupper(move[0]) - 65;
}

//converts the move string into a y-value for column index
int convertStringToMoveValueY(string move) {
	return move[1] - '0' - 1;
}

string convertXYValuesToMoveString(int x, int y) {
	char xChar = x + 65;
	char yChar = y + '0' + 1;

	return string() + xChar + yChar;
}


TreeNode::~TreeNode(void) {
	children.clear();
}