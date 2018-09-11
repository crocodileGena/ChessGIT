#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "board.h"

#define kIllegalSquare 9

class ResetComponent : public Component
{
public:
	ResetComponent() : myBoard(nullptr) { setBounds(550, 20, 50, 50); }
	void SetBoard(Board* in_board) { myBoard = in_board; }
	void mouseDown(const MouseEvent &event) override;
	void paint(Graphics& g) override;
private:
	Board* myBoard;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ResetComponent)
};

class BoardComponent : public Component
{
public:
	BoardComponent();
	void LoadImages();
	void paint(Graphics& g) override;
	void mouseDown(const MouseEvent &event) override;
	Board* GetBoard() { return myBoard; }
private:
	Board* myBoard;
	juce::Point<int> activeSquare;

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