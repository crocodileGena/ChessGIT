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

class PrevStateComponent : public Component
{
public:
	PrevStateComponent() {}
	void paint(Graphics& g) override;
	Board* GetBoard() { return myBoard; }
private:
	Board* myBoard;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrevStateComponent)
};

class NextStateComponent : public Component
{
public:
	NextStateComponent() {}
	void paint(Graphics& g) override;
	Board* GetBoard() { return myBoard; }
private:
	Board* myBoard;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NextStateComponent)
};

class BoardStateButton : public TextButton
{
public:
	BoardStateButton() {}
	void paint(Graphics& g) override;
private:
	Board* myBoard;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoardStateButton)
};

class NotationComponent : public Component
{
public:
	NotationComponent(Board* inBoard);
	void resized() override;
	void paint(Graphics& g) override;
	void addBoardState();
private:
	Board* myBoard;
	NextStateComponent next;
	PrevStateComponent prev;
	OwnedArray<BoardStateButton> boardStates;

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