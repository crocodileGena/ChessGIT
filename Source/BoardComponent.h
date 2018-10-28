#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "board.h"

class ResetButton : public TextButton
{
public:
	ResetButton(Board* inBoard) : myBoard(inBoard) {}
	void mouseDown(const MouseEvent &event) override;
	void paint(Graphics& g) override;
private:
	Board* myBoard;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ResetButton)
};

class StatusComponent : public Component
{
public:
	StatusComponent(Board* inBoard) : myBoard(inBoard) {}
	void paint(Graphics& g) override;
private:
	Board* myBoard;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusComponent)
};

class BoardStateButton : public TextButton
{
public:
	BoardStateButton(const std::string in_fen, const std::string in_algebraic, const size_t in_dex) :
		fen(in_fen), algebraic(in_algebraic), myIndex(in_dex) {
		setName(in_algebraic);
	}
	void mouseDown(const MouseEvent &event) override;
	void paint(Graphics& g) override;
	std::string GetFEN() { return fen; }

private:
	size_t myIndex;
	std::string fen;
	std::string algebraic;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoardStateButton)
};

class MovesComponent : public Viewport
{
public:
	MovesComponent() {}
	void resized() override;
	void paint(Graphics& g) override;
	void addBoardState(const std::string &in_fen, const std::string &in_algebraic);
	void Reset() { boardStates.clear(); }
	int GetNumberofMoves() { return int(((double)boardStates.size() / 2) + 0.6); }
	BoardStateButton* GetNode(const size_t index) { return boardStates[index]; }
	size_t GetBoardStatesSize() { return boardStates.size(); }

private:
	OwnedArray<BoardStateButton> boardStates;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MovesComponent)
};

class NotationComponent : public Component
{
public:
	NotationComponent(Board* inBoard);
	void resized() override;
	void paint(Graphics& g) override;
	void addBoardState(const std::string &in_fen, const std::string &in_algebraic);
	void Reset() { movesComponent.Reset(); resized(); }
	Board* GetBoard() { return myBoard; }
	void SetStateIndex(const size_t in_dex) { currentState = in_dex; }
	size_t GetStateIndex() { return currentState; }

private:
	Board* myBoard;
	TextButton start;
	TextButton next;
	TextButton prev;
	TextButton end;
	MovesComponent movesComponent;
	Viewport vpMovesComponent;
	size_t currentState;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NotationComponent)
};

class QueeningComponent : public Component
{
public:
	QueeningComponent(const bool isWhite);
	void resized() override;
	void paint(Graphics& g) override;
	void SetBoard(Board* in_board) { myBoard = in_board; }
	void SetQueeningSquare(const Square in_square) { queeningSquare = in_square; }

private:
	Board* myBoard;
	Square queeningSquare;
	bool isWhite;
	Image backgroundImage;
	Image rookImage;
	Image queenImage;
	Image bishopImage;
	Image knightImage;
	ImageButton rookButton;
	ImageButton queenButton;
	ImageButton bishopButton;
	ImageButton knightButton;

};

class BoardComponent : public Component
{
public:
	BoardComponent();
	void LoadImages();
	void paint(Graphics& g) override;
	void resized() override;
	void mouseDown(const MouseEvent &event) override;
	Board* GetBoard() { return myBoard; }
	void SetAvtiveSquare(juce::Point<int> inSquare) { activeSquare = inSquare; }
	void ResetClicked();
private:
	Board* myBoard;
	juce::Point<int> activeSquare;
	Square queeningSquare;
	QueeningComponent whiteQueeningComponent;
	QueeningComponent blackQueeningComponent;

	Image background;
	Image activeSquareImage;
	Image legalMoveImage;
	Image bKing;
	Image wKing;
	Image wQueen;
	Image bQueen;
	Image bRook;
	Image wRook;
	Image bBishop;
	Image wBishop;
	Image wKnight;
	Image bKnight;
	Image bPawn;
	Image wPawn;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoardComponent)
};