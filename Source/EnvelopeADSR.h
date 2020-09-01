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
class EnvelopeADSR  : public juce::Component, public juce::ComponentListener
{
public:
    EnvelopeADSR() : aAnchor(0.3, 0.2, 0.06, this)
    {
        //when child component bounds are set relative, parent size must be set first
        setSize(600, 400);
        printf("top width 1: %d\n", getWidth());
        printf("top height 1: %d\n", getHeight());
        addAndMakeVisible(aAnchor);
        aAnchor.reInitWithParent();
        //aAnchor.setToRelativeBounds();
        aAnchor.addComponentListener(this);
        printf("top width 2: %d\n", getWidth());
        printf("top height 2: %d\n", getHeight());
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
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override
    {
    }
    void resized() override
    {
        
    }

private:
    AnchorPoint aAnchor;
    float fAttackWindowRight, fDecayWindowRight, fSustainLevelBottom, fReleaseWindowLeft;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeADSR)
};
