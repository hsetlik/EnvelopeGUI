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

void AnchorPoint::checkStaticLimits()
{
    for(int i = 0; i < limitSetStatic.size(); ++i)
    {
        float fLimit = limitSetStatic[i].lVal;
        float fCheck;
        if(limitSetStatic[i].lAxis == x)
            fCheck = fXpos;
        else
            fCheck = fYpos;
        bool crossesLimit = false;
        if(limitSetStatic[i].lType == floor && fCheck < fLimit)
            crossesLimit = true;
        else if(limitSetStatic[i].lType == ceiling && fCheck > fLimit)
            crossesLimit = true;
        if(crossesLimit && limitSetStatic[i].lAxis == x)
            printf("fLimit is %f, ready to constrain X\n", fLimit);
            //constrainXTo(fLimit);
        if(crossesLimit && limitSetStatic[i].lAxis == y)
            constrainYTo(fLimit);
    }
}

void AnchorPoint::checkAnchorLimits()
{
    for(int i = 0; i < limitSetAnchor.size(); ++i)
    {
        float fLimit = *limitSetAnchor[i].pVal;
        float fCheck;
        if(limitSetAnchor[i].lAxis == x)
            fCheck = fXpos;
        else
            fCheck = fYpos;
        bool crossesLimit = false;
        if(limitSetAnchor[i].lType == floor && fCheck < fLimit)
            crossesLimit = true;
        else if(limitSetAnchor[i].lType == ceiling && fCheck > fLimit)
            crossesLimit = true;
        if(crossesLimit && limitSetAnchor[i].lAxis == x)
            constrainXTo(fLimit);
        if(crossesLimit && limitSetAnchor[i].lAxis == y)
            constrainYTo(fLimit);
    }
}

void AnchorPoint::constrainXTo(float xLim)
{
    printf("X limit triggered\n");
    fXpos = xLim;
}
void AnchorPoint::constrainYTo(float yLim)
{
    fYpos = yLim;
    setToRelativeBounds();
}

