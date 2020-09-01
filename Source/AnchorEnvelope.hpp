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
public:
    enum limitType {xFloor, xCeiling, yFloor, yCeiling};
    struct Limit
    {
        float lVal;
        float* pVal;
        limitType lType;
        bool fromAnchor;
    };
private:
    juce::Colour anchorColor = juce::Colours::royalblue;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    juce::Component* box;
public:
    AnchorPoint(float pX, float pY, float pW);
    ~AnchorPoint() {}
    void paint(juce::Graphics &g) override;
    void setToRelativeBounds();
    juce::Rectangle<int> conRect;
    void resized() override {setToRelativeBounds();}
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    std::vector<Limit> limitSet;
    void addLimit(float value, limitType type);
    void addLimit(AnchorPoint* source, limitType type);
    void conformToPeerBounds();
    //data
    float fXpos, fYpos, fHeight, fWidth;
};
//=============================================

class AnchorRect : public juce::Component, public juce::ComponentListener
{
public:
    AnchorRect(float afX, float afY, float afW, float afH, AnchorPoint* anchor);
    ~AnchorRect() {}
    AnchorPoint* peerAnchor;
    juce::Rectangle<float> rLimits;
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;
    void paint(juce::Graphics &g) override;
    void initPosition();
    void checkPeerLimits();
    float fX, fY, fW, fH;
    juce::Rectangle<int> tempBounds;
};

