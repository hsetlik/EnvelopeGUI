//
//  AnchorEnvelope.cpp
//  EnvelopeGUI - App
//
//  Created by Hayden Setlik on 8/30/20.
//

#include "AnchorEnvelope.hpp"

AnchorPoint::AnchorPoint(float pW, juce::Component* pTop)
{
    fXpos = 0.0f;
    fYpos = 0.0f;
    fHeight = pW;
    float aspectRatio = 1.0f * (getParentWidth() / getParentHeight());
    fWidth = fHeight * aspectRatio;
    setBoundsRelative(fXpos, fYpos, fWidth, fHeight);
    printf("fWidth: %f\n", fWidth);
    printf("fHeight: %f\n", fHeight);
    
}

void AnchorPoint::reInitWithParent()
{
    
}
void AnchorPoint::setToRelativeBounds()
{
    fWidth = fHeight * (getParentWidth() / getParentHeight());
    setBounds(fXpos, fYpos, fWidth, fHeight);
}

void AnchorPoint::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::white);
    g.fillAll();
    juce::Rectangle<float> anchorBounds = juce::Rectangle<float>(0.0, 0.0, (getParentWidth() * fWidth), (getParentHeight() * fHeight));
    g.setColour(anchorColor);
    g.fillEllipse(anchorBounds);
}

void AnchorPoint::mouseDown(const juce::MouseEvent &event)
{
    dragger.startDraggingComponent(this, event);
}

void AnchorPoint::mouseDrag(const juce::MouseEvent &event)
{
    dragger.dragComponent(this, event, &constrainer);
    printf("anchor x : %d\n", getX());
    printf("anchor y : %d\n", getY());
    //checkStaticLimits();
    //checkAnchorLimits();
}

void AnchorPoint::addLimit(float value, axis ax, limitType type)
{
    Limit newLim;
    newLim.lVal = value;
    newLim.lAxis = ax;
    newLim.lType = type;
    newLim.fromAnchor = false;
    limitSetStatic.push_back(newLim);
}

void AnchorPoint::addLimit(AnchorPoint *source, axis ax, limitType type)
{
    Limit newLim;
    newLim.lAxis = ax;
    newLim.lType = type;
    switch(ax)
    {
        case x:
            newLim.pVal = &source->fXpos;
        case y:
            newLim.pVal = &source->fYpos;
    }
    newLim.lVal = *newLim.pVal;
    limitSetAnchor.push_back(newLim);
}
//=====================================
AnchorBox::AnchorBox(float fX, float fY, float fW, float fH, float pW) : child(pW, nullptr)
{
    
}
