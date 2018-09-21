#include "BoardComponent.h"
#include "Pieces.h"

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

	rookButton.setImages(true, false, true, rookImage, 0.8, Colours::transparentBlack, rookImage,
		1.0, Colours::transparentBlack, rookImage, 0.8, Colours::transparentBlack);
	queenButton.setImages(true, false, true, queenImage, 0.8, Colours::transparentBlack, queenImage,
		1.0, Colours::transparentBlack, queenImage, 0.8, Colours::transparentBlack);
	bishopButton.setImages(true, false, true, bishopImage, 0.8, Colours::transparentBlack, bishopImage,
		1.0, Colours::transparentBlack, bishopImage, 0.8, Colours::transparentBlack);
	knightButton.setImages(true, false, true, knightImage, 0.8, Colours::transparentBlack, knightImage,
		1.0, Colours::transparentBlack, knightImage, 0.8, Colours::transparentBlack);

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

	if (queeningSquare.GetFile() != kIllegalSquare)
	{
		if (queeningSquare.GetRank() == Eight)
			whiteQueeningComponent.setVisible(true);
		else
			blackQueeningComponent.setVisible(true);
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

	if (kIllegalSquare != activeSquare.getX())
		myBoard->MovePiece(origin, dest);

	if (myBoard->GetQueeningMode())
	{
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

NotationComponent::NotationComponent(Board* inBoard) : myBoard(inBoard)
{
	addAndMakeVisible(next);
	addAndMakeVisible(prev);
	
	auto dummyState = new BoardStateButton;
	auto dummyState1 = new BoardStateButton;
	auto dummyState2 = new BoardStateButton;
	auto dummyState3 = new BoardStateButton;
	auto dummyState4 = new BoardStateButton;

	boardStates.add(dummyState);
	boardStates.add(dummyState1);
	boardStates.add(dummyState2);
	boardStates.add(dummyState3);
	boardStates.add(dummyState4);
	addAndMakeVisible(*dummyState);
	addAndMakeVisible(*dummyState1);
	addAndMakeVisible(*dummyState2);
	addAndMakeVisible(*dummyState3);
	addAndMakeVisible(*dummyState4);

}

void NotationComponent::paint(Graphics& g)
{
	g.setColour(Colours::white);
	g.drawRect(getLocalBounds());
}

void NotationComponent::addBoardState()
{
	//TODO: Implement something
}

void NotationComponent::resized()
{
	auto componentBounds(getLocalBounds());
	int buttonSize(30);
	int spacing(5);
	Rectangle<int> currRect(spacing, componentBounds.getBottom() - buttonSize - spacing, buttonSize, buttonSize);
	prev.setBounds(currRect);
	currRect.setX(currRect.getRight() + spacing);
	next.setBounds(currRect);

	int xWhite(10);
	int xBlack(100);
	int yPos(10);
	int xPos(xWhite);
	int moveNumber(1);
	for (auto currState : boardStates)
	{
		if (moveNumber % 2)
		{
			yPos = yPos + 25;
			xPos = xWhite;
		}
		else
		{
			xPos = xBlack;
		}

		currState->setBounds(xPos, yPos, 50, 20);
		moveNumber++;
	}
}

void NextStateComponent::paint(Graphics& g)
{
	g.setColour(Colours::white);
	//g.setOpacity(0.5);
	g.fillRect(getLocalBounds());
	g.setColour(Colours::black);
	g.drawText("next", getLocalBounds(), Justification::centred, true);
	//g.fillRect(getLocalBounds());
}

void BoardStateButton::paint(Graphics& g)
{
	g.setColour(Colours::white);
	//g.setOpacity(0.5);
	g.fillRect(getLocalBounds());
	g.setColour(Colours::black);
	g.drawText("boardState1", getLocalBounds(), Justification::centred, true);
	//g.fillRect(getLocalBounds());
}

void PrevStateComponent::paint(Graphics& g)
{
	g.setColour(Colours::white);
	g.fillRect(getLocalBounds());
	g.setColour(Colours::black);
	g.drawText("prev", getLocalBounds(), Justification::centred, true);
}