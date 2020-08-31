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
    std::vector<Limit> limitSetStatic;
    std::vector<Limit> limitSetAnchor;
public:
    AnchorPoint(float pX, float pY, float pW);
    ~AnchorPoint() {}
    void paint(juce::Graphics &g) override;
    void setToRelativeBounds();
    void resized() override {setToRelativeBounds();}
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void addLimit(float value, axis ax, limitType type);
    void addLimit(AnchorPoint* source, axis ax, limitType type);
    void checkStaticLimits();
    void checkAnchorLimits();
    void constrainXTo(float xLim);
    void constrainYTo(float yLim);
    //data
    float fXpos, fYpos, fWidth;
};
