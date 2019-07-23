#include "BoardComponent.h"
#include "Pieces.h"
#include "MainComponent.h"

int drawOffset = 18;

void QueeningComponent::paint(Graphics& g)
{
	g.drawImageAt(backgroundImage, 0, 0);
}

QueeningComponent::QueeningComponent(const bool in_isWhite) :
isWhite(in_isWhite),
rookButton("rookSelection"),
queenButton("queenSelection"),
bishopButton("bishopSelection"),
knightButton("knightSelection")
{
	juce::String colorLetter = in_isWhite ? "w" : "b";
	String workingDir = File::getSpecialLocation(File::hostApplicationPath).getParentDirectory().getFullPathName();
	String separator = File::getSeparatorString();
	workingDir += separator + "Images" + separator;

	File f(workingDir + "QueeningBackground.png");
	backgroundImage = ImageFileFormat::loadFrom(f);

	File rookFile(workingDir + colorLetter + "Rook.png");
	rookImage = ImageFileFormat::loadFrom(rookFile);
	File queenFile(workingDir + colorLetter + "Queen.png");
	queenImage = ImageFileFormat::loadFrom(queenFile);
	File bishopFile(workingDir + colorLetter + "Bishop.png");
	bishopImage = ImageFileFormat::loadFrom(bishopFile);
	File knightFile(workingDir + colorLetter + "Knight.png");
	knightImage = ImageFileFormat::loadFrom(knightFile);

	rookButton.setImages(true, false, true, rookImage, 0.8f, Colours::transparentBlack, rookImage,
		1.0f, Colours::transparentBlack, rookImage, 0.8f, Colours::transparentBlack);
	queenButton.setImages(true, false, true, queenImage, 0.8f, Colours::transparentBlack, queenImage,
		1.0f, Colours::transparentBlack, queenImage, 0.8f, Colours::transparentBlack);
	bishopButton.setImages(true, false, true, bishopImage, 0.8f, Colours::transparentBlack, bishopImage,
		1.0f, Colours::transparentBlack, bishopImage, 0.8f, Colours::transparentBlack);
	knightButton.setImages(true, false, true, knightImage, 0.8f, Colours::transparentBlack, knightImage,
		1.0f, Colours::transparentBlack, knightImage, 0.8f, Colours::transparentBlack);

	addAndMakeVisible(rookButton);
	addAndMakeVisible(queenButton);
	addAndMakeVisible(bishopButton);
	addAndMakeVisible(knightButton);
}

void QueeningComponent::resized()
{
	rookButton.setBounds(5, 5, 60, 60);
	queenButton.setBounds(65, 5, 60, 60);
	bishopButton.setBounds(125, 5, 60, 60);
	knightButton.setBounds(185, 5, 60, 60);

	auto mainComponent = findParentComponentOfClass <MainComponent>();
	auto notationComponent = mainComponent->GetGameNotation();

	rookButton.onClick = [this, notationComponent]
	{
		myBoard->QueenAPawn(queeningSquare, "R");
		setVisible(false);
		getParentComponent()->repaint();
		notationComponent->UpdatePromotionNode();
	};
	queenButton.onClick = [this, notationComponent]
	{
		myBoard->QueenAPawn(queeningSquare, "Q");
		setVisible(false);
		getParentComponent()->repaint();
		notationComponent->UpdatePromotionNode();
	};
	bishopButton.onClick = [this, notationComponent]
	{
		myBoard->QueenAPawn(queeningSquare, "B");
		setVisible(false);
		getParentComponent()->repaint();
		notationComponent->UpdatePromotionNode();
	};
	knightButton.onClick = [this, notationComponent]
	{
		myBoard->QueenAPawn(queeningSquare, "N");
		setVisible(false);
		getParentComponent()->repaint();
		notationComponent->UpdatePromotionNode();
	};
}

void BoardComponent::resized()
{
	whiteQueeningComponent.setBounds(133, 222, 250, 70);
	blackQueeningComponent.setBounds(133, 222, 250, 70);
}

BoardComponent::BoardComponent() : 
activeSquare(kIllegalSquare, kIllegalSquare),
queeningSquare(kIllegalSquare, kIllegalSquare),
blackQueeningComponent(false),
whiteQueeningComponent(true),
alreadySelected(false),
currentEditPiece(noPiece)
{
	addChildComponent(whiteQueeningComponent);
	addChildComponent(blackQueeningComponent);
	myBoard = new Board;
	myBoard->ResetBoard();
	blackQueeningComponent.SetBoard(myBoard);
	whiteQueeningComponent.SetBoard(myBoard);
	LoadImages();
}

void BoardComponent::ResetClicked()
{
	SetActiveSquare({ kIllegalSquare, kIllegalSquare });
	repaint(); 
}
void BoardComponent::LoadImages()
{
	String workingDir = File::getSpecialLocation(File::hostApplicationPath).getParentDirectory().getFullPathName();
	String separator = File::getSeparatorString();
	workingDir += separator + "Images" + separator;

	File f(workingDir + "Background.jpg");
	background = ImageFileFormat::loadFrom(f);

	File bf(workingDir + "BlackBackground.jpg");
	blackBackground = ImageFileFormat::loadFrom(bf);

	File as(workingDir + "ActiveSquare.png");
	activeSquareImage = ImageFileFormat::loadFrom(as);

	File bk(workingDir + "bKing.png");
	bKing = ImageFileFormat::loadFrom(bk);

	File wk(workingDir + "wKing.png");
	wKing = ImageFileFormat::loadFrom(wk);

	File wq(workingDir + "wQueen.png");
	wQueen = ImageFileFormat::loadFrom(wq);

	File bq(workingDir + "bQueen.png");
	bQueen = ImageFileFormat::loadFrom(bq);

	File wr(workingDir + "wRook.png");
	wRook = ImageFileFormat::loadFrom(wr);

	File br(workingDir + "bRook.png");
	bRook = ImageFileFormat::loadFrom(br);

	File wb(workingDir + "wBishop.png");
	wBishop = ImageFileFormat::loadFrom(wb);

	File bb(workingDir + "bBishop.png");
	bBishop = ImageFileFormat::loadFrom(bb);

	File wn(workingDir + "wKnight.png");
	wKnight = ImageFileFormat::loadFrom(wn);

	File bn(workingDir + "bKnight.png");
	bKnight = ImageFileFormat::loadFrom(bn);

	File wp(workingDir + "wPawn.png");
	wPawn = ImageFileFormat::loadFrom(wp);

	File bp(workingDir + "bPawn.png");
	bPawn = ImageFileFormat::loadFrom(bp);

	File circle(workingDir + "circle.png");
	legalMoveImage = ImageFileFormat::loadFrom(circle);
}

Image* BoardComponent::GetPieceImage(const Pieces currPiece)
{
	Image* retVal = nullptr;
	switch (currPiece)
	{
	case whitePawn:
		retVal = &wPawn;
		break;
	case whiteBishop:
		retVal = &wBishop;
		break;
	case whiteKnight:
		retVal = &wKnight;
		break;
	case whiteRook:
		retVal = &wRook;
		break;
	case whiteQueen:
		retVal = &wQueen;
		break;
	case whiteKing:
		retVal = &wKing;
		break;
	case blackPawn:
		retVal = &bPawn;
		break;
	case blackBishop:
		retVal = &bBishop;
		break;
	case blackKnight:
		retVal = &bKnight;
		break;
	case blackRook:
		retVal = &bRook;
		break;
	case blackQueen:
		retVal = &bQueen;
		break;
	case blackKing:
		retVal = &bKing;
		break;
	}
	return retVal;
}

void BoardComponent::paint(Graphics& g)
{
	MainComponent* mainComponent = findParentComponentOfClass<MainComponent>();
	const bool isBlackView = mainComponent->IsBlackView();
	
	g.drawImageAt(isBlackView ? blackBackground : background, 0, 0);
	Image* imageToDraw = nullptr;

	if (activeSquare.GetFile() != kIllegalSquare && !mainComponent->GetEditModeState())
	{
		int xOffset = 0;
		int yOffset = 0;
		if (isBlackView)
		{
			xOffset = (7 - activeSquare.GetFile()) * 60 + drawOffset;
			yOffset = activeSquare.GetRank() * 60 + drawOffset;
		}
		else
		{
			xOffset = activeSquare.GetFile() * 60 + drawOffset;
			yOffset = (7 - activeSquare.GetRank()) * 60 + drawOffset;
		}
		g.drawImageAt(activeSquareImage, xOffset, yOffset);
	}
	
	for (int j = BoardSize - 1; j >= 0; --j)
		for (int i = 0; i < BoardSize; ++i)
		{
			Piece* currPiece = myBoard->GetPiece({ i,j });
			if (currPiece)
			{
				imageToDraw = GetPieceImage(currPiece);
				if (isBlackView)
					g.drawImageAt(*imageToDraw, (7 - i) * 60 + drawOffset, j * 60 + drawOffset);
				else
					g.drawImageAt(*imageToDraw, i * 60 + drawOffset, (7 - j) * 60 + drawOffset);
			}
			else
				imageToDraw = nullptr;
		}

	if (draggedPiece.GetDraw() && draggedPiece.GetImage())
		g.drawImageAt(*draggedPiece.GetImage(), draggedPiece.GetPosition().getX(), draggedPiece.GetPosition().getY());

	if (myBoard->GetQueeningMode())
	{
		if (queeningSquare.GetRank() == Eight)
			whiteQueeningComponent.setVisible(true);
		else
			blackQueeningComponent.setVisible(true);
	}
	else if (activeSquare.GetFile() != kIllegalSquare)
	{
		const Square mySquare = { activeSquare.GetFile(), activeSquare.GetRank() };
		Piece* myPiece = myBoard->GetPiece(mySquare);
		if (myPiece && myPiece->m_color != myBoard->m_lastColorMoved)
		{
			std::vector<Move> legalMoves = myPiece->GetLegalMoves(*myBoard, mySquare);
			for (auto move : legalMoves)
			{
				if (isBlackView)
					g.drawImageAt(legalMoveImage,  (7 - move.m_dest.GetFile()) * 60 + drawOffset, move.m_dest.GetRank() * 60 + drawOffset);
				else
					g.drawImageAt(legalMoveImage, move.m_dest.GetFile() * 60 + drawOffset, (7 - move.m_dest.GetRank()) * 60 + drawOffset);

			}
			
		}
	}
}

Image* BoardComponent::GetPieceImage(const Piece* currPiece)
{
	Image* imageToDraw = nullptr;

	if (currPiece)
	{
		int currWorth(currPiece->m_worth);
		Color currColor = currPiece->m_color;
		switch (currWorth)
		{
		case ePawn:
			imageToDraw = currColor == eWhite ? &wPawn : &bPawn;
			break;
		case eKnight:
		{
			if (currPiece->m_name == "B")
				imageToDraw = currColor == eWhite ? &wBishop : &bBishop;
			else
				imageToDraw = currColor == eWhite ? &wKnight : &bKnight;
		}
		break;
		case eRook:
			imageToDraw = currColor == eWhite ? &wRook : &bRook;
			break;
		case eQueen:
			imageToDraw = currColor == eWhite ? &wQueen : &bQueen;
			break;
		case eKing:
			imageToDraw = currColor == eWhite ? &wKing : &bKing;
			break;
		}
	}
	return imageToDraw;
}

void BoardComponent::mouseUp(const MouseEvent &event)
{
	MainComponent* mainComponent = findParentComponentOfClass<MainComponent>();
	const bool isBlackView = mainComponent->IsBlackView();

	Square origin({ activeSquare.GetFile(), activeSquare.GetRank() });
	juce::Point<int> destPos = event.getPosition();

	auto destX = (destPos.getX() - drawOffset) / 60;
	auto destY = (destPos.getY() - drawOffset) / 60;

	if (destPos.getX() - drawOffset < 0 || destPos.getY() - drawOffset < 0
		|| destX > 7 || destX < 0 || destY > 7 || destY < 0)
		destX = kIllegalSquare;

	Square dest({ destX, 7 - destY });
	if (isBlackView)
		dest.SetSquare(7 - dest.GetFile(), 7 - dest.GetRank());

	if (GetPressedAlreadySelected() && draggedPiece.GetDraw() == false)
		activeSquare.SetFile(kIllegalSquare);

	bool pieceMoved = false;
	if (kIllegalSquare != activeSquare.GetFile())
		pieceMoved = myBoard->MovePiece(origin, dest);

	if (pieceMoved)
	{
		// get notation from board
		auto newNode = myBoard->m_gameNotation.GetLastNode();
		// pass it to GUI Notation
		MainComponent *myParentComponent = dynamic_cast<MainComponent*>(getParentComponent());
		NotationComponent *gameNotationComponent = myParentComponent->GetGameNotation();
		gameNotationComponent->addBoardState(newNode.GetFEN(), newNode.GetAlgebraic());
		gameNotationComponent->resized();
		SetActiveSquare(dest);
		myParentComponent->RepaintBalance();
	}

	if (myBoard->GetQueeningMode())
	{
		if (dest.GetRank() == Eight)
			whiteQueeningComponent.SetQueeningSquare(dest);
		else
			blackQueeningComponent.SetQueeningSquare(dest);
		queeningSquare = dest;
	}

	draggedPiece.SetDraw(false);
	draggedPiece.SetImage(nullptr);
	repaint();
}

void BoardComponent::mouseDrag(const MouseEvent &event)
{
	if (draggedPiece.GetImage())
	{
		juce::Point<int> currPos = event.getPosition();
		currPos.setX(currPos.getX() - 30);
		currPos.setY(currPos.getY() - 30);

		draggedPiece.SetPosition(currPos);
		draggedPiece.SetDraw(true);
		repaint();
	}
}

void BoardComponent::mouseDown(const MouseEvent &event)
{
	MainComponent* mainComponent = findParentComponentOfClass<MainComponent>();
	const bool isBlackView = mainComponent->IsBlackView();

	Square origin({ activeSquare.GetFile(), activeSquare.GetRank() });
	juce::Point<int> destPos = event.getPosition();

	auto destX = (destPos.getX() - drawOffset) / 60;
	auto destY = (destPos.getY() - drawOffset) / 60;

	if (destPos.getX() - drawOffset < 0 || destPos.getY() - drawOffset < 0
		|| destX > 7 || destX < 0 || destY > 7 || destY < 0)
		destX = kIllegalSquare;

	Square dest({ destX, 7 - destY });
	if (isBlackView)
		dest.SetSquare(7 - dest.GetFile(), 7 - dest.GetRank());

	if (!myBoard->GetQueeningMode())
	{
		if (origin == dest)
			SetPressedAlreadySelected(true);
		else
		{
			activeSquare.SetFile(dest.GetFile());
			activeSquare.SetRank(dest.GetRank());
			SetPressedAlreadySelected(false);
		}
	}

	bool pieceMoved = false;
	if (kIllegalSquare != activeSquare.GetFile())
	{
		Pieces newPieceType = GetCurrentEditPiece();
		if (newPieceType)
		{
			delete myBoard->GetPiece(dest);
			Piece* newPiece = Piece::NewPiece(newPieceType);
			myBoard->SetPiece(dest, newPiece);
		}
		else
			pieceMoved = myBoard->MovePiece(origin, dest);
	}

	if (pieceMoved)
	{
		// get notation from board
		auto newNode = myBoard->m_gameNotation.GetLastNode();
		// pass it to GUI Notation
		MainComponent *myParentComponent = dynamic_cast<MainComponent*>(getParentComponent());
		NotationComponent *gameNotationComponent = myParentComponent->GetGameNotation();
		gameNotationComponent->addBoardState(newNode.GetFEN(), newNode.GetAlgebraic());
		gameNotationComponent->resized();
		myParentComponent->RepaintBalance();
	}

	if (myBoard->GetQueeningMode())
	{
		if (dest.GetRank() == Eight)
			whiteQueeningComponent.SetQueeningSquare(dest);
		else
			blackQueeningComponent.SetQueeningSquare(dest);
		queeningSquare = dest;
	}

	draggedPiece.SetPosition({ destPos.getX() - 30, destPos.getY() });
	Piece* currPiece = myBoard->GetPiece(dest);
	Image* currPieceImage = GetPieceImage(currPiece);
	draggedPiece.SetImage(currPieceImage);

	repaint();
}


void ResetButton::mouseDown(const MouseEvent &event)
{
	myBoard->ResetBoard();
	Button::mouseDown(event);
}

void BoardStateButton::mouseDown(const MouseEvent &event)
{
	auto myGrandParentComponent = findParentComponentOfClass <NotationComponent>();
	myGrandParentComponent->GetBoard()->LoadFEN(fen, myIndex);
	myGrandParentComponent->SetStateIndex(myIndex);

	Button::mouseDown(event);
}

void ResetButton::paint(Graphics& g)
{
	TextButton::paint(g);
	g.setColour(Colours::white);
	//g.drawRect(getLocalBounds());
	g.drawText("Reset Button", getLocalBounds(), Justification::centred, true);
}

void StatusComponent::paint(Graphics& g)
{
	auto currStatus= myBoard->GetStatus();
	g.setColour(Colours::white);
	g.drawText(currStatus, getLocalBounds(), Justification::centred, true);
}

void BalanceComponent::paint(Graphics& g)
{
	auto currBalance = myBoard->GetBalance();
	g.setColour(Colours::white);

	std::string sign = currBalance > 0 ? "white: +" : "white: ";
	if (currBalance != 0)
		g.drawText(sign + std::to_string(currBalance), getLocalBounds(), Justification::centred, true);
}

NotationComponent::NotationComponent(Board* inBoard) : 
myBoard(inBoard),
start("<<"),
next(">"),
prev("<"),
end(">>"),
currentState(0)
{
	vpMovesComponent.setViewedComponent(&movesComponent, false);
	addAndMakeVisible(next);
	addAndMakeVisible(prev);
	addAndMakeVisible(start);
	addAndMakeVisible(end);
	addAndMakeVisible(vpMovesComponent);
}


PiecesInventory::PiecesInventory()
{
	addAndMakeVisible(whitePawnButton);
	addAndMakeVisible(whiteKnightButton);
	addAndMakeVisible(whiteBishopButton);
	addAndMakeVisible(whiteRookButton);
	addAndMakeVisible(whiteQueenButton);
	addAndMakeVisible(whiteKingButton);
	addAndMakeVisible(blackPawnButton);
	addAndMakeVisible(blackBishopButton);
	addAndMakeVisible(blackKnightButton);
	addAndMakeVisible(blackRookButton);
	addAndMakeVisible(blackQueenButton);
	addAndMakeVisible(blackKingButton);
}

void PiecesInventory::paint(Graphics& g)
{
	auto bounds = getLocalBounds();
	g.setColour(Colours::white);
	g.drawRect(bounds);
}

void NotationComponent::paint(Graphics& g)
{
	auto bounds = getLocalBounds();
	g.setColour(Colours::white);
	g.drawRect(bounds);
}

void NotationComponent::addBoardState(const std::string &in_fen, const std::string &in_algebraic)
{
	movesComponent.addBoardState(in_fen, in_algebraic);
	SetStateIndex(GetStateIndex() + 1);
}

void MovesComponent::addBoardState(const std::string &in_fen, const std::string &in_algebraic)
{
	const size_t currIndex = boardStates.size() + 1;
	BoardStateButton *newState = new BoardStateButton(in_fen, in_algebraic, currIndex);
	boardStates.add(newState);
	setColour(3, Colours::white);
	addAndMakeVisible(*newState);
	newState->setColour(TextButton::buttonColourId, Colours::transparentWhite);
	newState->setColour(TextButton::buttonOnColourId, Colours::white);

	resized();

	auto mainComponent = findParentComponentOfClass <MainComponent>();
	auto boardComponent = mainComponent->GetBoardComponent();
	newState->onClick = [mainComponent, boardComponent] 
	{
		boardComponent->repaint(); 
	};
}

void MovesComponent::resized()
{
	int xWhite(30);
	int xBlack(90);
	int yPos(-moveButtonHeight + buttonSpacing);
	int xPos(xWhite);
	int moveNumber(1);
	for (auto currState : boardStates)
	{
		if (moveNumber % 2)
		{
			yPos = yPos + moveButtonHeight + buttonSpacing;
			xPos = xWhite;
		}
		else
		{
			xPos = xBlack;
		}

		currState->setBounds(xPos, yPos, moveButtonWidth, moveButtonHeight);
		moveNumber++;
	}
}

void PiecesInventory::resized()
{
	auto componentBounds(getLocalBounds());
	Rectangle<int> currRect(buttonSpacing, buttonSpacing, 60, 60);
	whitePawnButton.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	blackPawnButton.setBounds(currRect);
	currRect.setX(buttonSpacing);
	currRect.setY(currRect.getBottom() + buttonSpacing);
	whiteKnightButton.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	blackKnightButton.setBounds(currRect);
	currRect.setX(buttonSpacing);
	currRect.setY(currRect.getBottom() + buttonSpacing);
	whiteBishopButton.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	blackBishopButton.setBounds(currRect);
	currRect.setX(buttonSpacing);
	currRect.setY(currRect.getBottom() + buttonSpacing);
	whiteRookButton.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	blackRookButton.setBounds(currRect);
	currRect.setX(buttonSpacing);
	currRect.setY(currRect.getBottom() + buttonSpacing);
	whiteQueenButton.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	blackQueenButton.setBounds(currRect);
	currRect.setX(buttonSpacing);
	currRect.setY(currRect.getBottom() + buttonSpacing);
	whiteKingButton.setBounds(currRect);
	whiteKingButton.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	blackKingButton.setBounds(currRect);
	
	MainComponent *mainComponent = dynamic_cast<MainComponent*>(getParentComponent());
	auto boardComponent = mainComponent->GetBoardComponent();

	/*
		rookButton.setImages(true, false, true, rookImage, 0.8f, Colours::transparentBlack, rookImage,
			1.0f, Colours::transparentBlack, rookImage, 0.8f, Colours::transparentBlack);*/

	Image whitePawnImage = *boardComponent->GetPieceImage(Pieces::whitePawn);
	Image blackPawnImage = *boardComponent->GetPieceImage(Pieces::blackPawn);
	Image whiteKnightImage = *boardComponent->GetPieceImage(Pieces::whiteKnight);
	Image blackKnightImage = *boardComponent->GetPieceImage(Pieces::blackKnight);
	Image whiteBishopImage = *boardComponent->GetPieceImage(Pieces::whiteBishop);
	Image blackBishopImage = *boardComponent->GetPieceImage(Pieces::blackBishop);
	Image whiteRookImage = *boardComponent->GetPieceImage(Pieces::whiteRook);
	Image blackRookImage = *boardComponent->GetPieceImage(Pieces::blackRook);
	Image whiteQueenImage = *boardComponent->GetPieceImage(Pieces::whiteQueen);
	Image blackQueenImage = *boardComponent->GetPieceImage(Pieces::blackQueen);
	Image whiteKingImage = *boardComponent->GetPieceImage(Pieces::whiteKing);
	Image blackKingImage = *boardComponent->GetPieceImage(Pieces::blackKing);
	 
	whitePawnButton.setImages(true, false, true, whitePawnImage, 0.8f, Colours::transparentBlack, whitePawnImage,
		1.0f, Colours::transparentBlack, whitePawnImage, 0.8f, Colours::transparentBlack);
	blackPawnButton.setImages(true, false, true, blackPawnImage, 0.8f, Colours::transparentBlack, blackPawnImage,
		1.0f, Colours::transparentBlack, blackPawnImage, 0.8f, Colours::transparentBlack);
	whiteKnightButton.setImages(true, false, true, whiteKnightImage, 0.8f, Colours::transparentBlack, whiteKnightImage,
		1.0f, Colours::transparentBlack, whiteKnightImage, 0.8f, Colours::transparentBlack);
	blackKnightButton.setImages(true, false, true, blackKnightImage, 0.8f, Colours::transparentBlack, blackKnightImage,
		1.0f, Colours::transparentBlack, blackKnightImage, 0.8f, Colours::transparentBlack);
	whiteBishopButton.setImages(true, false, true, whiteBishopImage, 0.8f, Colours::transparentBlack, whiteBishopImage,
		1.0f, Colours::transparentBlack, whiteBishopImage, 0.8f, Colours::transparentBlack);
	blackBishopButton.setImages(true, false, true, blackBishopImage, 0.8f, Colours::transparentBlack, blackBishopImage,
		1.0f, Colours::transparentBlack, blackBishopImage, 0.8f, Colours::transparentBlack);
	whiteRookButton.setImages(true, false, true, whiteRookImage, 0.8f, Colours::transparentBlack, whiteRookImage,
		1.0f, Colours::transparentBlack, whiteRookImage, 0.8f, Colours::transparentBlack);
	blackRookButton.setImages(true, false, true, blackRookImage, 0.8f, Colours::transparentBlack, blackRookImage,
		1.0f, Colours::transparentBlack, blackRookImage, 0.8f, Colours::transparentBlack);
	whiteQueenButton.setImages(true, false, true, whiteQueenImage, 0.8f, Colours::transparentBlack, whiteQueenImage,
		1.0f, Colours::transparentBlack, whiteQueenImage, 0.8f, Colours::transparentBlack);
	blackQueenButton.setImages(true, false, true, blackQueenImage, 0.8f, Colours::transparentBlack, blackQueenImage,
		1.0f, Colours::transparentBlack, blackQueenImage, 0.8f, Colours::transparentBlack);
	whiteKingButton.setImages(true, false, true, whiteKingImage, 0.8f, Colours::transparentBlack, whiteKingImage,
		1.0f, Colours::transparentBlack, whiteKingImage, 0.8f, Colours::transparentBlack);
	blackKingButton.setImages(true, false, true, blackKingImage, 0.8f, Colours::transparentBlack, blackKingImage,
		1.0f, Colours::transparentBlack, blackKingImage, 0.8f, Colours::transparentBlack);

	whitePawnButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::whitePawn); };
	whiteKnightButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::whiteKnight); };
	whiteBishopButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::whiteBishop); };
	whiteRookButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::whiteRook); };
	whiteQueenButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::whiteQueen); };
	whiteKingButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::whiteKing); };
	blackPawnButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::blackPawn); };
	blackKnightButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::blackKnight); };
	blackBishopButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::blackBishop); };
	blackRookButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::blackRook); };
	blackQueenButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::blackQueen); };
	blackKingButton.onClick = [this, boardComponent] { boardComponent->SetCurrentEditPiece(Pieces::blackKing); };

}

void NotationComponent::resized()
{
	auto componentBounds(getLocalBounds());
	Rectangle<int> currRect(buttonSpacing, componentBounds.getBottom() - staticButtonHeight - buttonSpacing, staticButtonWidth, staticButtonHeight);
	start.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	prev.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	next.setBounds(currRect);
	currRect.setX(currRect.getRight() + buttonSpacing);
	end.setBounds(currRect);

	int moveNumber = movesComponent.GetNumberofMoves();
	int movesComponentHeight = (moveNumber) * (moveButtonHeight + buttonSpacing) + buttonSpacing;
	movesComponent.setBounds(0,0, componentBounds.getRight() - 10, movesComponentHeight);
	vpMovesComponent.setBounds(0, 2, componentBounds.getRight(), 350);
	vpMovesComponent.setViewPositionProportionately(1, 1);

	MainComponent *mainComponent = dynamic_cast<MainComponent*>(getParentComponent());
	auto boardComponent = mainComponent->GetBoardComponent();

	start.onClick = [this, boardComponent]
	{
		SetStateIndex(0);
		boardComponent->GetBoard()->LoadFEN(); 
		boardComponent->SetActiveSquare({ kIllegalSquare, kIllegalSquare });
		boardComponent->repaint(); 
	};
	
	end.onClick = [this, boardComponent]
	{
		const size_t lastStateIndex = movesComponent.GetBoardStatesSize();
		SetStateIndex(lastStateIndex);
		if (lastStateIndex == 0)
			return;
		std::string endPosition = movesComponent.GetNode(lastStateIndex - 1)->GetFEN();
		boardComponent->GetBoard()->LoadFEN(endPosition, lastStateIndex);
		boardComponent->SetActiveSquare({ kIllegalSquare, kIllegalSquare });
		boardComponent->repaint();
	};
	
	next.onClick = [this, boardComponent]
	{
		const size_t lastStateIndex = movesComponent.GetBoardStatesSize();
		const size_t stateIndex = GetStateIndex();
		if (stateIndex >= lastStateIndex)
			return;
		SetStateIndex(stateIndex + 1);
		std::string nextPosition = movesComponent.GetNode(stateIndex)->GetFEN();
		boardComponent->GetBoard()->LoadFEN(nextPosition, GetStateIndex());
		boardComponent->SetActiveSquare({ kIllegalSquare, kIllegalSquare });
		boardComponent->repaint();
	};
	
	prev.onClick = [this, boardComponent]
	{
		const size_t lastStateIndex = movesComponent.GetBoardStatesSize();
		const size_t stateIndex = GetStateIndex();
		if (stateIndex < 1 || lastStateIndex < stateIndex)
			return;
		SetStateIndex(stateIndex - 1);
		if (stateIndex == 1)
			boardComponent->GetBoard()->LoadFEN();
		else
		{
			std::string prevPosition = movesComponent.GetNode(stateIndex - 2)->GetFEN();
			boardComponent->GetBoard()->LoadFEN(prevPosition, GetStateIndex());
		}
		boardComponent->SetActiveSquare({ kIllegalSquare, kIllegalSquare });
		boardComponent->repaint();
	};

}

void MovesComponent::paint(Graphics& g)
{
	float yPos(-moveButtonHeight + buttonSpacing);
	int numStates = boardStates.size();
	Rectangle<float> rect(5.0f, yPos, 15.0f, 20.0f);
	for (int moveNumber = 1; moveNumber <= numStates; ++moveNumber)
	{
		if (moveNumber % 2)
			yPos = yPos + moveButtonHeight + buttonSpacing;

		g.setColour(Colours::navajowhite);
		rect.setY(yPos);
		g.drawText(std::to_string((int)((double)moveNumber / 2 + 0.6)), rect, Justification::centredLeft);
		moveNumber++;
	}
}

void BoardStateButton::paint(Graphics& g)
{
	TextButton::paint(g);
	g.setColour(Colours::grey);
	g.drawText(getName(), getLocalBounds(), Justification::centred, true);
}