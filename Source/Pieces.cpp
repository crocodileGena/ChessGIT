//#include "stdafx.h"
#include "Pieces.h"
#include <stdlib.h>

void RecursiveCheckCaptures(Board &board, Square source, int xDirection, int yDirection, std::vector<Piece*>& outCaptures, Color canCaptureColor)
{
	Square dest({ source.GetFile() + xDirection, source.GetRank() + yDirection });
	if (dest.GetFile() > BoardSize - 1 || dest.GetFile() < 0 ||
		dest.GetRank() > BoardSize - 1 || dest.GetRank() < 0)
		return;
	Piece* destPiece = board.GetPiece(dest);
	if (destPiece && destPiece->m_color == canCaptureColor)
	{
		outCaptures.push_back(destPiece);
		return;
	}
	RecursiveCheckCaptures(board, dest, xDirection, yDirection, outCaptures, canCaptureColor);

	
}

std::vector<Piece*> DiagonalsCaptures(Board &board, Square source, Color canCaptureColor)
{
	std::vector<Piece*> retVal;
	RecursiveCheckCaptures(board, source, 1, 1, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, 1, -1, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, -1, 1, retVal, canCaptureColor);
	RecursiveCheckCaptures(board, source, -1, -1, retVal, canCaptureColor);
	return retVal;
}

std::vector<Piece*> StraightCaptures(Board &board, Square source, Color canCaptureColor)
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
	return (source.GetFile() - dest.GetFile() == source.GetRank() - dest.GetRank() ||
			source.GetFile() - dest.GetFile() == dest.GetRank() - source.GetRank());
}

bool IsVerticalPath(const Square source, const Square dest)
{
	return source.GetFile() == dest.GetFile();
}

bool IsHorizontalPath(const Square source, const Square dest)
{
	return source.GetRank() == dest.GetRank();
}

bool IsVerticalClear(Board &board, const Square source, const Square dest)
{
	if (source.GetRank() == dest.GetRank())
		return true;

	int verticalPolarity = dest.GetRank() - source.GetRank() > 0 ? 1 : -1;
	Square next(source.GetFile(), source.GetRank() + verticalPolarity);

	if (nullptr != board.GetPiece(next) && next.GetRank() != dest.GetRank())
		return false;

	return IsVerticalClear(board, next, dest);
}

bool IsHorizontalClear(Board &board, const Square source, const Square dest)
{
	if (source.GetFile() == dest.GetFile())
		return true;

	int horizontalPolarity = dest.GetFile() - source.GetFile() > 0 ? 1 : -1;
	Square next(source.GetFile() + horizontalPolarity, source.GetRank() );

	if (nullptr != board.GetPiece(next) && next.GetFile() != dest.GetFile())
		return false;

	return IsHorizontalClear(board, next, dest);
}

bool IsDiagonalClear(Board &board, const Square source, const Square dest)
{
	if (source.GetFile() == dest.GetFile()) // we already checked the path is diagonal, hence enough to check only one point
		return true;

	int fileDirectionPolarity = dest.GetFile() - source.GetFile() > 0 ? 1 : -1;
	int rankDirectionPolarity = dest.GetRank() - source.GetRank() > 0 ? 1 : -1;

	Square next(source.GetFile() + fileDirectionPolarity, source.GetRank() + rankDirectionPolarity);
	if (nullptr != board.GetPiece(next) && next.GetRank() != dest.GetRank())
		return false;

	return IsDiagonalClear(board, next, dest);
}

bool ArePiecesInWay(Board &board, const Square source, const Square dest)
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

bool Pawn::MakeMove(Board &board, const Square source, const Square dest)
{
	bool retVal = false;
	// allow one straight move if not blocked.
	if ((((Color::eWhite == m_color) &&
		(dest.GetRank() == (source.GetRank() + 1)) &&
		(nullptr == board.GetPiece(dest)))
		|| ((Color::eBlack == m_color) &&
		(dest.GetRank() == (source.GetRank() - 1)) &&
			(nullptr == board.GetPiece(dest))))
		&& source.GetFile() == dest.GetFile())
		retVal = true;
	// allow capture if piece exists on destination.
	if (((Color::eWhite == m_color) &&
		(dest.GetFile() == (source.GetFile() + 1) || (dest.GetFile() == (source.GetFile() - 1))) &&
		(dest.GetRank() == (source.GetRank() + 1)) &&
		(nullptr != board.GetPiece(dest)) &&
		eBlack == board.GetPiece(dest)->m_color)
		|| ((Color::eBlack == m_color) &&
		(dest.GetFile() == (source.GetFile() + 1) || (dest.GetFile() == (source.GetFile() - 1))) &&
		(dest.GetRank() == (source.GetRank() - 1)) &&
		(nullptr != board.GetPiece(dest)) &&
		eWhite == board.GetPiece(dest)->m_color))
		retVal = true;
	// allow two steps from start if not blocked.
	if ( (Color::eWhite == m_color &&
		Two == source.GetRank() &&
		Four == dest.GetRank() && 
		source.GetFile() == dest.GetFile() &&
		nullptr == board.GetPiece(dest)))
		retVal = true;
	else if ( (Color::eBlack == m_color &&
		Seven == source.GetRank() && 
		Five == dest.GetRank() &&
		source.GetFile() == dest.GetFile() &&
		nullptr == board.GetPiece(dest)) )
		retVal = true;

	if (retVal && (dest.GetRank() == Eight || dest.GetRank() == One))
		board.SetQueeningMode(true);

	return retVal;
}

bool Bishop::MakeMove(Board &board, const Square source, const Square dest)
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

bool Knight::MakeMove(Board &board, const Square source, const Square dest)
{
	bool retVal = false;
	// check the 8 possible knight moves 
	if ((dest.GetFile() - source.GetFile() == 1 && dest.GetRank() - source.GetRank() == 2) ||
		(dest.GetFile() - source.GetFile() == 2 && dest.GetRank() - source.GetRank() == 1) ||
		(dest.GetFile() - source.GetFile() == 2 && dest.GetRank() - source.GetRank() == -1) ||
		(dest.GetFile() - source.GetFile() == 1 && dest.GetRank() - source.GetRank() == -2) ||
		(dest.GetFile() - source.GetFile() == -1 && dest.GetRank() - source.GetRank() == -2) ||
		(dest.GetFile() - source.GetFile() == -2 && dest.GetRank() - source.GetRank() == -1) ||
		(dest.GetFile() - source.GetFile() == -2 && dest.GetRank() - source.GetRank() == 1) ||
		(dest.GetFile() - source.GetFile() == -1 && dest.GetRank() - source.GetRank() == 2))
		retVal = true;
	// don't allow if same color's is at dest
	if (nullptr != board.GetPiece(dest) && m_color == board.GetPiece(dest)->m_color)
		retVal = false;

	return retVal;
}

bool Rook::MakeMove(Board &board, const Square source, const Square dest)
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

bool Queen::MakeMove(Board &board, const Square source, const Square dest)
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

bool King::MakeMove(Board &board, const Square source, const Square dest)
{
	bool retVal = false;
	m_castleRookSquare = { kIllegalSquare, kIllegalSquare };
	// check if 1 step away
	int fileDiff = abs(dest.GetFile() - source.GetFile());
	int rankDiff = abs(dest.GetRank() - source.GetRank());
	if ( fileDiff <= 1 &&
		 rankDiff <= 1 &&
		 fileDiff + rankDiff > 0)
		retVal = true;

	// check if dest is vacant or oponent
	if (retVal && nullptr != board.GetPiece(dest) && m_color == board.GetPiece(dest)->m_color)
		retVal = false;

	//allow castling - check if moved. check if rook moved. check if dest is left or right. check path clear
	
	if (!retVal && m_bCastleAllowed)
	{
		Piece* piece = nullptr;
		int rookFile = dest.GetFile() == G ? H : A;
		piece = board.GetPiece({ rookFile, dest.GetRank() });
		Rook* rook = dynamic_cast<Rook*>(piece);
		if (rook)
		{
			if (rook->m_bCastleAllowed && IsHorizontalClear(board, source, { rookFile, dest.GetRank() }))
			{
				m_castleRookSquare = { rookFile, dest.GetRank() };
				retVal = true;
			}
		}
	}
		

	return retVal;
}

void King::OnPieceMoved(Board &board)
{
	m_bCastleAllowed = false;
	Piece *piece = board.GetPiece(m_castleRookSquare);
	board.SetPiece(m_castleRookSquare, nullptr);
	m_castleRookSquare.SetFile(m_castleRookSquare.GetFile() == H ? F : D);
	board.SetPiece(m_castleRookSquare, piece);
	m_castleRookSquare = { kIllegalSquare, kIllegalSquare };
}

void Rook::OnPieceMoved(Board &board)
{
	m_bCastleAllowed = false;
}

std::vector<Piece*> Pawn::CanPieceCapture(Board &board, const Square source)
{
	Square leftDestination;
	Square rightDestination;
	std::vector<Piece*> retVal;

	if (eWhite == m_color)
	{
		leftDestination = { source.GetFile() - 1, source.GetRank() + 1 };
		rightDestination = { source.GetFile() + 1, source.GetRank() + 1 };
	}
	else
	{
		leftDestination = { source.GetFile() - 1, source.GetRank() - 1 };
		rightDestination = { source.GetFile() + 1, source.GetRank() - 1 };
	}

	Piece* leftPiece = board.GetPiece(leftDestination);
	Piece* rightPiece = board.GetPiece(rightDestination);

	if (leftPiece && leftPiece->m_color != m_color)
		retVal.push_back(leftPiece);
	if (rightPiece && rightPiece->m_color != m_color)
		retVal.push_back(rightPiece);
		
	return retVal;
}

std::vector<Piece*> Bishop::CanPieceCapture(Board &board, const Square source)
{
	std::vector<Piece*> retVal = DiagonalsCaptures(board, source, m_color? eWhite : eBlack);
	return retVal;
}

std::vector<Piece*> Knight::CanPieceCapture(Board &board, const Square source)
{
	std::vector<Piece*> retVal;
	Square square1 = { source.GetFile() + 1, source.GetRank() + 2 };
	Square square2 = { source.GetFile() + 1, source.GetRank() - 2 };
	Square square3 = { source.GetFile() + 2, source.GetRank() + 1 };
	Square square4 = { source.GetFile() + 2, source.GetRank() - 1 };
	Square square5 = { source.GetFile() - 1, source.GetRank() + 2 };
	Square square6 = { source.GetFile() - 1, source.GetRank() - 2 };
	Square square7 = { source.GetFile() - 2, source.GetRank() + 1 };
	Square square8 = { source.GetFile() - 2, source.GetRank() - 1 };
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

std::vector<Piece*> Rook::CanPieceCapture(Board &board, const Square source)
{
	std::vector<Piece*> retVal = StraightCaptures(board, source, m_color ? eWhite : eBlack);
	return retVal;
}

std::vector<Piece*> Queen::CanPieceCapture(Board &board, const Square source)
{
	std::vector<Piece*> retVal = StraightCaptures(board, source, m_color ? eWhite : eBlack);
	std::vector<Piece*> retVal2 = DiagonalsCaptures(board, source, m_color ? eWhite : eBlack);

	retVal.insert(retVal.end(), retVal2.begin(), retVal2.end());

	return retVal;
}

std::vector<Piece*> King::CanPieceCapture(Board &board, const Square source)
{
	std::vector<Piece*> retVal;
	return retVal;
}