/*
  ==============================================================================

    EnvelopeADSR.h
    Created: 30 Aug 2020 6:22:55pm
    Author:  Hayden Setlik

  ==============================================================================
*/

#pragma once
#include "AnchorEnvelope.hpp"

#include <JuceHeader.h>

//==============================================================================
/*
*/
class EnvelopeADSR  : public juce::Component
{
public:
    EnvelopeADSR() : aBox(0.2, 0.2, 0.3, 0.3),
    dBox(0.55, 0.2, 0.3, 0.3)
    {
        //when child component bounds are set relative, parent size must be set first
        setSize(600, 400);
        addAndMakeVisible(aBox);
        aBox.setBoundsRelative(aBox.bX, aBox.bY, aBox.bW, aBox.bH);
        aBox.child.setBoundsRelative(aBox.child.fXpos, aBox.child.fYpos, aBox.child.fWidth, aBox.child.fHeight);
        addAndMakeVisible(dBox);
        dBox.setBoundsRelative(dBox.bX, dBox.bY, dBox.bW, dBox.bH);
        dBox.child.setBoundsRelative(dBox.child.fXpos, dBox.child.fYpos, dBox.child.fWidth, dBox.child.fHeight);
        aBox.addLimit(0.45f, AnchorBox::xCeiling);
        
    }

    ~EnvelopeADSR() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        //aAnchor.repaint();
    }
    void resized() override
    {
        aBox.resized();
        dBox.resized();
    }

private:
    AnchorBox aBox;
    AnchorBox dBox;
    float fAttackWindowRight, fDecayWindowRight, fSustainLevelBottom, fReleaseWindowLeft;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeADSR)
};
