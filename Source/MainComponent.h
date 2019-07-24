/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "BoardComponent.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	NotationComponent* GetGameNotation() { return &gameNotation; }
	BoardComponent* GetBoardComponent() { return &boardGUI; }
	bool IsBlackView() { return flipBoard.getToggleState(); }
	void RepaintBalance() { boardBalance.repaint(); }
	void RepaintStatus() { boardStatus.repaint(); }


private:
    //==============================================================================
    // Your private member variables go here...
	BoardComponent		boardGUI;
	NotationComponent	gameNotation;
	ResetButton			resetBoard;
	ToggleButton		flipBoard;
	StatusComponent		boardStatus;
	BalanceComponent	boardBalance;
	ToggleButton		editModeToggle;
	PiecesInventory		piecesInventory;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

