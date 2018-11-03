/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
//==============================================================================
MainComponent::MainComponent() : 
boardGUI(), 
gameNotation(boardGUI.GetBoard()),
resetBoard(boardGUI.GetBoard()),
boardStatus(boardGUI.GetBoard()),
flipBoard("Black View"),
editModeToggle("Edit Board")
{
	addAndMakeVisible(boardGUI);
	addAndMakeVisible(resetBoard);
	addAndMakeVisible(boardStatus);
	addAndMakeVisible(gameNotation);
	addAndMakeVisible(flipBoard);
	addAndMakeVisible(editModeToggle);
	addAndMakeVisible(piecesInventory);

	editModeToggle.setColour(TextButton::buttonColourId, Colours::transparentWhite);
	editModeToggle.setColour(TextButton::buttonOnColourId, Colours::white);

	resetBoard.onClick = [this] { boardGUI.ResetClicked(); GetGameNotation()->Reset(); };
	flipBoard.onClick = [this] {boardGUI.repaint(); };
	editModeToggle.onClick = [this] 
	{
		gameNotation.setVisible(!editModeToggle.getToggleState()); 
		piecesInventory.setVisible(editModeToggle.getToggleState());
		boardGUI.SetCurrentEditPiece(noPiece);
		boardGUI.repaint();
	};
	

    setSize (800, 600);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	boardGUI.setBounds(0, 0, 515, 515);
	resetBoard.setBounds(550, 20, 100, 20);
	editModeToggle.setBounds(670, 20, 100, 20);
	flipBoard.setBounds(550, 50, 100, 20);
	boardStatus.setBounds(20, 520, 100, 20);
	gameNotation.setBounds(550, 100, 150, 400);	
	piecesInventory.setBounds(550, 100, 150, 400);

	const bool isEditMode = editModeToggle.getToggleState();
	piecesInventory.setVisible(isEditMode);
	gameNotation.setVisible(!isEditMode);
}

//==============================================================================
