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


//=============================================

AnchorBox::AnchorBox(float x, float y, float w, float h, float anchorHeight) : child(anchorHeight)
{
    bX = x;
    bY = y;
    bW = w;
    bH = h;
    
    //initialize the limits to be the same as the arguments
    aLims.leftMin = bX;
    aLims.rightMax = bX + bW;
    aLims.topMin = bY;
    aLims.bottomMax = bY + bH;
    
    int pW = getParentWidth();
    int pH = getParentHeight();
    
    iX = bX * pW;
    iY = bY * pH;
    iW = bW * pW;
    iH = bH * pH;
    
    
    setBounds(iX, iY, iW, iH);
    
    
    addAndMakeVisible(child);
    child.addComponentListener(this);
}

void AnchorBox::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white);
}
void AnchorBox::addLimit(float value, limitType lType)
{
    limit newLim;
    newLim.val = value;
    newLim.type = lType;
    newLim.fromAnchor = false;
    newLim.pVal = nullptr;
    allLimits.push_back(newLim);
}

void AnchorBox::addLimit(AnchorBox *limitingBox, limitType lType)
{
    limit newLim;
    newLim.fromAnchor = true;
    newLim.type = lType;
    switch(lType)
    {
        case xFloor:
            newLim.pVal = &limitingBox->child.fXpos;
            break;
        case xCeiling:
            newLim.pVal = &limitingBox->child.fXpos;
            break;
        case yFloor:
            newLim.pVal = &limitingBox->child.fYpos;
            break;
        case yCeiling:
            newLim.pVal = &limitingBox->child.fYpos;
            break;
    }
    newLim.val = *newLim.pVal;
    allLimits.push_back(newLim);
}



void AnchorBox::componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized)
{
    printf("Component Moved\n");
    setActiveLimits();
    constrainToLimits();
}

void AnchorBox::resized()
{
    printf("window resized\n");
    setActiveLimits();
    constrainToLimits();
}

void AnchorBox::setActiveLimits()
{
    for(int i = 0; i < allLimits.size(); ++i)
    {
        if(allLimits[i].fromAnchor)
            allLimits[i].val = *allLimits[i].pVal;
        limitType cType = allLimits[i].type;
        float cVal = allLimits[i].val;
        switch(cType)
        {
            case xFloor:
                if(cVal > aLims.leftMin)
                    aLims.leftMin = cVal;
                break;
            case xCeiling:
                if(cVal < aLims.rightMax)
                    aLims.rightMax = cVal;
                break;
            case yFloor:
                if(cVal > aLims.topMin)
                    aLims.topMin = cVal;
                break;
            case yCeiling:
                if(cVal < aLims.bottomMax)
                    aLims.bottomMax = cVal;
        }
    }
}

void AnchorBox::constrainToLimits()
{
    setBoundsRelative(aLims.leftMin, aLims.topMin, aLims.rightMax - aLims.leftMin, aLims.bottomMax - aLims.topMin);
}
