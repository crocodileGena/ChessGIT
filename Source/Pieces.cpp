//#include "stdafx.h"
#include "Pieces.h"
#include <stdlib.h>

void RecursiveCheckCaptures(Board board, Square source, int xDirection, int yDirection, std::vector<Piece*>& outCaptures, Color canCaptureColor)
{
	Square dest({ source.GetLetter() + xDirection, source.GetNumber() + yDirection });
	if (dest.GetLetter() > BoardSize - 1 || dest.GetLetter() < 0 ||
		dest.GetNumber() > BoardSize - 1 || dest.GetNumber() < 0)
		return;
	Piece* destPiece = board.GetPiece(dest);
	if (destPiece && destPiece->m_color == canCaptureColor)
	{
		outCaptures.push_back(destPiece);
		return;
	}
	RecursiveCheckCaptures(board, dest, xDirection, yDirection, outCaptures, canCaptureColor);

	
}

std::vector<Piece*> DiagonalsCaptures(Board board, Square source, Color canCaptureColor)
{
	std::vector<Piece*> retVal;
	RecursiveCheckCaptures(board, source, 1, 1, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, 1, -1, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, -1, 1, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, -1, -1, retVal, canCaptureColor);
	return retVal;
}

std::vector<Piece*> StraightCaptures(Board board, Square source, Color canCaptureColor)
{
	std::vector<Piece*> retVal;
	RecursiveCheckCaptures(board, source, 1, 0, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, -1, 0, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, 0, 1, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, 0, -1, retVal, canCaptureColor);
	return retVal;
}

bool IsDiagonalPath(const Square source, const Square dest)
{
	return (source.GetLetter() - dest.GetLetter() == source.GetNumber() - dest.GetNumber() ||
			source.GetLetter() - dest.GetLetter() == dest.GetNumber() - source.GetNumber());
}

bool IsVerticalPath(const Square source, const Square dest)
{
	return source.GetLetter() == dest.GetLetter();
}

bool IsHorizontalPath(const Square source, const Square dest)
{
	return source.GetNumber() == dest.GetNumber();
}

bool IsVerticalClear(Board board, const Square source, const Square dest)
{
	if (source.GetNumber() == dest.GetNumber())
		return true;

	int verticalPolarity = dest.GetNumber() - source.GetNumber() > 0 ? 1 : -1;
	Square next(source.GetLetter(), source.GetNumber() + verticalPolarity);

	if (nullptr != board.GetPiece(next) && next.GetNumber() != dest.GetNumber())
		return false;

	return IsVerticalClear(board, next, dest);
}

bool IsHorizontalClear(Board board, const Square source, const Square dest)
{
	if (source.GetLetter() == dest.GetLetter())
		return true;

	int horizontalPolarity = dest.GetLetter() - source.GetLetter() > 0 ? 1 : -1;
	Square next(source.GetLetter() + horizontalPolarity, source.GetNumber() );

	if (nullptr != board.GetPiece(next) && next.GetNumber() != dest.GetNumber())
		return false;

	return IsHorizontalClear(board, next, dest);
}

bool IsDiagonalClear(Board board, const Square source, const Square dest)
{
	if (source.GetLetter() == dest.GetLetter()) // we already checked the path is diagonal, hence enough to check only one point
		return true;

	int letterDirectionPolarity = dest.GetLetter() - source.GetLetter() > 0 ? 1 : -1;
	int numberDirectionPolarity = dest.GetNumber() - source.GetNumber() > 0 ? 1 : -1;

	Square next(source.GetLetter() + letterDirectionPolarity, source.GetNumber() + numberDirectionPolarity);
	if (nullptr != board.GetPiece(next) && next.GetNumber() != dest.GetNumber())
		return false;

	return IsDiagonalClear(board, next, dest);
}

bool ArePiecesInWay(Board board, const Square source, const Square dest)
{
	bool retVal = true;
	if (IsDiagonalPath(source, dest))
		retVal = IsDiagonalClear(board, source, dest);

	else if (IsVerticalPath(source, dest))
		retVal = IsVerticalClear(board, source, dest);

	else if (IsHorizontalPath(source, dest))
		retVal = IsHorizontalClear(board, source, dest);

	return retVal;
}

bool Pawn::MakeMove(Board board, const Square source, const Square dest)
{
	bool retVal = false;
	// allow one straight move if not blocked.
	if ((((Color::eWhite == m_color) &&
		(dest.GetNumber() == (source.GetNumber() + 1)) &&
		(nullptr == board.GetPiece(dest)))
		|| ((Color::eBlack == m_color) &&
		(dest.GetNumber() == (source.GetNumber() - 1)) &&
			(nullptr == board.GetPiece(dest))))
		&& source.GetLetter() == dest.GetLetter())
		retVal = true;
	// allow capture if piece exists on destination.
	if (((Color::eWhite == m_color) &&
		(dest.GetLetter() == (source.GetLetter() + 1) || (dest.GetLetter() == (source.GetLetter() - 1))) &&
		(dest.GetNumber() == (source.GetNumber() + 1)) &&
		(nullptr != board.GetPiece(dest)) &&
		eBlack == board.GetPiece(dest)->m_color)
		|| ((Color::eBlack == m_color) &&
		(dest.GetLetter() == (source.GetLetter() + 1) || (dest.GetLetter() == (source.GetLetter() - 1))) &&
		(dest.GetNumber() == (source.GetNumber() - 1)) &&
		(nullptr != board.GetPiece(dest)) &&
		eWhite == board.GetPiece(dest)->m_color))
		retVal = true;
	// allow two steps from start if not blocked.
	if ( (Color::eWhite == m_color &&
		Two == source.GetNumber() &&
		Four == dest.GetNumber() && 
		source.GetLetter() == dest.GetLetter() &&
		nullptr == board.GetPiece(dest)))
		retVal = true;
	else if ( (Color::eBlack == m_color &&
		Seven == source.GetNumber() && 
		Five == dest.GetNumber() &&
		source.GetLetter() == dest.GetLetter() &&
		nullptr == board.GetPiece(dest)) )
		retVal = true;

	return retVal;
}

bool Bishop::MakeMove(Board board, const Square source, const Square dest)
{
	bool retVal = false;
	// allow only if on diagonal and no pieces are in the way and if dest if vacant or oponent's piece

	// check if diagonal
	if (IsDiagonalPath(source, dest))
		retVal = true;
	// check if dest is vacant or openent's piece
	if (retVal == true && nullptr != board.GetPiece(dest) && board.GetPiece(dest)->m_color == m_color)
		retVal = false;
	// check if pieces on the way
	if (retVal == true && !IsDiagonalClear(board, source, dest))
		retVal = false;

	return retVal;
}

bool Knight::MakeMove(Board board, const Square source, const Square dest)
{
	bool retVal = false;
	// check the 8 possible knight moves 
	if ((dest.GetLetter() - source.GetLetter() == 1 && dest.GetNumber() - source.GetNumber() == 2) ||
		(dest.GetLetter() - source.GetLetter() == 2 && dest.GetNumber() - source.GetNumber() == 1) ||
		(dest.GetLetter() - source.GetLetter() == 2 && dest.GetNumber() - source.GetNumber() == -1) ||
		(dest.GetLetter() - source.GetLetter() == 1 && dest.GetNumber() - source.GetNumber() == -2) ||
		(dest.GetLetter() - source.GetLetter() == -1 && dest.GetNumber() - source.GetNumber() == -2) ||
		(dest.GetLetter() - source.GetLetter() == -2 && dest.GetNumber() - source.GetNumber() == -1) ||
		(dest.GetLetter() - source.GetLetter() == -2 && dest.GetNumber() - source.GetNumber() == 1) ||
		(dest.GetLetter() - source.GetLetter() == -1 && dest.GetNumber() - source.GetNumber() == 2))
		retVal = true;
	// don't allow if same color's is at dest
	if (nullptr != board.GetPiece(dest) && m_color == board.GetPiece(dest)->m_color)
		retVal = false;

	return retVal;
}

bool Rook::MakeMove(Board board, const Square source, const Square dest)
{
	bool retVal = false;
	// Check if straight path
	if (IsHorizontalPath(source, dest) || IsVerticalPath(source, dest))
		retVal = true;
	// check if dest is empty or oponent
	if (retVal && nullptr != board.GetPiece(dest) && m_color == board.GetPiece(dest)->m_color)
		retVal = false;
	// check if pieces in the way
	if (retVal && !ArePiecesInWay(board, source, dest))
		retVal = false;

	return retVal;
}

bool Queen::MakeMove(Board board, const Square source, const Square dest)
{
	bool retVal = false;
	// Check if straight or diagonal path
	if (IsHorizontalPath(source, dest) || IsVerticalPath(source, dest) || IsDiagonalPath(source, dest))
		retVal = true;
	// check if dest is empty or oponent
	Piece* destPiece = board.GetPiece(dest);
	if (retVal && nullptr != destPiece && m_color == destPiece->m_color)
		retVal = false;
	// check if pieces in the way
	if (retVal && !ArePiecesInWay(board, source, dest))
		retVal = false;

	return retVal;
}

bool King::MakeMove(Board board, const Square source, const Square dest)
{
	bool retVal = false;

	// check if 1 step away
	int letterDiff = abs(dest.GetLetter() - source.GetLetter());
	int numberDiff = abs(dest.GetNumber() - source.GetNumber());
	if ( letterDiff <= 1 &&
		 numberDiff <= 1 &&
		 letterDiff + numberDiff > 0)
		retVal = true;

	// check if dest is vacant or oponent
	if (retVal && nullptr != board.GetPiece(dest) && m_color == board.GetPiece(dest)->m_color)
		retVal = false;

	//allow castling - check if moved. check if rook moved. check if dest is left or right. check path clear
	
	if (!retVal && m_bCastleAllowed)
	{
		Piece* piece = nullptr;
		int rookNumber = dest.GetLetter() == G ? H : A;
		piece = board.GetPiece({ dest.GetLetter(), rookNumber });
		Rook* rook = dynamic_cast<Rook*>(piece);
		if (rook)
		{
			if (rook->m_bCastleAllowed && IsHorizontalClear(board, source, { dest.GetLetter(), rookNumber }))
				retVal = true;
		}
	}
		

	return retVal;
}

void King::OnPieceMoved()
{
	m_bCastleAllowed = false;
}

void Rook::OnPieceMoved()
{
	m_bCastleAllowed = false;
}

std::vector<Piece*> Pawn::CanPieceCapture(Board board, const Square source)
{
	Square leftDestination;
	Square rightDestination;
	std::vector<Piece*> retVal;

	if (eWhite == m_color)
	{
		leftDestination = { source.GetLetter() - 1, source.GetNumber() + 1 };
		rightDestination = { source.GetLetter() + 1, source.GetNumber() + 1 };
	}
	else
	{
		leftDestination = { source.GetLetter() - 1, source.GetNumber() - 1 };
		rightDestination = { source.GetLetter() + 1, source.GetNumber() - 1 };
	}

	Piece* leftPiece = board.GetPiece(leftDestination);
	Piece* rightPiece = board.GetPiece(rightDestination);

	if (leftPiece && leftPiece->m_color != m_color)
		retVal.push_back(leftPiece);
	if (rightPiece && rightPiece->m_color != m_color)
		retVal.push_back(rightPiece);
		
	return retVal;
}

std::vector<Piece*> Bishop::CanPieceCapture(Board board, const Square source)
{
	std::vector<Piece*> retVal = DiagonalsCaptures(board, source, m_color? eWhite : eBlack);
	return retVal;
}

std::vector<Piece*> Knight::CanPieceCapture(Board board, const Square source)
{
	std::vector<Piece*> retVal;
	Square square1 = { source.GetLetter() + 1, source.GetNumber() + 2 };
	Square square2 = { source.GetLetter() + 1, source.GetNumber() - 2 };
	Square square3 = { source.GetLetter() + 2, source.GetNumber() + 1 };
	Square square4 = { source.GetLetter() + 2, source.GetNumber() - 1 };
	Square square5 = { source.GetLetter() - 1, source.GetNumber() + 2 };
	Square square6 = { source.GetLetter() - 1, source.GetNumber() - 2 };
	Square square7 = { source.GetLetter() - 2, source.GetNumber() + 1 };
	Square square8 = { source.GetLetter() - 2, source.GetNumber() - 1 };
	std::vector<Square> dests;
	dests.push_back(square1);
	dests.push_back(square2);
	dests.push_back(square3);
	dests.push_back(square4);
	dests.push_back(square5);
	dests.push_back(square6);
	dests.push_back(square7);
	dests.push_back(square8);

	for (auto dest : dests)
	{
		Piece* destPiece = board.GetPiece(dest);
		if (destPiece && destPiece->m_color != m_color)
			retVal.push_back(destPiece);
	}

	return retVal;
}

std::vector<Piece*> Rook::CanPieceCapture(Board board, const Square source)
{
	std::vector<Piece*> retVal = StraightCaptures(board, source, m_color ? eWhite : eBlack);
	return retVal;
}

std::vector<Piece*> Queen::CanPieceCapture(Board board, const Square source)
{
	std::vector<Piece*> retVal = StraightCaptures(board, source, m_color ? eWhite : eBlack);
	std::vector<Piece*> retVal2 = DiagonalsCaptures(board, source, m_color ? eWhite : eBlack);

	retVal.insert(retVal.end(), retVal2.begin(), retVal2.end());

	return retVal;
}

std::vector<Piece*> King::CanPieceCapture(Board board, const Square source)
{
	std::vector<Piece*> retVal;
	return retVal;
}