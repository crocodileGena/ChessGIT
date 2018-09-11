/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
//==============================================================================
MainComponent::MainComponent(): resetBoard(&boardGUI)
{
	addAndMakeVisible(boardGUI);
	addAndMakeVisible(resetBoard);
	addAndMakeVisible(gameNotation);
	gameNotation.setText("hihi");
	gameNotation.setColour(Colours::white);
	gameNotation.setFont(Font(16.0f), false);
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

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	boardGUI.setBounds(0, 0, 515, 515);
	boardGUI.setBounds(550, 20, 50, 50);
	gameNotation.setBounds(100, 100, 100, 400);
	juce::Parallelogram<float> bb({ 550.0f, 100.0f }, { 700.0f, 100.0f }, { 550.0f, 200.0f });
	gameNotation.setBoundingBox(bb);
}

//==============================================================================
