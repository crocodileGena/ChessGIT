//#include "stdafx.h"
#include "board.h"
#include <algorithm>
#include <cctype>
#include "Pieces.h"

Board::Board(const Board& in_board) : 
m_lastColorMoved(in_board.m_lastColorMoved),
m_halfmoveClock(in_board.m_halfmoveClock),
m_status(in_board.m_status),
m_queeningMode(in_board.m_queeningMode),
m_enPassantSquare(in_board.m_enPassantSquare),
m_gameNotation(in_board.m_gameNotation),
m_checkOrMate(in_board.m_checkOrMate)
{
	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
			{
				board[i][j] = in_board.board[i][j];
			}

	for (int i = 0; i < numCastlingOptions; ++i)
		m_castlingFlag[i] = in_board.GetCastlingFlag()[i];
}

void Board::PiecesSum(int& whiteSum, int& blackSum)
{
	int temp_blackSum = 0;
	int temp_whiteSum = 0;

	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
		{
			Piece* currPiece = GetPiece({ i,j });
			if (!currPiece)
				continue;
			if (eWhite == currPiece->m_color)
				temp_whiteSum += currPiece->m_worth;
			else
				temp_blackSum += currPiece->m_worth;
		}

	whiteSum = temp_whiteSum;
	blackSum = temp_blackSum;
}

Board::Board() : 
m_lastColorMoved(eBlack), 
m_status("status bar"),
m_halfmoveClock(0),
m_queeningMode(false),
m_checkOrMate(eNone)
{
	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
		{
			SetPiece({ i,j }, nullptr);
		}

	for (int i = 0; i < numCastlingOptions; ++i)
		m_castlingFlag[i] = true;
}
void Board::ResetBoard()
{
	m_lastColorMoved = eBlack;
	m_queeningMode = false;
	m_status = "status bar";
	m_halfmoveClock = 0;
	m_gameNotation.Reset();
	SetCheckOrMate(eNone);

	for (int i = 0; i < numCastlingOptions; ++i)
		m_castlingFlag[i] = true;

	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
		{
			Piece* currPiece = GetPiece({ i,j });
			if (currPiece)
			{
				delete currPiece;
				SetPiece({ i,j }, nullptr);
			}
		}

	board[A][One] = new Rook(Color::eWhite);
	board[A][Two] = new Pawn(Color::eWhite);
	board[B][One] = new Knight(Color::eWhite);
	board[B][Two] = new Pawn(Color::eWhite);
	board[C][One] = new Bishop(Color::eWhite);
	board[C][Two] = new Pawn(Color::eWhite);
	board[D][One] = new Queen(Color::eWhite);
	board[D][Two] = new Pawn(Color::eWhite);
	board[E][One] = new King(Color::eWhite);
	board[E][Two] = new Pawn(Color::eWhite);
	board[F][One] = new Bishop(Color::eWhite);
	board[F][Two] = new Pawn(Color::eWhite);
	board[G][One] = new Knight(Color::eWhite);
	board[G][Two] = new Pawn(Color::eWhite);
	board[H][One] = new Rook(Color::eWhite);
	board[H][Two] = new Pawn(Color::eWhite);

	board[A][Eight] = new Rook(Color::eBlack);
	board[A][Seven] = new Pawn(Color::eBlack);
	board[B][Eight] = new Knight(Color::eBlack);
	board[B][Seven] = new Pawn(Color::eBlack);
	board[C][Eight] = new Bishop(Color::eBlack);
	board[C][Seven] = new Pawn(Color::eBlack);
	board[D][Eight] = new Queen(Color::eBlack);
	board[D][Seven] = new Pawn(Color::eBlack);
	board[E][Eight] = new King(Color::eBlack);
	board[E][Seven] = new Pawn(Color::eBlack);
	board[F][Eight] = new Bishop(Color::eBlack);
	board[F][Seven] = new Pawn(Color::eBlack);
	board[G][Eight] = new Knight(Color::eBlack);
	board[G][Seven] = new Pawn(Color::eBlack);
	board[H][Eight] = new Rook(Color::eBlack);
	board[H][Seven] = new Pawn(Color::eBlack);
}

void Board::PrintBoard()
{
	for (int j = BoardSize - 1; j >= 0; --j)
	{
		for (int i = 0; i < BoardSize; ++i)
		{
			Piece* currPiece = GetPiece({ i, j });
			if (currPiece)
			{
				Color currColor = static_cast<Color>(currPiece->m_color);
				char* colorDisplay = currColor == eWhite ? "W" : "B";
				char* isPawn = currPiece->m_name.empty() ? " " : "";
				std::cout << colorDisplay << isPawn << currPiece->m_name << " ";
			}
			else
				std::cout << "NN ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

}

std::string Board::GetPiecesPosition()
{
	std::string retVal;

	for (int j = BoardSize - 1; j >= 0; --j)
		for (int i = 0; i < BoardSize; ++i)
		{
			Piece* currPiece = GetPiece({ i, j });
			if (!currPiece)
				retVal += "-";
			else
			{
				if (currPiece->m_color == eWhite)
					retVal += currPiece->m_name;
				else
				{
					auto lower = (char)std::tolower(currPiece->m_name[0]);
					retVal += lower;
				}
			}
		}
	return retVal;
}

void Board::UpdateCastlingFlag(const Piece* in_piece, const Square in_origin)
{
	int piece = in_piece->m_worth;
	if (piece != eKing && piece != eRook)
		return;
	if (in_piece->m_color == eWhite && piece == eKing)
	{
		m_castlingFlag[whiteShort] = false;
		m_castlingFlag[whiteLong] = false;
	}
	else if (in_piece->m_color == eWhite && piece == eRook && in_origin.GetFile() == A)
		m_castlingFlag[whiteLong] = false;
	else if (in_piece->m_color == eWhite && piece == eRook && in_origin.GetFile() == H)
		m_castlingFlag[whiteShort] = false;
	else if (in_piece->m_color == eBlack && piece == eKing)
	{
		m_castlingFlag[blackShort] = false;
		m_castlingFlag[blackLong] = false;
	}
	else if (in_piece->m_color == eBlack && piece == eRook && in_origin.GetFile() == A)
		m_castlingFlag[blackLong] = false;
	else if (in_piece->m_color == eBlack && piece == eRook && in_origin.GetFile() == H)
		m_castlingFlag[blackShort] = false;

}

void Board::UpdateHalfmoveClock(const bool isCapture, const bool isPawn)
{
	if (isCapture || isPawn)
		m_halfmoveClock = 0;
	else
		++m_halfmoveClock;
}

void Board::QueenAPawn(const Square in_square, const std::string in_piece)
{
	if (!GetQueeningMode())
		return;
	SetQueeningMode(false);

	Piece* newPiece(nullptr);
	if (in_piece == "R")
		newPiece = new Rook(in_square.GetRank() == Eight ? eWhite : eBlack);
	else if (in_piece == "Q")
		newPiece = new Queen(in_square.GetRank() == Eight ? eWhite : eBlack);
	else if (in_piece == "B")
		newPiece = new Bishop(in_square.GetRank() == Eight ? eWhite : eBlack);
	else if (in_piece == "N")
		newPiece = new Knight(in_square.GetRank() == Eight ? eWhite : eBlack);

	board[in_square.GetFile()][in_square.GetRank()] = newPiece;
}

bool Board::MovePiece(const Square inBase, const Square inDest)
{
	bool isPieceMoved = false;
	auto currPiece = GetPiece(inBase);
	auto checkOrMate = GetCheckOrMate();
	if (!GetQueeningMode() && currPiece && inBase.InBounds() && inDest.InBounds() &&
		checkOrMate != eMate && checkOrMate != eDraw)
	{
		auto legalMoves = currPiece->GetLegalMoves(*this, inBase);
		Move myMove(inBase, inDest);
		if (m_lastColorMoved == currPiece->m_color)
			m_status = "Same color ";
		else if (legalMoves.end() != std::find(legalMoves.begin(), legalMoves.end(), myMove))
		{
			// Prepare parameters
			const Square enPassantDestSquare = GetEnPassantSquare();
			const bool isCapture = GetPiece(inDest) != nullptr || enPassantDestSquare == inDest;
			bool specifyRank = false; //TODO : Implement this
			bool specifyFile = false; //ToDO : Implement this
			const bool whitesMove = currPiece->m_color == eBlack;
			const bool* castlingOptions = GetCastlingFlag();

			auto allLegalMoves = GetLegalMoves(currPiece->m_color);
			for (auto legalMove : allLegalMoves)
			{
				if (legalMove != myMove && legalMove.m_dest == inDest && GetPiece(legalMove.m_origin)->m_name == currPiece->m_name)
				{
					if (legalMove.m_origin.GetFile() != inBase.GetFile())
						specifyFile = true;
					else
						specifyRank = true;
				}
			}

			isPieceMoved = true;
			CommitMove(currPiece, inBase, inDest);

			const std::string piecesPosition = GetPiecesPosition();
			UpdateHalfmoveClock(isCapture, currPiece->m_worth == ePawn);
			SetCheckOrMate(DeriveCheckOrMate(piecesPosition));
			UpdateCastlingFlag(currPiece, inBase);
			m_lastColorMoved = currPiece->m_color;
			m_gameNotation.PushMove(piecesPosition, currPiece->m_name, inBase, inDest,
				isCapture, specifyRank, specifyFile, whitesMove, castlingOptions,
				enPassantDestSquare, m_halfmoveClock, GetCheckOrMate());
			UpdateEnPassantSquare(currPiece, inBase, inDest);
		}
	}
	if (!isPieceMoved)
		std::cout << "Illegal Move\n";

	return isPieceMoved;
}

CheckOrMate Board::DeriveCheckOrMate(const std::string piecesPosition)
{
	CheckOrMate retVal = eNone;

	bool noLegalMoves = false;
	std::vector<Move> legalMoves = GetLegalMoves(m_lastColorMoved);
	if (legalMoves.empty())
		noLegalMoves = true;

	if (CheckIsCheck())
		retVal = noLegalMoves ? eMate : eCheck;

	if (retVal == eNone && IsADraw(noLegalMoves, piecesPosition))
		retVal = eDraw;

	return retVal;
}

bool Board::IsADraw(const bool noLegalMoves, const std::string piecesPosition)
{
	bool retVal = false;

	if (noLegalMoves || m_halfmoveClock > 100 ||
		m_gameNotation.IsPerpetual(piecesPosition, m_lastColorMoved == eBlack))
		retVal = true;
	else
	{
		int whiteSum(0), blackSum(0);
		PiecesSum(whiteSum, blackSum);
		int combined = whiteSum + blackSum;
		if (combined == 0 || combined == 3)
			retVal = true;
		else if (combined == 6)
		{
			//must be two pieces
			std::vector<Piece*> leftPieces = GetPieces();
			std::string firstPiece = leftPieces.at(0)->m_name;
			std::string secondPiece = leftPieces.at(1)->m_name;
			Color firstPieceColor = leftPieces.at(0)->m_color;
			Color secondPieceColor = leftPieces.at(1)->m_color;
			if (firstPiece.compare("B") == std::string::npos && secondPiece.compare("B") == std::string::npos &&
				firstPieceColor == secondPieceColor)
				retVal = true;
		}

	}

	return retVal;
}

std::vector<Piece*> Board::GetPieces()
{
	std::vector<Piece*> retVal;
	for (int i = 0; i < BoardSize; ++i)
	{
		for (int j = 0; j < BoardSize; ++j)
		{
			Piece* currPiece = GetPiece({ i,j });
			retVal.push_back(currPiece);
		}
	}
	return retVal;
}

void Board::CommitMove(Piece * currPiece, const Square &inBase, const Square &inDest)
{
	// Make the move
	SetPiece(inBase, nullptr);
	SetPiece(inDest, currPiece);
	currPiece->OnPieceMoved(*this); // update other affected pieces
}

void Board::UpdateEnPassantSquare(Piece * currPiece, const Square & inBase, const Square & inDest)
{
	Color currColor = currPiece->m_color;
	if (currPiece->isEnPassantMove(currPiece->m_color, inBase, inDest))
		SetEnPassantSquare({ inBase.GetFile(), currColor == eWhite ? inBase.GetRank() + 1 : inBase.GetRank() - 1 });
	else
		SetEnPassantSquare({ kIllegalSquare, kIllegalSquare });
}

std::string GameNotation::GetFENFromPosition(const std::string in_position, const bool whitesMove,
											const bool* castlingOptions, const Square enPassant,
											const int halfmoveClock)
{
	std::string retVal;
	PositionToString(in_position, retVal);
	retVal += whitesMove ? " w " : " b ";
	GetCastlingString(castlingOptions, retVal);
	retVal += " ";
	if (enPassant.GetFile() == kIllegalSquare)
		retVal += '-';
	else
	{
		retVal += char(enPassant.GetFile() + 'a');
		retVal += char(enPassant.GetRank() + '1');
	}
	retVal += " ";
	retVal += std::to_string(halfmoveClock);
	retVal += " ";
	int moveNumber = int(((double)m_vNotation.size() / 2) + 0.6);
	retVal += std::to_string(moveNumber);
	return retVal;
}

void GameNotation::GetCastlingString(const bool* in_options, std::string &retVal)
{
	bool isNone = true;
	if (in_options[whiteShort])
	{
		retVal += "K";
		isNone = false;
	}
	if (in_options[whiteLong])
	{
		retVal += "Q";
		isNone = false;
	}
	if (in_options[blackShort])
	{
		retVal += "k";
		isNone = false;
	}
	if (in_options[blackLong])
	{
		retVal += "q";
		isNone = false;
	}
	if (isNone)
		retVal += "-";

}
void GameNotation::PositionToString(const std::string &in_position, std::string &retVal)
{
	int emptyCounter(0);
	int rawCounter(0);
	for (char c : in_position)
	{
		if (c == '-')
			++emptyCounter;
		else
		{
			if (emptyCounter != 0)
			{
				retVal += std::to_string(emptyCounter);
				emptyCounter = 0;
			}
			retVal += c;
		}

		if (rawCounter++ == H)
		{
			rawCounter = 0;
			if (emptyCounter != 0)
			{
				retVal += std::to_string(emptyCounter);
				emptyCounter = 0;
			}
			retVal += '/';
		}
	}
}
void GameNotation::PushMove(const std::string &in_piecesPosition, const std::string &pieceName,
							const Square in_origin, const Square in_dest, const bool isCapture,
							const bool specifyRank, const bool specifyFile, const bool whitesMove,
							const bool* castlingOptions, const Square enPassant, const int halfmoveClock,
							CheckOrMate checkOrMate)
{
	std::string fen = GetFENFromPosition(in_piecesPosition, whitesMove, castlingOptions, enPassant, halfmoveClock);
	std::string algebraic = GetAlgebraic(pieceName, in_origin, in_dest, isCapture, specifyRank, specifyFile, checkOrMate);
	NotationNode newNode(algebraic, fen, in_piecesPosition);
	m_vNotation.push_back(newNode);
}

std::string GameNotation::GetAlgebraic(const std::string &pieceName, const Square in_origin, const Square in_dest, const bool isCapture, const bool specifyRank, const bool specifyFile, CheckOrMate checkOrMate)
{
	std::string retVal;
	
	if (pieceName == "K" && in_dest.GetFile() == G && in_origin.GetFile() == E)
		retVal = "O-O";
	else if (pieceName == "K" && in_dest.GetFile() == C && in_origin.GetFile() == E)
		retVal = "O-O-O";
	else
	{
		retVal += pieceName == "P" ? "" : pieceName;
		if (pieceName == "P" && isCapture)
			retVal += char(in_origin.GetFile() + 'a');
		if (specifyFile)
			retVal += char(in_origin.GetFile() + 'a');
		if (specifyRank)
			retVal += char(in_origin.GetRank() + '1');
		retVal += isCapture ? "x" : "";
		retVal += char(in_dest.GetFile() + 'a');
		retVal += char(in_dest.GetRank() + '1');
		if (checkOrMate == eCheck)
			retVal += "+";
		else if (checkOrMate == eMate)
			retVal += "#";
		else if (checkOrMate == eDraw)
			retVal += "=";
	}
	return retVal;
}

bool GameNotation::IsPerpetual(const std::string &in_piecesPosition, const bool didWhiteJustMove)
{
	bool retVal(false);

	int numOfRepetitions = 0;
	for (auto notationNode : m_vNotation)
	{
		const bool isWhiteMove = notationNode.GetFEN().find('w') == std::string::npos;

		if (in_piecesPosition.compare(notationNode.GetPosition()) == 0 && didWhiteJustMove == isWhiteMove)
			++numOfRepetitions;
	}

	if (numOfRepetitions >= 2)
		retVal = true;

	return retVal;
}
//void GameNotation::PrintNotation()
//{
//	std::for_each(m_vNotation.begin(), m_vNotation.end(), [](NotationNode currNode) {currNode.Print(); });
//}

std::string Square::toString() const
{
	std::string retString;

	char file = 'a';
	file += (char)m_file;

	char rank = '1';
	rank += (char)m_rank;

	retString += file;
	retString += rank;

	return retString;
}

bool Board::CheckIsCheck()
{
	bool retVal = false;
	bool keepSearching = true;
	for (int i = 0; i < BoardSize && keepSearching; ++i)
	{
		for (int j = 0; j < BoardSize; ++j)
		{
			Piece* currPiece = GetPiece({ i,j });
			if (currPiece && m_lastColorMoved != currPiece->m_color)
			{
				std::vector<Piece*> captures = currPiece->CanPieceCapture(*this, { i,j });
				if (CanPieceCaptureKing(captures))
				{
					keepSearching = false;
					retVal = true;
					break;
				}
			}
		}
	}
	return retVal;
}

std::vector<Move> Board::GetLegalMoves(Color for_which_color)
{
	std::vector<Move> retVal;

	Board dummyBoard = *this;
	dummyBoard.m_lastColorMoved = for_which_color == eWhite ? eBlack: eWhite;

	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
		{
			Piece* currPiece = dummyBoard.GetPiece({ i,j });
			if (currPiece && currPiece->m_color == for_which_color)
			{
				std::vector<Move> pieceMoves = currPiece->GetLegalMoves(dummyBoard, { i, j });
				retVal.insert(retVal.end(), pieceMoves.begin(), pieceMoves.end());
			}

		}

	return retVal;
}

void Board::RemoveUndefendedCheckMoves(std::vector<Move>& legalMoves) const
{
	//if (GetCheckOrMate() == eCheck) TODO: Optimization. See if this can be put back
	{
		for (auto option = legalMoves.begin(); option != legalMoves.end();)
		{
			Board dummyBoard = *this;
			Piece* currPiece = dummyBoard.GetPiece(option->m_origin);
			dummyBoard.CommitMove(currPiece, option->m_origin, option->m_dest);
			dummyBoard.m_lastColorMoved = dummyBoard.m_lastColorMoved == eWhite ? eBlack : eWhite;
			if (dummyBoard.CheckIsCheck())
			{
				option = legalMoves.erase(option);
				continue;
			}
			++option;
		}
	}
}

bool Square::InBounds() const
{
	return GetFile() <= H && GetRank() <= Eight && GetFile() >= A && GetRank() >= One;
}

bool Board::CheckIsMate()
{
	bool retVal = false;
	Color for_which_color = m_lastColorMoved;
	std::vector<Move> legalMoves = GetLegalMoves(for_which_color);
	
	if (legalMoves.empty())
		retVal = true;

	return retVal;
}

bool Board::CanPieceCaptureKing(std::vector<Piece *> &captures)
{
	bool retVal = false;
	for (auto currCapture : captures)
	{
		if (dynamic_cast<King*>(currCapture))
		{
			retVal = true;
			break;
		}
	}

	return retVal;
}

Piece* Board::GetPiece(const Square inLocation) const
{
	if (inLocation.GetFile() < A || inLocation.GetFile() > H ||
		inLocation.GetRank() < One || inLocation.GetRank() > Eight)
		return nullptr;
		
	return board[inLocation.GetFile()][inLocation.GetRank()];
}