// cs420-4inALine.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <string>
#include <vector>
#include <ctime>
#include <stdlib.h>
#define GRID_SIZE 8

using namespace std;

struct moveStats {
	char xUpAmount;
	char xDownAmount;
	char xLeftAmount;
	char xRightAmount;

	char oUpAmount;
	char oDownAmount;
	char oLeftAmount;
	char oRightAmount;

	char upEmpties;
	char downEmpties;
	char leftEmpties;
	char rightEmpties;

	char upBreaker;
	char downBreaker;
	char leftBreaker;
	char rightBreaker;

	enum direction{UP=0,DOWN=1, LEFT=2, RIGHT=3};

	moveStats()
	{
		xUpAmount = 0;
		xDownAmount = 0;
		xLeftAmount = 0;
		xRightAmount = 0;
		oUpAmount = 0;
		oDownAmount = 0;
		oLeftAmount = 0;
		oRightAmount = 0;
		upEmpties = 0;
		downEmpties = 0;
		leftEmpties = 0;
		rightEmpties = 0;
	}

	void increment(char charAmountToIncrement, char direction){
		if (charAmountToIncrement == 'X' && direction == UP)xUpAmount++;
		else if (charAmountToIncrement == 'X' && direction == DOWN)xDownAmount++;
		else if (charAmountToIncrement == 'X' && direction == LEFT)xLeftAmount++;
		else if (charAmountToIncrement == 'X' && direction == RIGHT)xRightAmount++;
		else if (charAmountToIncrement == 'O' && direction == UP)oUpAmount++;
		else if (charAmountToIncrement == 'O' && direction == DOWN)oDownAmount++;
		else if (charAmountToIncrement == 'O' && direction == LEFT)oLeftAmount++;
		else if (charAmountToIncrement == 'O' && direction == RIGHT)oRightAmount++;
		else if (charAmountToIncrement == '_' && direction == UP)upEmpties++;
		else if (charAmountToIncrement == '_' && direction == DOWN)downEmpties++;
		else if (charAmountToIncrement == '_' && direction == LEFT)leftEmpties++;
		else if (charAmountToIncrement == '_' && direction == RIGHT)rightEmpties++;
	}

	int getCharAmount(char charAmountToGet, char direction) {
		if (charAmountToGet == 'X' && direction == UP)return xUpAmount;
		else if (charAmountToGet == 'X' && direction == DOWN)return xDownAmount;
		else if (charAmountToGet == 'X' && direction == LEFT)return xLeftAmount;
		else if (charAmountToGet == 'X' && direction == RIGHT)return xRightAmount;
		else if (charAmountToGet == 'O' && direction == UP)return oUpAmount;
		else if (charAmountToGet == 'O' && direction == DOWN)return oDownAmount;
		else if (charAmountToGet == 'O' && direction == LEFT)return oLeftAmount;
		else if (charAmountToGet == 'O' && direction == RIGHT)return oRightAmount;
		else if (charAmountToGet == '_' && direction == UP)return upEmpties;
		else if (charAmountToGet == '_' && direction == DOWN)return downEmpties;
		else if (charAmountToGet == '_' && direction == LEFT)return leftEmpties;
		else if (charAmountToGet == '_' && direction == RIGHT)return rightEmpties;
		return -1; //something went wrong
	}
};

void initializeBoard();
void printBoard();
bool makeMoveHuman(string move);
int checkForWinner();
void getMoveHuman();
moveStats getMoveStats(int x, int y);
int convertStringToMoveValueX(string move);
int convertStringToMoveValueY(string move);
string convertXYValuesToMoveString(int x, int y);
int min(int depth, clock_t startTime, int x, int y);
int max(int depth, clock_t startTime, int x, int y);
void checkGameOver();
void makeMoveAI();
void xOrO();
void getMaxTime();
void getWhoGoesFirst();
void getAIFirstMove();
int evaluate(int x, int y);

char board[GRID_SIZE][GRID_SIZE];
int maxdepth = 64, maxTime = -1;
char maxChar = 'X'; //computer
char minChar = 'O'; //human
int currentMax;
int currentMin;

int main()
{
	char c, d;
	initializeBoard();
	getMaxTime();
	getWhoGoesFirst();
	////create some phony data to make branching factor smaller
	//for(int i = 0; i < GRID_SIZE-1; ++i)
	//{
	//	c = i%2==0 ? 'X' : 'O';
	//	d = i%2==0 ? 'O' : 'X';
	//	for(int j = 0; j < GRID_SIZE; ++j)
	//		board[i][j] = j%2==0 ? c : d;
	//}
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
void getWhoGoesFirst()
{
	int response = -1;
	cout<<"Enter 1 to go first, 0 to go second: ";
	cin>>response;
	if(response==1)
		return;
	else if(response==0)
		getAIFirstMove();
	while(!(response == 1 || response == 0))
	{
		cout<<"Invalid input.\n";
		cout<<"Enter 1 to go first, 0 to go second: ";
		cin>>response;
		if(response==1)
			return;
		else if(response==0)
			getAIFirstMove();
	}
}
void getAIFirstMove()
{
	srand(time(0));
	//number between 2 and 5 inclusive
	int x = rand() % 4 + 2;
	//number between 2 and 5 inclusive
	int y = rand() % 4 + 2;
	board[x][y] = 'X';

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
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			if (board[i][j] == '_')return 0;
		}
	}
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

void makeMoveAI()
{
	clock_t startTime = clock();
	int best = 0x80000000, depth, score, movei, movej;
	currentMin = best;		//currentMin starts as lowest number because in Min, it will be updated.
	currentMax = 0x7FFFFFFF;	//currentMax starts as largest number because condition for update in Max.
	for(int depth = 1; depth < maxdepth; ++depth)
	{
		for(int i = 0; i < GRID_SIZE; ++i)
			for(int j = 0; j < GRID_SIZE; ++j)
			{
				if(board[i][j] == '_')
				{
					board[i][j] = 'X';
					score = min(depth - 1, startTime, i,j);
					if(score > best)
					{
						movei = i;
						movej = j;
						best = score;
						currentMax = best;
					}
					board[i][j] = '_';
					if((float)(clock() - startTime)/CLOCKS_PER_SEC > maxTime)
					{
						cout<<"My current move is: "<<convertXYValuesToMoveString(movei, movej)<<endl<<endl;
						board[movei][movej] = 'X';
						return;
					}
				}
			}
	}

	cout<<"My current move is: "<<convertXYValuesToMoveString(movei, movej)<<endl<<endl;
	board[movei][movej] = 'X';
}

int min(int depth, clock_t startTime, int x, int y)
{
	int best = 0x7FFFFFFF, score;
	if(checkForWinner() != 0)
		return checkForWinner();
	if(depth==0)
		return evaluate(x,y);
	if((float)(clock() - startTime)/CLOCKS_PER_SEC > maxTime)
		return evaluate(x,y);
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[i][j] == '_')
			{
				board[i][j] = 'O';
				score = max(depth - 1, startTime,i, j);
				if(score < best)
					best = score;
				board[i][j] = '_';
				if (currentMin < score) currentMin = score;
				else if (score < currentMin)return currentMin;
			}
		}
	return best;
}
int max(int depth, clock_t startTime, int x, int y)
{
	int best = 0x80000000, score;
	if(checkForWinner() != 0)
		return checkForWinner();
	if(depth==0)
		return evaluate(x,y);
	if((float)(clock() - startTime)/CLOCKS_PER_SEC > maxTime)
		return evaluate(x,y);
	for(int i = 0; i < GRID_SIZE; ++i)
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			if(board[i][j] == '_')
			{
				board[i][j] = 'X';
				score = min(depth - 1, startTime,i,j);
				if(score > best)
					best = score;
				board[i][j] = '_';
				if (currentMax > score) currentMax = score;
				else if (score > currentMax)return currentMax;
			}
		}
	return best;
}


int evaluate(int x, int y)
{
	moveStats stats = getMoveStats(x,y);
	int viability = 0;

	int oneOpponentVal = 200;
	if (stats.oUpAmount == 1)viability += oneOpponentVal;
	if (stats.oDownAmount == 1)viability += oneOpponentVal;
	if (stats.oLeftAmount == 1)viability += oneOpponentVal;
	if (stats.oRightAmount == 1)viability += oneOpponentVal;

	int twoOpponentVal = 1000;
	if (stats.oUpAmount == 2)viability += twoOpponentVal;
	if (stats.oDownAmount == 2)viability += twoOpponentVal;
	if(stats.oLeftAmount == 2)viability += twoOpponentVal;
	if(stats.oRightAmount == 2)viability += twoOpponentVal;

	int twoOpponentEmptyBreakersVal = 5250;
	if (stats.oUpAmount == 2 && stats.upBreaker == '_')viability += twoOpponentEmptyBreakersVal;
	if (stats.oDownAmount == 2 && stats.downBreaker == '_')viability += twoOpponentEmptyBreakersVal;
	if (stats.oLeftAmount == 2 && stats.leftBreaker == '_')viability += twoOpponentEmptyBreakersVal;
	if (stats.oRightAmount == 2 && stats.rightBreaker == '_')viability += twoOpponentEmptyBreakersVal;

	int threeOpponentVal = 6000;
	if (stats.oUpAmount == 3)viability += threeOpponentVal;
	if (stats.oDownAmount == 3)viability += threeOpponentVal;
	if (stats.oLeftAmount == 3)viability += threeOpponentVal;
	if (stats.oRightAmount == 3)viability += threeOpponentVal;

	int oneAIVal = 250;
	if (stats.xUpAmount == 1)viability += oneAIVal;
	if (stats.xDownAmount == 1)viability += oneAIVal;
	if (stats.xLeftAmount == 1)viability += oneAIVal;
	if (stats.xRightAmount == 1)viability += oneAIVal;

	//less important than twoOpponentVal
	int twoAIVal = 4000;
	if (stats.xUpAmount == 3)viability += twoAIVal;
	if (stats.xDownAmount == 3)viability += twoAIVal;
	if (stats.xLeftAmount == 3)viability += twoAIVal;
	if (stats.xRightAmount == 3)viability += twoAIVal;

	//VERY IMPORTANT! YOU CAN WIN!
	int threeAIVal = 9001;
	if (stats.xUpAmount == 3)viability += threeAIVal;
	if (stats.xDownAmount == 3)viability += threeAIVal;
	if (stats.xLeftAmount == 3)viability += threeAIVal;
	if (stats.xRightAmount == 3)viability += threeAIVal;

	//Keep away from sides that will give you no possible space to win.
	if (stats.xUpAmount + stats.downEmpties + 1 >= 4)viability += 100;
	if (stats.xDownAmount + stats.upEmpties + 1 >= 4)viability += 100;
	if (stats.xLeftAmount + stats.rightEmpties + 1 >= 4)viability += 100;
	if (stats.xRightAmount + stats.leftEmpties + 1 >= 4)viability += 100;
	
	return viability;
}

//currently, this gets the total amount of repeated o,x, or empties that are adjacent to the given move.
//It will likely have to be modified once a proper evaluation function is discovered.
//I'm not sure if it's important that we keep information on the direction of each repeat. If so, that can probably
//be easily accomplished via an array in the struct.
moveStats getMoveStats(int x, int y) {
	int i = 1;
	char repeatChar;
	moveStats stats;

	if (x - 1 > 0) {
		repeatChar = board[x - i][y];

		//Checking upward for repeats
		while (board[x - i][y] == repeatChar && x - i >= 0) {
			stats.increment(repeatChar, stats.UP);
			i++;
		}
		if (x - i >= 0)stats.upBreaker = board[x - i][y];
		else stats.upBreaker = board[x - i + 1][y];
	}
	if (y + 1 < GRID_SIZE) {
		i = 1;
		repeatChar = board[x][y + 1];

		//Checking right for repeats
		while (board[x][y + i] == repeatChar && y + i < GRID_SIZE) {
			stats.increment(repeatChar, stats.RIGHT);
			i++;
		}
		if (y + i < GRID_SIZE)stats.rightBreaker = board[x][y+i];
		else stats.rightBreaker = board[x][y+i-1];
	}
	if (x + 1 < GRID_SIZE) {
		i = 1;
		repeatChar = board[x + 1][y];

		//Checking down for repeats
		while (board[x + i][y] == repeatChar && x + i < GRID_SIZE) {
			stats.increment(repeatChar, stats.DOWN);
			i++;
		}

		if (x + i < GRID_SIZE)stats.downBreaker = board[x + i][y];
		else stats.downBreaker = board[x + i - 1][y];
	}
	if (y - 1 > 0) {
		i = 1;
		repeatChar = board[x][y - 1];

		//Checking left for repeats
		while (board[x][y - i] == repeatChar && y - i >= 0) {
			stats.increment(repeatChar, stats.LEFT);
			i++;
		}

		if (y - i >= 0)stats.leftBreaker = board[x][y - i];
		else stats.leftBreaker = board[x][y - i + 1];
	}

	//cout << stats.upBreaker << stats.rightBreaker << stats.downBreaker << stats.leftBreaker <<endl;

	return stats;
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