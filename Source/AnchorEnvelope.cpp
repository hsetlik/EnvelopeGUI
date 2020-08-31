//
//  AnchorEnvelope.cpp
//  EnvelopeGUI - App
//
//  Created by Hayden Setlik on 8/30/20.
//

#include "AnchorEnvelope.hpp"

AnchorPoint::AnchorPoint(float pX, float pY, float pW)
{
    fXpos = pX;
    fYpos = pY;
    fWidth = pW;
    setBoundsRelative(fXpos, fYpos, fWidth, fWidth);
    conRect = getLocalBounds().reduced(5);
    constrain();
}

void AnchorPoint::constrain()
{
    constrainer.setSizeLimits(conRect.getX(),
                              conRect.getY(),
                              conRect.getX() + conRect.getWidth(),
                              conRect.getY() + conRect.getHeight());
    constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
}

void AnchorPoint::setToRelativeBounds()
{
    setBoundsRelative(fXpos, fYpos, fWidth, fWidth);
}

void AnchorPoint::paint(juce::Graphics &g)
{
    juce::Rectangle<float> anchorBounds = juce::Rectangle<float>(0.0, 0.0, getParentHeight() * fWidth, getParentHeight() * fWidth);
   
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

void AnchorPoint::checkLimits() //in the EnvelopeADSR component, this is called for each anchor as the movement callback
{
    // 1. loop through limitSet and find the highest floor limit and the lowest ceiling limit for each axis
    float xFloor = 0.0f, xCeiling = 1.0f,  yFloor = 0.0f, yCeiling = 1.0f; //these default to constrain tto the window
    for(int i = 0; i < limitSet.size(); ++i)
    {
        if(limitSet[i].fromAnchor)
            limitSet[i].lVal = *limitSet[i].pVal; //updating lVal from the pointer if it can move
        if(limitSet[i].lAxis == x)
        {
            if(limitSet[i].lType == floor && limitSet[i].lVal > xFloor)
                xFloor = limitSet[i].lVal;
            if(limitSet[i].lType == ceiling && limitSet[i].lVal < xCeiling)
                xCeiling = limitSet[i].lVal;
        }
        if(limitSet[i].lAxis == y)
        {
            if(limitSet[i].lType == floor && limitSet[i].lVal > yFloor)
                yFloor = limitSet[i].lVal;
            if(limitSet[i].lType == ceiling && limitSet[i].lVal < yCeiling)
                yCeiling = limitSet[i].lVal;
        }
    }
    // 2. send those values to the constrainerRng structs
    xRange.min = xFloor;
    xRange.max = xCeiling;
    yRange.min = yFloor;
    yRange.max = yCeiling;
    // 3. update conRect's bounds to reflect the constrainerRng values
    // 4. constrain()
    constrain();
}
