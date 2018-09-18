#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "board.h"

#define kIllegalSquare 8

class ResetButton : public TextButton
{
public:
	ResetButton() : myBoard(nullptr) {}
	void SetBoard(Board* in_board) { myBoard = in_board; }
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
	StatusComponent() : myBoard(nullptr) {}
	void SetBoard(Board* in_board) { myBoard = in_board; }
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

class BoardStateComponent : public Component
{
public:
	BoardStateComponent() {}
	void paint(Graphics& g) override;
	Board* GetBoard() { return myBoard; }
private:
	Board* myBoard;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoardStateComponent)
};

class NotationComponent : public Component
{
public:
	NotationComponent();
	void resized() override;
	void paint(Graphics& g) override;
	Board* GetBoard() { return myBoard; }
private:
	Board* myBoard;
	NextStateComponent next;
	PrevStateComponent prev;
	OwnedArray<BoardStateComponent> boardStates;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NotationComponent)
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