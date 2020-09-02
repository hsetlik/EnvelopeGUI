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
    enum axis {x, y};
    enum limitType {floor, ceiling};
private:
    juce::Colour anchorColor = juce::Colours::royalblue;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    juce::Rectangle<int> conRect;
    struct Limit
    {
        float lVal;
        float* pVal;
        axis lAxis;
        limitType lType;
        bool fromAnchor;
    };
    std::vector<Limit> limitSet;
public:
    AnchorPoint(float pW);
    ~AnchorPoint() {}
    void paint(juce::Graphics &g) override;
    void setToRelativeBounds();
    void resized() override {setToRelativeBounds();}
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void addLimit(float value, axis ax, limitType type);
    void addLimit(AnchorPoint* source, axis ax, limitType type);
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
    struct axisLimits
    {
        float xFloor;
        float xCeiling;
        float yFloor;
        float yCeiling;
    };
    void checkChildLimits(axisLimits limits);
    void paint(juce::Graphics& g) override;
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override;
    void updateChildLimits(); //finds the 2 limits for each axis, saves them to the struct and then passes it to checkCHildLimits
};



