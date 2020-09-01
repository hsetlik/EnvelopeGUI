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
    enum limitType{ xFloor,
                    yFloor,
                    xCeiling,
                    yCeiling
    };
private:
    juce::Colour anchorColor = juce::Colours::royalblue;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    struct Limit
    {
        float lVal;
        float* pVal;
        limitType lType;
        bool fromAnchor;
    };
    std::vector<Limit> limitSet;
public:
    juce::Rectangle<int> conRect;
    int centerScreenX();
    int centerScreenY();
    AnchorPoint(float pX, float pY, float pW);
    ~AnchorPoint() {}
    void paint(juce::Graphics &g) override;
    void setToRelativeBounds();
    void resized() override {setToRelativeBounds();}
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void addLimit(float value, limitType type);
    void addLimit(AnchorPoint* source, limitType type);
    void checkLimits();
    //data
    float fXpos, fYpos, fHeight, fWidth;
};
