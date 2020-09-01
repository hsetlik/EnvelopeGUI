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
    EnvelopeADSR() : aAnchor(0.3, 0.2, 0.06),
                     aRect(0.1, 0.1, 0.7, 0.5, &aAnchor)
    {
        setSize(600, 400);
        addAndMakeVisible(aRect);
        aRect.initPosition();
        addAndMakeVisible(aAnchor);
        aAnchor.setToRelativeBounds();
        aAnchor.addLimit(0.8f, AnchorPoint::xCeiling);
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
    AnchorPoint aAnchor;
    AnchorRect aRect;
    float fAttackWindowRight, fDecayWindowRight, fSustainLevelBottom, fReleaseWindowLeft;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeADSR)
};
