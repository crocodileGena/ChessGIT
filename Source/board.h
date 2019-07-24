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
	Square(const int input[2]) { m_file = input[eFile]; m_rank = input[eRank]; }
	Square(const int file, const int rank) { m_file = file; m_rank = rank; }
	~Square() {}

	bool Square::operator==(const Square &other) const { return (m_file == other.m_file && m_rank == other.m_rank); }
	int GetFile() const { return m_file; }
	int GetRank() const { return m_rank; }
	void SetFile(int in_file) { m_file = in_file; }
	void SetRank(int in_rank) { m_rank = in_rank; }
	void SetSquare(int in_file, int in_rank) { m_file = in_file; m_rank = in_rank; }
	std::string toString() const;
	bool InBounds() const;
private:
	int m_rank;
	int m_file;
};

struct Move
{
	Move() : m_origin({ kIllegalSquare, kIllegalSquare }), m_dest({ kIllegalSquare, kIllegalSquare }) {}
	Move(const Square in_origin, const Square in_dest) : m_origin(in_origin), m_dest(in_dest) {}
	bool Move::operator==(const Move &other) const { return (m_origin == other.m_origin && m_dest == other.m_dest); }
	bool Move::operator!=(const Move &other) const { return *this == other ? false : true; }

	Square m_origin;
	Square m_dest;
};

class NotationNode
{
public:
	NotationNode(const std::string in_algebraic, const std::string in_fen, const std::string in_position) : 
		m_algebraic(in_algebraic), m_fen(in_fen), m_position(in_position) {}
	~NotationNode() {}

	void UpdatePromotedPawn(const std::string piece_name) 
	{ 
		if (m_fen.find('w') == std::string::npos)
			m_fen[m_fen.find_first_of('P')] = piece_name[0];
		else
			m_fen[m_fen.find_last_of('p')] = piece_name[0];

		m_algebraic += "=" + piece_name; 
	}
	std::string GetAlgebraic() { return m_algebraic; }
	std::string GetFEN() { return m_fen; }
	std::string GetPosition() { return m_position; }

private:
	std::string m_algebraic;	//move
	std::string m_fen;			//state
	std::string m_position;		//first part of fen
};

class GameNotation
{
public:
	GameNotation() {}
	GameNotation(const GameNotation& in_notation) : m_vNotation(in_notation.m_vNotation)
	{}
	~GameNotation() { m_vNotation.clear(); }
	void Reset() { m_vNotation.clear(); }

	size_t PushMove(const std::string &in_piecesPosition, const std::string &pieceName, Square in_origin, const Square in_dest,
					const bool isCapture, const bool specifyRank, const bool specifyFile,
					const bool whitesMove, const bool* castlingOptions, const Square enPassant, const int halfmoveClock, CheckOrMate checkOrMate);
	bool IsPerpetual(const std::string &in_piecesPosition, const bool whitesMove);
	void ParseFEN(const std::string& in_fen, std::string& position, bool* castlingOptions, Square& enPassantDestSquare, int& halfMoveClock, bool& whitesMove);
	void UpdatePromotedPawn(const std::string pieceName) { m_vNotation.back().UpdatePromotedPawn(pieceName); }
	NotationNode GetLastNode() { return m_vNotation.back(); }
	void GetLastNode(std::string& in_algebraic, std::string& in_fen)
	{ 
		in_algebraic = m_vNotation.back().GetAlgebraic(); 
		in_fen = m_vNotation.back().GetFEN();
	}
	size_t GetNumberOfNodes() const { return m_vNotation.size(); }

private:
	std::string GetAlgebraic(const std::string &pieceName, const Square in_origin, const Square in_dest, const bool isCapture, const bool specifyRank, const bool specifyFile, CheckOrMate checkOrMate);
	std::string GetFENFromPosition(const std::string in_position, const bool whitesMove,
		const bool* castlingOptions, const Square enPassant,
		const int halfmoveClock);
	void PositionToString(const std::string &in_position, std::string &retVal);
	void GetCastlingString(const bool* in_options, std::string &retVal);

	std::vector<NotationNode> m_vNotation;
};

class Board
{
public:
	Board();
	Board(const Board& in_board);
	~Board() {}

	void PrintBoard();
	void ResetBoard();
	void RemovePieces();
	void PlacePieces(const std::string in_position);
	void PiecesSum(int& whiteSum, int& blackSum);
	
	bool MovePiece(const Square inBase, const Square inDest);
	Piece* GetPiece(const Square inLocation) const;
	void SetPiece(const Square inLocation, Piece* inPiece) { board[inLocation.GetFile()][inLocation.GetRank()] = inPiece; }
	std::vector<Piece*> GetPieces();
	
	bool CheckIsCheck();
	bool CheckIsMate();
	CheckOrMate DeriveCheckOrMate(const std::string piecesPosition);

	std::string GetStatus() { return m_status; }
	CheckOrMate GetCheckOrMate() const { return m_checkOrMate; }
	Square GetEnPassantSquare() const { return m_enPassantSquare; }
	const bool* GetCastlingFlag() const { return m_castlingFlag; }
	bool GetQueeningMode() { return m_queeningMode; }
	std::string GetPiecesPosition();
	std::vector<Move> GetLegalMoves(Color for_which_color);
	size_t GetCurrentNotationIndex() const { return m_currNotationIndex; }
	int  GetBalance() { return m_balance; }

	void SetEnPassantSquare(const Square in_square) { m_enPassantSquare = in_square; }
	void SetQueeningMode(const bool in_queeningMode) { m_queeningMode = in_queeningMode; }
	void SetCheckOrMate(CheckOrMate in_checkormate) { m_checkOrMate = in_checkormate; m_status = in_checkormate == eCheck? "Check" :"Mate"; }
	void SetBalance(const int in_balance) { m_balance = in_balance; }

	void UpdateCastlingFlag(const Piece* in_piece, const Square in_origin);
	void UpdateHalfmoveClock(const bool isCapture, const bool isPawn);
	void QueenAPawn(const Square in_square, const std::string in_piece);
	void RemoveUndefendedCheckMoves(std::vector<Move>& legalMoves) const;
	bool IsADraw(const bool noLegalMoves, const std::string piecesPosition);
	void LoadFEN(const std::string in_position = fenOpening, const size_t nodeNumber = 1);
	int	 CalculateBalance();

	Color			m_lastColorMoved;
	Piece*			board[BoardSize][BoardSize];
	GameNotation	m_gameNotation;
	std::string		m_status;
	bool			m_castlingFlag[numCastlingOptions];
	int				m_halfmoveClock;
	bool			m_queeningMode;
	int				m_balance = 0;

private:
	void CommitMove(Piece * currPiece, const Square &inBase, const Square &inDest);
	void UpdateEnPassantSquare(Piece * currPiece, const Square & inBase, const Square & inDest);
	bool CanPieceCaptureKing(std::vector<Piece *> &captures);

	size_t m_currNotationIndex;
	Square m_enPassantSquare;
	CheckOrMate m_checkOrMate;
};
