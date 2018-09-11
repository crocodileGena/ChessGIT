// ChessConsole.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "board.h"

int main()
{
	Board* newBoard = new Board;

	// Make Moves
	newBoard->ResetBoard();
	newBoard->PrintBoard();
	newBoard->MovePiece(B1, C3);
	newBoard->PrintBoard();
	newBoard->MovePiece(A7, A6);
	newBoard->PrintBoard();
	newBoard->MovePiece(C3, B5);
	newBoard->PrintBoard();
	newBoard->MovePiece(A6, A5);
	newBoard->PrintBoard();
	newBoard->MovePiece(B5, C7);
	newBoard->PrintBoard();

	// Test Prints
	newBoard->PrintNotation();
	newBoard->PrintPiecesSum();
    return 0;
}

