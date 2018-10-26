//#include "stdafx.h"
#include "Pieces.h"
#include <stdlib.h>

std::vector<Move> Piece::GetLegalMoves(const Board& in_board, const Square origin)
{
	std::vector<Move> retVal = GetLegalMovesSelf(in_board, origin);
	in_board.RemoveUndefendedCheckMoves(retVal);

	return retVal;
}

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

bool IsVerticalClear(const Board &board, const Square source, const Square dest)
{
	if (source.GetRank() == dest.GetRank())
		return true;

	int verticalPolarity = dest.GetRank() - source.GetRank() > 0 ? 1 : -1;
	Square next(source.GetFile(), source.GetRank() + verticalPolarity);

	if (nullptr != board.GetPiece(next) && next.GetRank() != dest.GetRank())
		return false;

	return IsVerticalClear(board, next, dest);
}

bool IsHorizontalClear(const Board &board, const Square source, const Square dest)
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


void GetLegalLineOfMoves(const Board& board, const Square origin, const Square current, std::vector<Move> &out_moves, const int fileIncrement, const int rankIncrement)
{
	//stop if captured
	if (nullptr != board.GetPiece(current) && board.GetPiece(current)->m_color != board.GetPiece(origin)->m_color)
		return;

	Square nextSquare(current.GetFile() + fileIncrement, current.GetRank() + rankIncrement);
	Piece* nextPiece = board.GetPiece(nextSquare);
	if (!nextSquare.InBounds() ||
		(nullptr != nextPiece && nextPiece->m_color == board.GetPiece(origin)->m_color))
		return;

	out_moves.push_back({ origin, nextSquare });

	return GetLegalLineOfMoves(board, origin, nextSquare, out_moves, fileIncrement, rankIncrement);
}

void GetLegalDiagonalMoves(const Board& board, const Square origin, std::vector<Move> &out_moves)
{
	GetLegalLineOfMoves(board, origin, origin, out_moves, 1, 1);
	GetLegalLineOfMoves(board, origin, origin, out_moves, 1, -1);
	GetLegalLineOfMoves(board, origin, origin, out_moves, -1, 1);
	GetLegalLineOfMoves(board, origin, origin, out_moves, -1, -1);
}

void GetLegalVerticalMoves(const Board& board, const Square origin, std::vector<Move> &out_moves)
{
	GetLegalLineOfMoves(board, origin, origin, out_moves, 0, 1);
	GetLegalLineOfMoves(board, origin, origin, out_moves, 0, -1);
}

void GetLegalHorizontalMoves(const Board& board, const Square origin, std::vector<Move> &out_moves)
{
	GetLegalLineOfMoves(board, origin, origin, out_moves, 1, 0);
	GetLegalLineOfMoves(board, origin, origin, out_moves, -1, 0);
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

bool Pawn::IsMoveLegal(Board &board, const Square source, const Square dest)
{
	bool retVal = false;
	//m_enpassantCaptured = false;
	// allow en passant capture
	Square enpassantSquare = board.GetEnPassantSquare();
	if (enpassantSquare.GetFile() != kIllegalSquare &&
		((Color::eWhite == m_color) &&
		(enpassantSquare.GetFile() == (source.GetFile() + 1) || (enpassantSquare.GetFile() == (source.GetFile() - 1))) &&
			(enpassantSquare.GetRank() == (source.GetRank() + 1)))
		|| ((Color::eBlack == m_color) &&
		(enpassantSquare.GetFile() == (source.GetFile() + 1) || (enpassantSquare.GetFile() == (source.GetFile() - 1))) &&
			(enpassantSquare.GetRank() == (source.GetRank() - 1))))
	{
		retVal = true;
		//m_enpassantCaptured = true;
	}
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
	if ((Color::eWhite == m_color &&
		Two == source.GetRank() &&
		Four == dest.GetRank() &&
		source.GetFile() == dest.GetFile() &&
		nullptr == board.GetPiece(dest)))
	{
		retVal = true;
		board.SetEnPassantSquare({ dest.GetFile(), dest.GetRank() - 1 });
	}
	else if ((Color::eBlack == m_color &&
		Seven == source.GetRank() &&
		Five == dest.GetRank() &&
		source.GetFile() == dest.GetFile() &&
		nullptr == board.GetPiece(dest)))
	{
		retVal = true;
		board.SetEnPassantSquare({ dest.GetFile(), dest.GetRank() + 1 });
	}

	if (retVal && (dest.GetRank() == Eight || dest.GetRank() == One))
		board.SetQueeningMode(true);

	return retVal;
}

bool Bishop::IsMoveLegal(Board &board, const Square source, const Square dest)
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

bool Knight::IsMoveLegal(Board &board, const Square source, const Square dest)
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

bool Rook::IsMoveLegal(Board &board, const Square source, const Square dest)
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

bool Queen::IsMoveLegal(Board &board, const Square source, const Square dest)
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

bool King::IsMoveLegal(Board &board, const Square source, const Square dest)
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
	auto castlingFlag = board.GetCastlingFlag();
	Piece * isWhiteShort = board.GetPiece({ G, One });
	Piece * isWhiteLong = board.GetPiece({ C, One });
	Piece * isBlackShort = board.GetPiece({ G, Eight });
	Piece * isBlackLong = board.GetPiece({ C, Eight });
	Square castleRookSquare;
	if (eWhite == m_color && castlingFlag[whiteShort] == true && isWhiteShort && "K" == isWhiteShort->m_name) //white short
		castleRookSquare = { H, One };
	else if (eWhite == m_color && castlingFlag[whiteLong] == true && isWhiteLong && "K" == isWhiteLong->m_name) //white long
		castleRookSquare = { A, One };
	else if (eBlack == m_color && castlingFlag[blackShort] == true && isBlackShort && "K" == isBlackShort->m_name) //black short
		castleRookSquare = { H, Eight };
	else if (eBlack == m_color && castlingFlag[blackLong] == true && isBlackLong && "K" == isBlackLong->m_name) //black long
		castleRookSquare = { A, Eight };

	if (kIllegalSquare != castleRookSquare.GetFile())
	{
		Piece *piece = board.GetPiece(castleRookSquare);
		board.SetPiece(castleRookSquare, nullptr);
		castleRookSquare.SetFile(castleRookSquare.GetFile() == H ? F : D);
		board.SetPiece(castleRookSquare, piece);
	}
}

void Rook::OnPieceMoved(Board &/*board*/)
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

std::vector<Move> Pawn::GetLegalMovesSelf(const Board& in_board, const Square origin)
{
	std::vector<Move> retVal;
	// allow en passant capture
	Square enpassantSquare = in_board.GetEnPassantSquare();
	if (enpassantSquare.GetFile() != kIllegalSquare &&
		((Color::eWhite == m_color) &&
		(enpassantSquare.GetFile() == (origin.GetFile() + 1) || (enpassantSquare.GetFile() == (origin.GetFile() - 1))) &&
			(enpassantSquare.GetRank() == (origin.GetRank() + 1)))
		|| ((Color::eBlack == m_color) &&
		(enpassantSquare.GetFile() == (origin.GetFile() + 1) || (enpassantSquare.GetFile() == (origin.GetFile() - 1))) &&
			(enpassantSquare.GetRank() == (origin.GetRank() - 1))))
	{
		retVal.push_back({ origin, enpassantSquare });
	}
	// allow one straight move if not blocked.
	Square straightMoveSquareWhite({ origin.GetFile(), origin.GetRank() + 1 });
	Square straightMoveSquareBlack({ origin.GetFile(), origin.GetRank() -1 });
	if (Color::eWhite == m_color && nullptr == in_board.GetPiece(straightMoveSquareWhite))
		retVal.push_back({ origin, straightMoveSquareWhite });
	else if (Color::eBlack == m_color && nullptr == in_board.GetPiece(straightMoveSquareBlack))
		retVal.push_back({ origin, straightMoveSquareBlack });

	Square leftCaptureWhite(origin.GetFile() - 1, origin.GetRank() + 1);
	Square rightCaptureWhite(origin.GetFile() + 1, origin.GetRank() + 1);
	Square leftCaptureBlack(origin.GetFile() - 1, origin.GetRank() - 1);
	Square rightCaptureBlack(origin.GetFile() + 1, origin.GetRank() - 1);

	// allow capture if piece exists on destination.
	if (eWhite == m_color && nullptr != in_board.GetPiece(leftCaptureWhite) &&
		eBlack == in_board.GetPiece(leftCaptureWhite)->m_color)
		retVal.push_back({ origin, leftCaptureWhite });
	if (eWhite == m_color && nullptr != in_board.GetPiece(rightCaptureWhite) &&
		eBlack == in_board.GetPiece(rightCaptureWhite)->m_color)
		retVal.push_back({ origin, rightCaptureWhite });
	if (eBlack == m_color && nullptr != in_board.GetPiece(leftCaptureBlack) &&
		eWhite == in_board.GetPiece(leftCaptureBlack)->m_color)
		retVal.push_back({ origin, leftCaptureBlack });
	if (eBlack == m_color && nullptr != in_board.GetPiece(rightCaptureBlack) &&
		eWhite == in_board.GetPiece(rightCaptureBlack)->m_color)
		retVal.push_back({ origin, rightCaptureBlack });

	Square twoStepsWhite({ origin.GetFile(), origin.GetRank() + 2 });
	Square twoStepsBlack({ origin.GetFile(), origin.GetRank() - 2 });
	// allow two steps from start if not blocked.
	if (Color::eWhite == m_color &&	Two == origin.GetRank() && 
		IsVerticalClear(in_board, origin, twoStepsWhite) && nullptr == in_board.GetPiece(twoStepsWhite))
	{
		retVal.push_back({ origin, twoStepsWhite });
	}
	else if (Color::eBlack == m_color && Seven == origin.GetRank() &&
		IsVerticalClear(in_board, origin, twoStepsBlack) && nullptr == in_board.GetPiece(twoStepsBlack))
	{
		retVal.push_back({ origin, twoStepsBlack });
	}

	if (in_board.GetCheckOrMate() == eCheck)
		in_board.RemoveUndefendedCheckMoves(retVal);
	//if (retVal && (dest.GetRank() == Eight || dest.GetRank() == One))
	//	board.SetQueeningMode(true); //TODO - Add queening options to legal moves
	return retVal;
}

std::vector<Move> Bishop::GetLegalMovesSelf(const Board& in_board, const Square origin)
{
	std::vector<Move> retVal;

	GetLegalDiagonalMoves(in_board, origin, retVal);

	return retVal;
}

std::vector<Move> Knight::GetLegalMovesSelf(const Board& in_board, const Square origin)
{
	std::vector<Move> retVal;

	std::vector<Square> options;
	options.push_back({ origin.GetFile() + 1, origin.GetRank() + 2 });
	options.push_back({ origin.GetFile() + 2, origin.GetRank() + 1 });
	options.push_back({ origin.GetFile() + 1, origin.GetRank() - 2 });
	options.push_back({ origin.GetFile() + 2, origin.GetRank() - 1 });
	options.push_back({ origin.GetFile() - 1, origin.GetRank() + 2 });
	options.push_back({ origin.GetFile() - 2, origin.GetRank() + 1 });
	options.push_back({ origin.GetFile() - 1, origin.GetRank() - 2 });
	options.push_back({ origin.GetFile() - 2, origin.GetRank() - 1 });

	for (auto option : options)
	{
		Piece* destPiece = in_board.GetPiece(option);
		if (!option.InBounds() || (nullptr != destPiece && destPiece->m_color == in_board.GetPiece(origin)->m_color))
			continue;
		retVal.push_back({ origin, option });
	}

	return retVal;
}

std::vector<Move> Rook::GetLegalMovesSelf(const Board& in_board, const Square origin)
{
	std::vector<Move> retVal;

	GetLegalHorizontalMoves(in_board, origin, retVal);
	GetLegalVerticalMoves(in_board, origin, retVal);

	return retVal;
}

std::vector<Move> Queen::GetLegalMovesSelf(const Board& in_board, const Square origin)
{
	std::vector<Move> retVal;

	GetLegalHorizontalMoves(in_board, origin, retVal);
	GetLegalVerticalMoves(in_board, origin, retVal);
	GetLegalDiagonalMoves(in_board, origin, retVal);

	return retVal;
}


std::vector<Move> King::GetLegalMovesSelf(const Board& in_board, const Square origin)
{
	std::vector<Move> retVal;
	std::vector<Square> options;

	Piece *myPiece = in_board.GetPiece(origin);
	Color myColor = myPiece->m_color;
	auto castlingFlag = in_board.GetCastlingFlag();

	options.push_back({ origin.GetFile() + 1, origin.GetRank() + 1});
	options.push_back({ origin.GetFile() + 1, origin.GetRank() });
	options.push_back({ origin.GetFile() + 1, origin.GetRank() - 1});
	options.push_back({ origin.GetFile(), origin.GetRank() + 1});
	options.push_back({ origin.GetFile(), origin.GetRank() - 1});
	options.push_back({ origin.GetFile() - 1, origin.GetRank() + 1});
	options.push_back({ origin.GetFile() - 1, origin.GetRank() });
	options.push_back({ origin.GetFile() - 1, origin.GetRank() - 1});
	if (myColor == eWhite && castlingFlag[whiteShort] && IsHorizontalClear(in_board, origin, { H, One }))
		options.push_back({ origin.GetFile() + 2, origin.GetRank() }); //short white castle
	if (myColor == eBlack && castlingFlag[blackShort] && IsHorizontalClear(in_board, origin, { H, Eight }))
		options.push_back({ origin.GetFile() + 2, origin.GetRank() }); //short black castle
	if (myColor == eWhite && castlingFlag[whiteLong] && IsHorizontalClear(in_board, origin, { A, One }))
		options.push_back({ origin.GetFile() - 2, origin.GetRank() }); //long white castle
	if (myColor == eBlack && castlingFlag[blackLong] && IsHorizontalClear(in_board, origin, { A, Eight }))
		options.push_back({ origin.GetFile() - 2, origin.GetRank() }); //long black castle

	for (auto option = options.begin(); option != options.end();)
	{
		Piece* destPiece(in_board.GetPiece(*option));
		if (!option->InBounds() || (nullptr != destPiece && destPiece->m_color == myColor))
		{
			option = options.erase(option);
			continue;
		}
		Board dummyBoard = in_board;
		Square currOption = *option;
		dummyBoard.SetPiece(currOption, myPiece);
		dummyBoard.SetPiece(origin, nullptr);
		if (dummyBoard.CheckIsCheck())
		{
			option = options.erase(option);
			continue;
		}
		retVal.push_back({ origin, *option });
		++option;
	}


	return retVal;
}

std::vector<Piece*> Queen::CanPieceCapture(Board &board, const Square source)
{
	std::vector<Piece*> retVal = StraightCaptures(board, source, m_color ? eWhite : eBlack);
	std::vector<Piece*> retVal2 = DiagonalsCaptures(board, source, m_color ? eWhite : eBlack);

	retVal.insert(retVal.end(), retVal2.begin(), retVal2.end());

	return retVal;
}

std::vector<Piece*> King::CanPieceCapture(Board &/*board*/, const Square source)
{
	std::vector<Piece*> retVal;
	return retVal;
}

bool Pawn::isEnPassantMove(const Color in_color, const Square in_source, const Square in_dest)
{
	bool retVal = false;
	if (in_color == eWhite && in_source.GetRank() == Two && in_dest.GetRank() == Four)
		retVal = true;
	else if (in_color == eBlack && in_source.GetRank() == Seven && in_dest.GetRank() == Five)
		retVal = true;

	return retVal;
}

void Pawn::OnPieceMoved(Board &board)
{
	Square enPassantSquare = board.GetEnPassantSquare();
	if (enPassantSquare.GetFile() != kIllegalSquare && nullptr != board.GetPiece(enPassantSquare))
	{
		Square removeEatenPawn = enPassantSquare;
		removeEatenPawn.SetRank(enPassantSquare.GetRank() == Six ? Five : Four);
		board.SetPiece(removeEatenPawn, nullptr);
	}
}