//
//  AnchorEnvelope.cpp
//  EnvelopeGUI - App
//
//  Created by Hayden Setlik on 8/30/20.
//

#include "AnchorEnvelope.hpp"


AnchorPoint::AnchorPoint(float xRatio, float yRatio)
{
    fXpos = xRatio;
    fYpos = yRatio;
    setBoundsRelative(xRatio, yRatio, fRectSide, fRectSide);
    juce::Rectangle<int> bounds = getBoundsInParent().reduced(5);
    constrainer.setSizeLimits(bounds.getY(),
                              bounds.getX(),
                              bounds.getWidth(),
                              bounds.getHeight());
    constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
}

void AnchorPoint::paint(juce::Graphics &g)
{
    juce::Rectangle<float> anchorRect = getBounds().toFloat();
    g.setColour(centerColor);
    g.fillEllipse(anchorRect);
    g.setColour(rimColor);
    g.drawEllipse(anchorRect, 2.0);
}

void AnchorPoint::setXRelative(float ratio)
{
    float fY = getY() / getParentHeight();
    setCentreRelative(ratio, fY);
}

void AnchorPoint::setYRelative(float ratio)
{
    float fX = (float)(getX() / getParentWidth());
    setCentreRelative(fX, ratio);
}

float::AnchorPoint::getXRelative()
{
    return (float)(getX() / getParentWidth());
}

float::AnchorPoint::getYRelative()
{
    return (float)(getY() / getParentHeight());
}

void AnchorPoint::mouseDown(const juce::MouseEvent &event)
{
    dragger.startDraggingComponent(this, event);
}

void AnchorPoint::mouseDrag(const juce::MouseEvent &event)
{
    checkLimits();
    dragger.dragComponent(this, event, &constrainer);
    
}

void AnchorPoint::addLimit(float fLimit, bool onXAxis, limitType type)
{
    limit newLimit(fLimit,onXAxis, type, false);
    LimitSet.push_back(newLimit);
}

void AnchorPoint::addLimit(AnchorPoint *source, bool onXAxis, limitType type)
{
    float sourceRatio;
    if(onXAxis)
        sourceRatio = (float)(source->getX() / getParentWidth());
    else
        sourceRatio = (float)(source->getY() / getParentHeight());
    LimitSet.push_back(limit(sourceRatio, onXAxis, type, true));
    LimitSet.back().sourceAnchor = source;
}

void AnchorPoint::checkLimits()
{
    for(int i = 0; i <  LimitSet.size(); ++i)
    {
        float iLimitRatio;
        float iCurrentRatio;
        //determining the limit values for limits from other anchor points
        if(LimitSet[i].fromAnchor)
        {
            if(LimitSet[i].onXAxis)
                iLimitRatio = (float)(LimitSet[i].sourceAnchor->getX() / getParentWidth());
            else
                iLimitRatio = (float)(LimitSet[i].sourceAnchor->getY() / getParentHeight());
        }
        else
            iLimitRatio = LimitSet[i].fLimit;
        //assigning iCurrentRatio based on the limit's axis
        if(LimitSet[i].onXAxis)
            iCurrentRatio = getXRelative();
        else
            iCurrentRatio = getYRelative();
        //determining how to check the limit
        limitType typeToCheck = LimitSet[i].type;
        //switch through the type enum...
        // todo: put a switch statement that moves the anchor here
        switch(typeToCheck)
        {
            case floor:
            {
                if(iCurrentRatio <= iLimitRatio)
                {
                    if(LimitSet[i].onXAxis)
                        setXRelative(iLimitRatio);
                    else
                        setYRelative(iLimitRatio);
                }
                break;
            }
            case ceiling:
            {
              if(iCurrentRatio >= iLimitRatio)
                {
                    if(LimitSet[i].onXAxis)
                        setXRelative(iLimitRatio);
                    else
                        setYRelative(iLimitRatio);
                }
            }
                break;
        }
    }
}
