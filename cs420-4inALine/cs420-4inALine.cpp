// cs420-4inALine.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <iomanip>
#include <ctype.h>
#define GRID_SIZE 8
#define X 'X'
#define O 'O'

using namespace std;

void initializeBoard(char board[][GRID_SIZE]);
void printBoard(char board[][GRID_SIZE]);
void makeMove(char board[][GRID_SIZE], char player, string move);

int main()
{
	char board[GRID_SIZE][GRID_SIZE];
	initializeBoard(board);
	printBoard(board);
	makeMove(board, X, "A5");
	printBoard(board);
	makeMove(board, O, "D6");
	printBoard(board);

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
void makeMove(char board[][GRID_SIZE], char player, string move)
{
	board[toupper(move[0]) - 65][move[1]-'0' - 1] = player;
}
