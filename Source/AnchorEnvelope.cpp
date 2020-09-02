//
//  AnchorEnvelope.cpp
//  EnvelopeGUI - App
//
//  Created by Hayden Setlik on 8/30/20.
//

#include "AnchorEnvelope.hpp"

AnchorPoint::AnchorPoint(float pW)
{
    fXpos = 0.0f;
    fYpos = 0.0f;
    fHeight = pW;
    fWidth = (fHeight * getParentHeight()) / getParentWidth();
    setBoundsRelative(fXpos, fYpos, fWidth, fHeight);
    printf("fWidth: %f\n", fWidth);
    printf("fHeight: %f\n", fHeight);
    conRect = getLocalBounds().reduced(5);
    constrainer.setSizeLimits(conRect.getX(),
                              conRect.getY(),
                              conRect.getX() + conRect.getWidth(),
                              conRect.getY() + conRect.getHeight());
    constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
}
void AnchorPoint::setToRelativeBounds()
{
    fWidth = (fHeight * getParentHeight()) / getParentWidth();
    setBoundsRelative(fXpos, fYpos, fWidth, fHeight);
}

void AnchorPoint::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::white);
    g.fillAll();
    juce::Rectangle<float> anchorBounds = juce::Rectangle<float>(0.0, 0.0, getWidth(), getHeight());
   
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
    limitSet.push_back(newLim);
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
    limitSet.push_back(newLim);
}


//=============================================

AnchorBox::AnchorBox(float x, float y, float w, float h) : child(0.06f)
{
    bX = x;
    bY = y;
    bW = w;
    bH = h;
    
    int pW = getParentWidth();
    int pH = getParentHeight();
    
    iX = bX * pW;
    iY = bY * pH;
    iW = bW * pW;
    iH = bH * pH;
    
    
    setBounds(iX, iY, iW, iH);
    
    
    addAndMakeVisible(child);
    child.addComponentListener(this);
    
    updateChildLimits();
}

void AnchorBox::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white);
}
void AnchorBox::updateChildLimits()
{
    
}

void AnchorBox::checkChildLimits(axisLimits limits)
{
    
}

void AnchorBox::componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized)
{
    printf("Component Moved\n");
}

