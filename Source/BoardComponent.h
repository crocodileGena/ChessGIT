#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "board.h"

class DraggedPiece
{
public:
	DraggedPiece() : draw(false), draggedImage(nullptr), position(0, 0) {}
	void SetPosition(const Point<int> in_pos) { position = in_pos; }
	Point<int> GetPosition() { return position; }
	void SetImage(Image* in_image) { draggedImage = in_image; }
	Image* GetImage() { return draggedImage; }
	void SetDraw(const bool in_draw) { draw = in_draw; }
	bool GetDraw() { return draw; }

private:
	bool draw;
	Image* draggedImage;
	Point<int> position;
};

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

class BalanceComponent : public Component
{
public:
	BalanceComponent(Board* inBoard) : myBoard(inBoard) {}
	void paint(Graphics& g) override;
private:
	Board* myBoard;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BalanceComponent)
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
	void UpdateState(const std::string in_algebraic, const std::string in_fen) { algebraic = in_algebraic; fen = in_fen; }
	//void SetActive(const bool inActive) { setToggleState(inActive, sendNotification); repaint(); }

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
	void SetActiveNode(const size_t newNode, const size_t oldNode) 
	{ 
		if (newNode != oldNode && newNode != 0 && oldNode != 0)
		{
			boardStates[newNode - 1]->setToggleState(true, sendNotification);
			boardStates[oldNode - 1]->setToggleState(false, sendNotification);
		}
		if (newNode == 0 && oldNode != 0)
			boardStates[oldNode - 1]->setToggleState(false, sendNotification);
		if (oldNode == 0 && newNode != 0)
			boardStates[newNode - 1]->setToggleState(true, sendNotification);
	}
	size_t GetBoardStatesSize() { return boardStates.size(); }
	void UpdatePromotionNode(const std::string in_algebraic, const std::string in_fen)
	{
		boardStates.getLast()->UpdateState(in_algebraic, in_fen);
		boardStates.getLast()->setName(in_algebraic);
		boardStates.getLast()->repaint();

	}
private:
	OwnedArray<BoardStateButton> boardStates;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MovesComponent)
};

class PiecesInventory : public Component
{
public:
	PiecesInventory();
	void resized() override;
	void paint(Graphics& g) override;

private:
	ImageButton whitePawnButton;
	ImageButton whiteKnightButton;
	ImageButton whiteBishopButton;
	ImageButton whiteRookButton;
	ImageButton whiteQueenButton;
	ImageButton whiteKingButton;
	ImageButton blackPawnButton;
	ImageButton blackKnightButton;
	ImageButton blackBishopButton;
	ImageButton blackRookButton;
	ImageButton blackQueenButton;
	ImageButton blackKingButton;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PiecesInventory)

};

class NotationComponent : public Component
{
public:
	NotationComponent(Board* inBoard);
	void resized() override;
	void paint(Graphics& g) override;
	void addBoardState(const std::string &in_fen, const std::string &in_algebraic);
	void Reset() { SetStateIndex(0);  movesComponent.Reset(); resized(); }
	Board* GetBoard() { return myBoard; }
	void SetStateIndex(const size_t in_dex) { movesComponent.SetActiveNode(in_dex, currentState); currentState = in_dex; }
	size_t GetStateIndex() { return currentState; }
	void UpdatePromotionNode() 
	{ 
		std::string algebraicStr;
		std::string fenStr;
		myBoard->m_gameNotation.GetLastNode(algebraicStr, fenStr);
		movesComponent.UpdatePromotionNode(algebraicStr, fenStr);
	}

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
	void mouseUp(const MouseEvent &event) override;
	void mouseDrag(const MouseEvent &event) override;
	Board* GetBoard() { return myBoard; }
	void SetBoard(Board* in_board) { myBoard = in_board; }
	void SetActiveSquare(const Square inSquare) { activeSquare = inSquare; }
	void SetCurrentEditPiece(const Pieces selectedPiece) { currentEditPiece = selectedPiece; }
	Pieces GetCurrentEditPiece() const { return currentEditPiece; }
	void ResetClicked();
	Image* GetPieceImage(const Piece* currPiece);
	Image* GetPieceImage(const Pieces currPiece);
	void SetPressedAlreadySelected(const bool in_selected) { alreadySelected = in_selected; }
	bool GetPressedAlreadySelected() { return alreadySelected; }

private:
	Board* myBoard;
	Square activeSquare;
	Square queeningSquare;
	QueeningComponent whiteQueeningComponent;
	QueeningComponent blackQueeningComponent;
	DraggedPiece draggedPiece;
	bool alreadySelected;
	Pieces currentEditPiece;

	Image background;
	Image blackBackground;
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