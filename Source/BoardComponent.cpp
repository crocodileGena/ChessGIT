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


	rookButton.onClick = [this] { myBoard->QueenAPawn(queeningSquare, "R"); setVisible(false); getParentComponent()->repaint(); };
	queenButton.onClick = [this] { myBoard->QueenAPawn(queeningSquare, "Q"); setVisible(false); getParentComponent()->repaint(); };
	bishopButton.onClick = [this] { myBoard->QueenAPawn(queeningSquare, "B"); setVisible(false); getParentComponent()->repaint(); };
	knightButton.onClick = [this] { myBoard->QueenAPawn(queeningSquare, "N"); setVisible(false); getParentComponent()->repaint(); };
}

void QueeningComponent::resized()
{
	rookButton.setBounds(5, 5, 60, 60);
	queenButton.setBounds(65, 5, 60, 60);
	bishopButton.setBounds(125, 5, 60, 60);
	knightButton.setBounds(185, 5, 60, 60);
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
whiteQueeningComponent(true)
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
	SetAvtiveSquare({ kIllegalSquare, kIllegalSquare });
	repaint(); 
}
void BoardComponent::LoadImages()
{
	String workingDir = File::getSpecialLocation(File::hostApplicationPath).getParentDirectory().getFullPathName();
	String separator = File::getSeparatorString();
	workingDir += separator + "Images" + separator;

	File f(workingDir + "Background.jpg");
	background = ImageFileFormat::loadFrom(f);

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

void BoardComponent::paint(Graphics& g)
{
	g.drawImageAt(background, 0, 0);
	Image* imageToDraw = nullptr;

	if (activeSquare.getX() != kIllegalSquare)
		g.drawImageAt(activeSquareImage, activeSquare.getX()*60 + drawOffset, activeSquare.getY()*60 + drawOffset);
	
	for (int j = BoardSize - 1; j >= 0; --j)
		for (int i = 0; i < BoardSize; ++i)
		{
			Piece* currPiece = myBoard->GetPiece({ i,j });

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
				default:
					continue;
				}
				g.drawImageAt(*imageToDraw, i * 60 + drawOffset, (7 - j) * 60 + drawOffset);
			}
			else
				imageToDraw = nullptr;
		}

	if (myBoard->GetQueeningMode())
	{
		if (queeningSquare.GetRank() == Eight)
			whiteQueeningComponent.setVisible(true);
		else
			blackQueeningComponent.setVisible(true);
	}
	else if (activeSquare.getX() != kIllegalSquare)
	{
		const Square mySquare = { activeSquare.getX(), 7 - activeSquare.getY() };
		Piece* myPiece = myBoard->GetPiece(mySquare);
		if (myPiece && myPiece->m_color != myBoard->m_lastColorMoved)
		{
			std::vector<Move> legalMoves = myPiece->GetLegalMoves(*myBoard, mySquare);
			for (auto move :legalMoves)
				g.drawImageAt(legalMoveImage,  move.m_dest.GetFile() * 60 + drawOffset, (7 - move.m_dest.GetRank()) * 60 + drawOffset);
			
		}
	}

}

void BoardComponent::mouseDown(const MouseEvent &event)
{
	Square origin({ activeSquare.getX(), 7 -activeSquare.getY() });
	juce::Point<int> destPos = event.getPosition();

	auto destX = (destPos.getX() - drawOffset) / 60;
	auto destY = (destPos.getY() - drawOffset) / 60;

	if (destPos.getX() - drawOffset < 0 || destPos.getY() - drawOffset < 0
		|| destX > 7 || destX < 0 || destY > 7 || destY < 0)
		destX = kIllegalSquare;

	Square dest({ destX, 7 -destY });
	if (!myBoard->GetQueeningMode())
	{
		activeSquare.setX(destX);
		activeSquare.setY(destY);

		if (origin == dest)
			activeSquare.setX(kIllegalSquare);
	}

	bool pieceMoved = false;
	if (kIllegalSquare != activeSquare.getX())
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
	}

	if (myBoard->GetQueeningMode())
	{
		if (dest.GetRank() == Eight)
			whiteQueeningComponent.SetQueeningSquare(dest);
		else
			blackQueeningComponent.SetQueeningSquare(dest);
		queeningSquare = dest;
	}

	repaint();
}


void ResetButton::mouseDown(const MouseEvent &event)
{
	myBoard->ResetBoard();
	Button::mouseDown(event);
}

void ResetButton::paint(Graphics& g)
{
	g.setColour(Colours::white);
	g.drawRect(getLocalBounds());
	g.drawText("Reset Button", getLocalBounds(), Justification::centred, true);
}

void StatusComponent::paint(Graphics& g)
{
	auto currStatus= myBoard->GetStatus();
	g.setColour(Colours::white);
	g.drawText(currStatus, getLocalBounds(), Justification::centred, true);
}

NotationComponent::NotationComponent(Board* inBoard) : 
myBoard(inBoard),
start("<<"),
next(">"),
prev("<"),
end(">>")
{
	vpMovesComponent.setViewedComponent(&movesComponent, false);
	addAndMakeVisible(next);
	addAndMakeVisible(prev);
	addAndMakeVisible(start);
	addAndMakeVisible(end);
	addAndMakeVisible(vpMovesComponent);
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
}

void MovesComponent::addBoardState(const std::string &in_fen, const std::string &in_algebraic)
{
	BoardStateButton *newState = new BoardStateButton(in_fen, in_algebraic);
	boardStates.add(newState);
	setColour(3, Colours::white);
	addAndMakeVisible(*newState);
	resized();
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
