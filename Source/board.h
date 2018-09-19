#pragma once

#include <iostream>
#include "consts.h"
#include <string>
#include <vector>

class Piece;
class NotationNode;

class Square
{
public:
	Square() {}
	Square(const int inPos[2]) { m_pos[0] = inPos[0]; m_pos[1] = inPos[1]; }
	Square(const int letter, const int number) { m_pos[0] = letter; m_pos[1] = number; }
	~Square() {}

	bool operator==(const Square &other) { return (m_pos[0] == other.m_pos[0] && m_pos[1] == other.m_pos[1]); }
	int GetLetter() const { return m_pos[eLetter]; }
	int GetNumber() const { return m_pos[eNumber]; }
	std::string toString() const;
private:
	int m_pos[2];
};

class GameNotation
{
public:
	GameNotation() {}
	~GameNotation() { m_vNotation.clear(); }

	void PushMove(const std::string in_piecesPosition, const std::string pieceName,
		Square in_origin, const Square in_dest, const bool isCapture,
		const bool specifyRank, const bool specifyFile, const bool whitesMove, 
		const int castlingOptions, const Square enPassant, const int halfmoveClock);
	std::string GetFENFromPosition(const std::string in_position, const bool whitesMove,
									const int castlingOptions, const Square enPassant,
									const int halfmoveClock);
	std::string GetAlgebraic(const Square in_origin, const Square in_dest, const bool isCapture, const bool specifyRank, const bool specifyFile);

private:
	std::vector<NotationNode> m_vNotation;

};

class NotationNode
{
public:
	NotationNode(const std::string in_algebraic, const std::string in_fen) : 
		m_algebraic(in_algebraic), m_fen(in_fen) {}
	~NotationNode() {}

	std::string GetAlgebraic() { return m_algebraic; }
	std::string GetFEN() { return m_fen; }

private:
	std::string m_algebraic;	//move
	std::string m_fen;			//state
};

class Board
{
public:
	Board();
	~Board() {}

	void PrintBoard();
	void ResetBoard();
	void MovePiece(const Square inBase, const Square inDest);
	Piece* GetPiece(const Square inLocation);
	void SetPiece(const Square inLocation, Piece* inPiece) { board[inLocation.GetLetter()][inLocation.GetNumber()] = inPiece; }
	void PrintPiecesSum();
	bool CheckIsCheck();
	std::string GetStatus() { return m_status; }
	std::string GetPiecesPosition();
	int GetCastlingOptions();
	Square GetEnPassantSquare(const Square inBase, const Square inDest);
	int GetHalfmoveClock();

	int m_lastColorMoved;
	Piece* board[BoardSize][BoardSize];
	GameNotation m_gameNotation;
	std::string m_status;
};
