//
//  AnchorEnvelope.hpp
//  EnvelopeGUI - App
//
//  Created by Hayden Setlik on 8/30/20.
//
#pragma once

#include <JuceHeader.h>
#include <stdio.h>
#include <vector>

class AnchorPoint : public juce::Component
{
private:
    juce::Colour anchorColor = juce::Colours::royalblue;
    juce::ComponentDragger dragger;
    
public:
    juce::ComponentBoundsConstrainer pConstrainer;
    AnchorPoint(float pX, float pY, float pW);
    ~AnchorPoint() {}
    void paint(juce::Graphics &g) override;
    int getCenterX();
    int getCenterY();
    void resized() override {}
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    //data
    float fXpos, fYpos, fWidth;
};


class Anchor : public juce::Component, public juce::ComponentListener
{
public:
    Anchor(float pX, float pY, float pW);
    AnchorPoint child;
    juce::ComponentBoundsConstrainer* pChild;
    juce::Rectangle<int> conRect;
    ~Anchor(){}
    void paint(juce::Graphics& g) override;
    //below here is to do with constraining
    enum limitType {floor, ceiling};
    struct limit
    {
        float value;
        limitType type;
        bool onXAxis;
        bool fromAnchor;
    };
    void addLimit(float val, limitType type, bool onX){}
    void addLimit(AnchorPoint* source, limitType type, bool onX){}
    std::vector<limit> limitSet;
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;
};
