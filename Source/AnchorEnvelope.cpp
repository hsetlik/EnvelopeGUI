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
    conRect = juce::Rectangle<int>(0, 0, getParentWidth(), getParentHeight()).reduced(15);
    printf("Reduction: %f\n", fWidth * getParentWidth());
    printf("conRect x: %d\n", conRect.getX());
    printf("conRect y: %d\n", conRect.getY());
    printf("conRect right: %d\n", conRect.getRight());
    printf("conRect bottom: %d\n", conRect.getBottom());
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

juce::Point<int> AnchorPoint::getTopLeft()
{
    
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
    g.drawRect(conRect, 1.0);
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
    // 3. make a temporary <float> rect to store the range values as fractions of the window
    // 4. convert that rectangle to an <int> rect and set contRect equal to that
    // 5. constrain()
    constrain();
}
