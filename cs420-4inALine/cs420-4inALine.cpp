// cs420-4inALine.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <vector>
#include <ctime>
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
void getMoveHuman();
moveStats getMoveStats(string move);
int convertStringToMoveValueX(string move);
int convertStringToMoveValueY(string move);
string convertXYValuesToMoveString(int x, int y);
int min(int depth, clock_t startTime);
int max(int depth, clock_t startTime);
void checkGameOver();
void makeMoveAI();
void xOrO();
void getMaxTime();

char board[GRID_SIZE][GRID_SIZE];
int maxdepth = 64, maxTime = -1;
char maxChar = 'X'; //computer
char minChar = 'O'; //human

int main()
{
	char c, d;
	initializeBoard();
	getMaxTime();
	//create some phony data to make branching factor smaller
	for(int i = 0; i < GRID_SIZE-1; ++i)
	{
		c = i%2==0 ? 'X' : 'O';
		d = i%2==0 ? 'O' : 'X';
		for(int j = 0; j < GRID_SIZE; ++j)
			board[i][j] = j%2==0 ? c : d;
	}
	printBoard();
	//xOrO();
	for (;;) {
		getMoveHuman();
		checkGameOver();
		makeMoveAI();
		checkGameOver();
	}
	return 0;
}
void getMaxTime()
{
	cout<<"How many seconds do I have to make a move: ";
	cin>>maxTime;
	while(maxTime <= 0)
	{
		cout<<"Invalid amount of seconds.\n";
		cout<<"How many seconds do I get to make a move: ";
		cin>>maxTime;
	}
}
void xOrO(){
	while (maxChar != 'X' && maxChar != 'O') {
		cout << "Is the AI 'X' , or 'O'?";
		cin >> maxChar;
		if (maxChar != 'X' && maxChar != 'O') {
			cout << "Please put a capital 'X' or 'O'.";
		}
	}
	if (maxChar == 'X')minChar = 'O';
	else minChar = 'X';
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
	cout<<"\n";
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
void getMoveHuman()
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

int evaluate() {
	return 0;
}

//Have to modify the values that this returns for computer or human wins, according to our eval function.
//Or model the returned eval function values based on this.
int checkForWinner()
{
	int maxCounter = 0, minCounter = 0;
	//check for horizontal wins
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		maxCounter = 0;
		minCounter = 0;
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			if (board[i][j] == '_')
			{
				maxCounter = 0;
				minCounter = 0;
			}
			else if (board[i][j] == minChar)
			{
				++minCounter;
				maxCounter = 0;
			}
			if (board[i][j] == maxChar)
			{
				++maxCounter;
				minCounter = 0;
			}
			if (minCounter == 4)
				return -5000;
			if (maxCounter == 4)
				return 5000;
		}
	}
	//check for vertical wins
	for (int i = 0; i < GRID_SIZE; ++i)
	{
		maxCounter = 0;
		minCounter = 0;
		for (int j = 0; j < GRID_SIZE; ++j)
		{
			if (board[j][i] == '_')
			{
				maxCounter = 0;
				minCounter = 0;
			}
			else if (board[j][i] == minChar)
			{
				++minCounter;
				maxCounter = 0;
			}
			else if(board[j][i] == maxChar)
			{
				++maxCounter;
				minCounter = 0;
			}
			if (minCounter == 4)
				return -5000;
			if (maxCounter == 4)
				return 5000;
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

void checkGameOver() {
	printBoard();
	int result = checkForWinner();
	if (result == -5000) {
		cout << "you win" << endl;
		exit(0);
	}
	if (result == 5000) {
		cout << "I win" << endl;
		exit(0);
	}
	if (result == 1) {
		cout << "draw" << endl;
		exit(0);
	}
}

//This will have to be adapted with Iterative Deepening and a time check.
//Not sure those changes will go in here, or in the solverAI function that calls this one.
//I'm leaning towards having them in here so we don't have to deal with passing the chosen coordinates around.
void makeMoveAI()
{
	clock_t startTime = clock();
	int best = 0x80000000, depth, score, movei, movej;
	for(int depth = 1; depth < maxdepth; ++depth)
	{
		for(int i = 0; i < GRID_SIZE; ++i)
			for(int j = 0; j < GRID_SIZE; ++j)
			{
				if(board[i][j] == '_')
				{
					board[i][j] = 'X';
					score = min(depth - 1, startTime);
					if(score > best)
					{
						movei = i;
						movej = j;
						best = score;
					}
					board[i][j] = '_';
				}
			}
	}
	//cout<<"my move is "<<movei <<" "<< movej<< endl;
	cout<<"My current move is: "<<convertXYValuesToMoveString(movei, movej)<<endl<<endl;
	board[movei][movej] = 'X';
}

int min(int depth, clock_t startTime)
{
	int best = 0x7FFFFFFF, score;
	if(checkForWinner() != 0)
		return checkForWinner();
	if(depth==0)
		return evaluate();
	if((float)(clock() - startTime)/CLOCKS_PER_SEC > maxTime)
		return evaluate();
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[i][j] == '_')
			{
				board[i][j] = 'O';
				score = max(depth - 1, startTime);
				if(score < best)
					best = score;
				board[i][j] = '_';
			}
		}
	return best;
}
int max(int depth, clock_t startTime)
{
	int best = 0x80000000, score;
	if(checkForWinner() != 0)
		return checkForWinner();
	if(depth==0)
		return evaluate();
	if((float)(clock() - startTime)/CLOCKS_PER_SEC > maxTime)
		return evaluate();
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[i][j] == '_')
			{
				board[i][j] = 'X';
				score = min(depth - 1, startTime);
				if(score > best)
					best = score;
				board[i][j] = '_';
			}
		}
	return best;
}
//returns the move to be made next. Starts searching board from the provided move.
//This may be able to be integrated into MinMax for our benefit.
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