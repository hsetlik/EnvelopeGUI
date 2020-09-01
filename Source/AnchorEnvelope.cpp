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

void AnchorPoint::addLimit(float value, limitType type)
{
    Limit newLim;
    newLim.lVal = value;
    newLim.lType = type;
    newLim.fromAnchor = false;
    limitSet.push_back(newLim);
}

void AnchorPoint::addLimit(AnchorPoint *source, limitType type)
{
    Limit newLim;
    newLim.lType = type;
    if(type == xFloor || type == xCeiling)
        newLim.pVal = &source->fXpos;
    else
        newLim.pVal = &source->fYpos;
    newLim.lVal = *newLim.pVal;
    limitSet.push_back(newLim);
}

void AnchorPoint::conformToPeerBounds()
{
    constrainer.setSizeLimits(conRect.getX(),
                              conRect.getY(),
                              conRect.getX() + conRect.getWidth(),
                              conRect.getY() + conRect.getHeight());
    constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
}

//=============================================================

AnchorRect::AnchorRect(float afX, float afY, float afW, float afH, AnchorPoint* anchor)
{
    fX = afX;
    fY = afY;
    fW = afW;
    fH = afH;
    
    
    rLimits = {fX, fY, fW - fX, fH - fY};
    
    peerAnchor = anchor;
    peerAnchor->addComponentListener(this);

}

void AnchorRect::initPosition()
{
    setBoundsRelative(rLimits);
}

void AnchorRect::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::pink);
    g.fillAll();
    g.setColour(juce::Colours::green);
    g.fillRect(tempBounds);
}

void AnchorRect::checkPeerLimits()
{
    float xMin = 0.0f, yMin = 0.0f, xMax = 1.0f, yMax = 1.0f;
    float fWidth = peerAnchor->getWidth() * (1.0f / peerAnchor->getParentWidth());
    float fHeight = peerAnchor->getHeight() * (1.0f / peerAnchor->getParentHeight());
    for(int i = 0; i < peerAnchor->limitSet.size(); ++i)
    {
        AnchorPoint::limitType thisType = peerAnchor->limitSet[i].lType;
        float thisLim = peerAnchor->limitSet[i].lVal;
        switch(thisType)
        {
            case AnchorPoint::xFloor:
            {
             if(thisLim > xMin)
                 xMin = thisLim;
            }
            case AnchorPoint::yFloor:
            {
                if(thisLim > yMin)
                    yMin = thisLim;
            }
            case AnchorPoint::xCeiling:
            {
                if(thisLim < (xMax + fWidth))
                    xMax = thisLim;
            }
            case AnchorPoint::yCeiling:
            {
                if(thisLim < (yMax + fHeight))
                    yMax = thisLim;
            }
        }
    }
    int newX, newY, newRight, newBottom;
    newX = xMin * getParentWidth();
    newY = yMin * getParentHeight();
    newRight = (xMax * getParentWidth());
    newBottom = (yMax * getParentHeight());
    //setBoundsRelative(xMin, yMin, xMax - xMin, yMax - yMin);
    tempBounds = peerAnchor->getLocalBounds().reduced(5);
    
    int oldWidth = peerAnchor->conRect.getWidth();
    int oldHeight = peerAnchor->conRect.getHeight();
    int oldX = peerAnchor->conRect.getX();
    int oldY = peerAnchor->conRect.getY();
    int oldRight = oldX + oldWidth;
    int oldBottom = oldY + oldHeight;
    
    int deltaX = newX;
    int deltaY = newY;
    int deltaRight = (getParentWidth() - newRight);
    int deltaBottom = (getParentHeight() - newBottom);
    
    tempBounds.setLeft(oldX - deltaX);
    tempBounds.setTop(oldY - deltaY);
    tempBounds.setRight(oldRight + deltaRight);
    tempBounds.setBottom(oldBottom + deltaBottom);
    
    peerAnchor->conRect = tempBounds;
    peerAnchor->conformToPeerBounds();
}

void AnchorRect::componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized)
{
    checkPeerLimits();
}
