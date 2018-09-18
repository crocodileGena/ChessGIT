/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
//==============================================================================
MainComponent::MainComponent()
{
	addAndMakeVisible(boardGUI);
	addAndMakeVisible(resetBoard);
	addAndMakeVisible(boardStatus);
	addAndMakeVisible(gameNotation);
	resetBoard.SetBoard(boardGUI.GetBoard());
	boardStatus.SetBoard(boardGUI.GetBoard());
	resetBoard.onClick = [this] { boardGUI.repaint(); };
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
	boardStatus.setBounds(20, 520, 100, 20);
	gameNotation.setBounds(550, 100, 200, 400);	
}

//==============================================================================
