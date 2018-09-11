#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "board.h"

#define kIllegalSquare 9

class ResetComponent : public Component
{
public:
	ResetComponent(BoardComponent* inBoard);
	void mouseDown(const MouseEvent &event) override;
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