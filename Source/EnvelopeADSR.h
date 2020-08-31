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
    EnvelopeADSR() : aAnchor(0.3, 0.2, 0.06)
    {
        //when child component bounds are set relative, parent size must be set first
        setSize(600, 400);
        addAndMakeVisible(aAnchor);
        
    
        printf("x is at: %d\n", aAnchor.getX());
        printf("y is at: %d\n", aAnchor.getY());
        
      
    }

    ~EnvelopeADSR() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        aAnchor.repaint();
    }
    void resized() override
    {
        
    }

private:
    Anchor aAnchor;
    float fAttackWindowRight, fDecayWindowRight, fSustainLevelBottom, fReleaseWindowLeft;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeADSR)
};
