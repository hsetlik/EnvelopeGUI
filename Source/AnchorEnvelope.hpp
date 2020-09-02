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
    juce::ComponentBoundsConstrainer constrainer;
    juce::Rectangle<int> conRect;
public:
    AnchorPoint(float pW);
    ~AnchorPoint() {}
    void paint(juce::Graphics &g) override;
    void setToRelativeBounds();
    void resized() override {setToRelativeBounds();}
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    //data
    float fXpos, fYpos, fHeight, fWidth;
};
//=============================================

class AnchorBox : public juce::Component, public juce::ComponentListener
{
public:
    AnchorBox(float x, float y, float w, float h);
    ~AnchorBox() {}
    AnchorPoint child;
    float bX, bY, bW, bH;
    int iX, iY, iW, iH;
    enum limitType {xFloor, xCeiling, yFloor, yCeiling};
    struct limit
    {
        limitType type;
        float* pVal;
        float val;
        bool fromAnchor;
    };
    void addLimit(float value, limitType lType);
    void addLimit(AnchorBox* limitingBox, limitType lType);
    std::vector<limit> allLimits;
    struct activeLimits
    {
        float leftMin;
        float rightMax;
        float topMin;
        float bottomMax;
    };
    activeLimits aLims;
    void setActiveLimits(); //determines which of the limits is actually able to act on the Anchor
    void constrainToLimits(); //binds the component to the aLims values
    void alignY(AnchorPoint* target);
    void forceAlign();
    AnchorPoint* alignTarget;
    void paint(juce::Graphics& g) override;
    void resized() override;
    //just calls setActiveLimits and constrainToLimits
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;
};



