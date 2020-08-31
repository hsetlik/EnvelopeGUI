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

enum limitType {floor, ceiling};



class AnchorPoint : juce::Component
{
public:
    AnchorPoint(float xRatio, float yRatio);
    ~AnchorPoint() {}
    void paint(juce::Graphics &g) override;
    void addLimit(float fLimit, bool axis, limitType type);
    void addLimit(AnchorPoint* source, bool onXAxis, limitType type);
    void setXRelative(float ratio);
    void setYRelative(float ratio);
    float getXRelative();
    float getYRelative();
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void checkLimits();
    
private:
    struct limit
    {
        limit(float ratio, bool axis, limitType sType, bool isFromAnchor)
        {
            fLimit = ratio;
            onXAxis = axis;
            type = sType;
            fromAnchor = isFromAnchor;
        }
        AnchorPoint* sourceAnchor;
        float fLimit;
        bool onXAxis;
        bool fromAnchor;
        limitType type;
    };
    std::vector<limit> LimitSet;
    juce::ComponentDragger dragger;
    juce::Colour centerColor = juce::Colours::lightblue;
    juce::Colour rimColor = juce::Colours::blue;
    juce::ComponentBoundsConstrainer constrainer;
    float fXpos, fYpos;
    float fRectSide = 0.045f;
};
