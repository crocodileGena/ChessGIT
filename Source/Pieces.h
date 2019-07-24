#pragma once

#include <string>
#include <vector>

#include "board.h"


class Piece
{
public:
	Piece(const Color in_color) : m_worth(0), m_color(in_color) {}
	virtual ~Piece() {}
	static Piece* NewPiece(const Pieces which_piece);

	virtual void OnPieceMoved(Board& /*in_board*/) {};
	virtual bool IsMoveLegal(Board &board, const Square source, const Square dest) = 0;
	virtual std::vector<Piece*> CanPieceCapture(Board &board, const Square source) = 0;
	virtual bool isEnPassantMove(const Color /*in_color*/, const Square /*in_source*/, const Square /*in_dest*/) { return false; }
	std::vector<Move> GetLegalMoves(const Board& in_board, const Square origin);
	virtual std::vector<Move> GetLegalMovesSelf(const Board& in_board, const Square origin) = 0;

	int m_worth;
	Color m_color;
	std::string m_name;
};

class Pawn : public Piece
{
public:
	Pawn(const Color in_color) : Piece(in_color)
	{ 
		m_worth = kPawnWorth; m_name = "P";
	}
	~Pawn() {}

	bool IsMoveLegal(Board &board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board &board, const Square source) override;
	void OnPieceMoved(Board &board) override;
	bool isEnPassantMove(const Color in_color, const Square in_source, const Square in_dest) override;
	std::vector<Move> GetLegalMovesSelf(const Board& in_board, const Square origin) override;
};

class Bishop : public Piece
{
public:
	Bishop(const Color in_color) : Piece(in_color) { m_worth = kBishopWorth; m_name = "B"; }
	~Bishop() {}

	bool IsMoveLegal(Board &board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board &board, const Square source) override;
	std::vector<Move> GetLegalMovesSelf(const Board& in_board, const Square origin) override;
};

class Knight : public Piece
{
public:
	Knight(const Color in_color) : Piece(in_color) { m_worth = kKnightWorth; m_name = "N";
	}
	~Knight() {}

	bool IsMoveLegal(Board &board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board &board, const Square source) override;
	std::vector<Move> GetLegalMovesSelf(const Board& in_board, const Square origin) override;
};

class Rook : public Piece
{
public:
	Rook(const Color in_color) : Piece(in_color), m_bCastleAllowed(true) { m_worth = kRookWorth; m_name = "R";
	}
	~Rook() {}

	void OnPieceMoved(Board &board) override;
	bool IsMoveLegal(Board &board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board &board, const Square source) override;
	std::vector<Move> GetLegalMovesSelf(const Board& in_board, const Square origin) override;

	bool m_bCastleAllowed;
};

class Queen : public Piece
{
public:
	Queen(const Color in_color) : Piece(in_color) { m_worth = kQueenWorth; m_name = "Q";
	}
	~Queen() {}

	bool IsMoveLegal(Board &board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board &board, const Square source) override;
	std::vector<Move> GetLegalMovesSelf(const Board& in_board, const Square origin) override;
};

class King : public Piece
{
public:
	King(const Color in_color) : 
		Piece(in_color),
		m_bCastleAllowed(true),
		m_castleRookSquare(kIllegalSquare, kIllegalSquare)
	{ m_worth = kKingWorth; m_name = "K";
	}
	~King() {}

	void OnPieceMoved(Board &board) override;
	bool IsMoveLegal(Board &board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board &board, const Square source) override;
	std::vector<Move> GetLegalMovesSelf(const Board& in_board, const Square origin) override;

	bool m_bCastleAllowed;
private:
	Square m_castleRookSquare;
};

