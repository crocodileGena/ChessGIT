#include "BoardComponent.h"
#include "Pieces.h"

int drawOffset = 18;
BoardComponent::BoardComponent() : activeSquare(0,0)
{
	myBoard = new Board;
	myBoard->ResetBoard();
	setBounds(20, 20, 100, 100);

	LoadImages();
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
}

void BoardComponent::mouseDown(const MouseEvent &event)
{
	Square origin({ activeSquare.getX(), 7 -activeSquare.getY() });
	juce::Point<int> currPosition = event.getPosition();
	activeSquare.setX((currPosition.getX() - drawOffset) / 60);
	activeSquare.setY((currPosition.getY() - drawOffset) / 60);
	Square dest({ activeSquare.getX(), 7 - activeSquare.getY() });

	if (origin == dest)
		activeSquare.setX(kIllegalSquare);

	if (kIllegalSquare != activeSquare.getX())
		myBoard->MovePiece(origin, dest);

	repaint();
}

ResetComponent::ResetComponent(BoardComponent* inBoard) : myBoard(inBoard)
{
	setBounds(550, 20, 50, 50);
}

void ResetComponent::mouseDown(const MouseEvent &event)
{
	myBoard->ResetBoard();
	repaint();
}