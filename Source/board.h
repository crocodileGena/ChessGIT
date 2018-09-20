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
	Square() : m_rank(kIllegalSquare), m_file(kIllegalSquare) {}
	Square(const int inPos[2]) { m_file = inPos[0]; m_rank = inPos[1]; }
	Square(const int letter, const int number) { m_file = letter; m_rank = number; }
	~Square() {}

	bool operator==(const Square &other) { return (m_file == other.m_file && m_rank == other.m_rank); }
	int GetFile() const { return m_file; }
	int GetRank() const { return m_rank; }
	void SetFile(int in_file) { m_file = in_file; }
	void SetRank(int in_rank) { m_rank = in_rank; }
	void SetSquare(int in_file, int in_rank) { m_file = in_file; m_rank = in_rank; }
	std::string toString() const;
private:
	int m_rank;
	int m_file;
};

class GameNotation
{
public:
	GameNotation() {}
	~GameNotation() { m_vNotation.clear(); }

	void PushMove(const std::string &in_piecesPosition, const std::string &pieceName,
		Square in_origin, const Square in_dest, const bool isCapture,
		const bool specifyRank, const bool specifyFile, const bool whitesMove, 
		const bool* castlingOptions, const Square enPassant, const int halfmoveClock);
	std::string GetFENFromPosition(const std::string in_position, const bool whitesMove,
									const bool* castlingOptions, const Square enPassant,
									const int halfmoveClock);
	void PositionToString(const std::string &in_position, std::string &retVal);
	void GetCastlingString(const bool* in_options, std::string &retVal);
	std::string GetAlgebraic(const std::string &pieceName, const Square in_origin, const Square in_dest, const bool isCapture, const bool specifyRank, const bool specifyFile);

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
	void SetPiece(const Square inLocation, Piece* inPiece) { board[inLocation.GetFile()][inLocation.GetRank()] = inPiece; }
	void PrintPiecesSum();
	bool CheckIsCheck();
	std::string GetStatus() { return m_status; }
	std::string GetPiecesPosition();
	void UpdateCastlingFlag(const Piece* in_piece, const Square in_origin);
	bool* GetCastlingFlag() { return m_castlingFlag; }
	Square GetEnPassantSquare(const Square inBase, const Square inDest);
	void UpdateHalfmoveClock(const bool isCapture, const bool isPawn);

	int m_lastColorMoved;
	Piece* board[BoardSize][BoardSize];
	GameNotation m_gameNotation;
	std::string m_status;
	bool m_castlingFlag[numCastlingOptions];
	int m_halfmoveClock;

};
