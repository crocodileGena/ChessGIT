//#include "stdafx.h"
#include "board.h"
#include <algorithm>
#include <cctype>
#include "Pieces.h"

void Board::PrintPiecesSum()
{
	int blackSum = 0;
	int whiteSum = 0;

	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
		{
			Piece* currPiece = GetPiece({ i,j });
			if (!currPiece)
				continue;
			if (eWhite == currPiece->m_color)
				whiteSum += currPiece->m_worth;
			else
				blackSum += currPiece->m_worth;
		}

	int overall = whiteSum - blackSum;
	char* sign = overall < 0 ? "-" : "+";
	std::cout << "White: " << whiteSum << ". Black : " 
		<< blackSum << ". Overall: " << sign
		<< overall << std::endl;
}
Board::Board() : m_lastColorMoved(eBlack), m_status("status bar")
{
	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
		{
			SetPiece({ i,j }, nullptr);
		}
}
void Board::ResetBoard()
{
	m_lastColorMoved = eBlack;
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

int Board::GetCastlingOptions()
{
	return 0; //TODO : implemetn this
}

Square Board::GetEnPassantSquare(const Square inBase, const Square inDest)
{
	Square retVal;
	return retVal; //TODO : implement
}

int Board::GetHalfmoveClock()
{
	return 0; // TODO : implement
}

void Board::MovePiece(const Square inBase, const Square inDest)
{
	bool isPieceMoved = false;
	auto currPiece = GetPiece(inBase);
	if (currPiece)
		if (m_lastColorMoved == currPiece->m_color)
			std::cout << "Same color ";
		else if (currPiece->MakeMove(*this, inBase, inDest))
		{
			const bool isCapture = GetPiece(inDest) != nullptr;
			const bool specifyRank = false; //TODO : Implement this
			const bool specifyFile = false; //ToDO : Implement this
			const bool whitesMove = currPiece->m_color == eWhite;
			const int castlingOptions = GetCastlingOptions();
			const Square enPassant = GetEnPassantSquare(inBase, inDest);
			const int halfmoveClock = GetHalfmoveClock();
			SetPiece(inBase, nullptr);
			SetPiece(inDest, currPiece);
			currPiece->OnPieceMoved();
			m_lastColorMoved = currPiece->m_color;
			m_gameNotation.PushMove(GetPiecesPosition(), currPiece->m_name, inBase, inDest, 
									isCapture, specifyRank, specifyFile, whitesMove, castlingOptions,
									enPassant, halfmoveClock);
			isPieceMoved = true;
		}

	if (!isPieceMoved)
		std::cout << "Illegal Move\n";
	else
		CheckIsCheck();
}

std::string GameNotation::GetFENFromPosition(const std::string in_position, const bool whitesMove,
											const int castlingOptions, const Square enPassant,
											const int halfmoveClock)
{
	return ""; //TODO: implement this
}
void GameNotation::PushMove(const std::string in_piecesPosition, const std::string pieceName, 
							const Square in_origin, const Square in_dest, const bool isCapture,
							const bool specifyRank, const bool specifyFile, const bool whitesMove,
							const int castlingOptions, const Square enPassant, 
							const int halfmoveClock)
{
	std::string fen = GetFENFromPosition(in_piecesPosition, whitesMove, castlingOptions, enPassant, halfmoveClock);
	std::string algebraic = GetAlgebraic(in_origin, in_dest, isCapture, specifyRank, specifyFile);
	NotationNode newNode(algebraic, fen);
	m_vNotation.push_back(newNode);
}

std::string GameNotation::GetAlgebraic(const Square in_origin, const Square in_dest, const bool isCapture, const bool specifyRank, const bool specifyFile)
{
	return ""; //TODO: Implement this
}
//void GameNotation::PrintNotation()
//{
//	std::for_each(m_vNotation.begin(), m_vNotation.end(), [](NotationNode currNode) {currNode.Print(); });
//}

std::string Square::toString() const
{
	std::string retString;

	char letter = 'a';
	letter += (char)m_pos[eLetter];

	char number = '1';
	number += (char)m_pos[eNumber];

	retString += letter;
	retString += number;

	return retString;
}

bool Board::CheckIsCheck()
{
	bool retVal = false;

	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
		{
			Piece* currPiece = GetPiece({ i,j });
			if (currPiece && m_lastColorMoved == currPiece->m_color)
			{
				std::vector<Piece*> captures = currPiece->CanPieceCapture(*this, { i,j });
				for (auto currCapture : captures)
				{
					if (dynamic_cast<King*>(currCapture))
						return true;
				}
			}
		}
	return retVal;
}

Piece* Board::GetPiece(const Square inLocation)
{
	if (inLocation.GetLetter() < A || inLocation.GetLetter() > H ||
		inLocation.GetNumber() < One || inLocation.GetNumber() > Eight)
		return nullptr;
		
	return board[inLocation.GetLetter()][inLocation.GetNumber()];
}