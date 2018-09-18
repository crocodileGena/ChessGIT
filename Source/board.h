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

	void PushMove(const std::string in_Name, const Color in_color, const Square inDest);
	void PrintNotation();

private:
	std::vector<NotationNode> m_vNotation;

};

class NotationNode
{
public:
	NotationNode(const std::string in_Name, const Square inDest, size_t in_moveNumber);
	~NotationNode() {}

	void UpdateNode(const std::string in_Name, const Square inDest);
	void Print();

private:
	size_t m_moveNumber;
	std::string m_whiteMove;
	std::string m_blackMove;
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
	void PrintNotation() { m_gameNotation.PrintNotation(); }
	void PrintPiecesSum();
	bool CheckIsCheck();
	std::string GetStatus() { return m_status; }

	int m_lastColorMoved;
	Piece* board[BoardSize][BoardSize];
	GameNotation m_gameNotation;
	std::string m_status;
};
