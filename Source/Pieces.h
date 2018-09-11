#pragma once

#include <string>
#include <vector>

#include "board.h"


class Piece
{
public:
	Piece(const Color in_color) : m_worth(0), m_color(in_color) {}
	~Piece() {}

	virtual void OnPieceMoved() {};
	virtual bool MakeMove(Board board, const Square source, const Square dest) = 0;
	virtual std::vector<Piece*> CanPieceCapture(Board board, const Square source) = 0;

	int m_worth;
	Color m_color;
	std::string m_name;
};

class Pawn : public Piece
{
public:
	Pawn(const Color in_color) : Piece(in_color) { m_worth = Worth::ePawn; }
	~Pawn() {}

	bool MakeMove(Board board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board board, const Square source) override;
};

class Bishop : public Piece
{
public:
	Bishop(const Color in_color) : Piece(in_color) { m_worth = Worth::eBishop; m_name = "B"; }
	~Bishop() {}

	bool MakeMove(Board board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board board, const Square source) override;
};

class Knight : public Piece
{
public:
	Knight(const Color in_color) : Piece(in_color) { m_worth = Worth::eKnight; m_name = "N";
	}
	~Knight() {}

	bool MakeMove(Board board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board board, const Square source) override;
};

class Rook : public Piece
{
public:
	Rook(const Color in_color) : Piece(in_color), m_bCastleAllowed(true) { m_worth = Worth::eRook; m_name = "R";
	}
	~Rook() {}

	void OnPieceMoved() override;
	bool MakeMove(Board board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board board, const Square source) override;

	bool m_bCastleAllowed;
};

class Queen : public Piece
{
public:
	Queen(const Color in_color) : Piece(in_color) { m_worth = Worth::eQueen; m_name = "Q";
	}
	~Queen() {}

	bool MakeMove(const Board board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board board, const Square source) override;
};

class King : public Piece
{
public:
	King(const Color in_color) : Piece(in_color), m_bCastleAllowed(true) { m_worth = Worth::eKing; m_name = "K";
	}
	~King() {}

	void OnPieceMoved() override;
	bool MakeMove(Board board, const Square source, const Square dest) override;
	std::vector<Piece*> CanPieceCapture(Board board, const Square source) override;

	bool m_bCastleAllowed;
};

