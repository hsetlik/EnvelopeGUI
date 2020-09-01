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
    juce::Rectangle<float> anchorBounds = juce::Rectangle<float>(0.0, 0.0, getParentHeight() * fHeight,
                                                                 getParentHeight() * fHeight);
    g.setColour(anchorColor);
    g.fillEllipse(anchorBounds);
}

int AnchorPoint::centerScreenX()
{
    int leftX = getX();
    return leftX + (getWidth() / 2);
}

int AnchorPoint::centerScreenY()
{
    int topY = getY();
    return topY + (getHeight() / 2);
}

void AnchorPoint::mouseDown(const juce::MouseEvent &event)
{
    dragger.startDraggingComponent(this, event);
}

void AnchorPoint::mouseDrag(const juce::MouseEvent &event)
{
    dragger.dragComponent(this, event, &constrainer);
    //printf("anchor x : %d\n", getX());
    //printf("anchor y : %d\n", getY());
   checkLimits();
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
    newLim.fromAnchor = true;
    newLim.lType = type;
    if(type == xFloor || type == xCeiling)
        newLim.pVal = &source->fXpos;
    else
        newLim.pVal = &source->fYpos;
    newLim.lVal = *newLim.pVal;
    limitSet.push_back(newLim);
}

void AnchorPoint::checkLimits()
{
    for(int i = 0; i < limitSet.size(); ++i)
    {
        if(limitSet[i].fromAnchor)
            limitSet[i].lVal = *limitSet[i].pVal;
        juce::Rectangle<int> tempAnchor = getBoundsInParent();
        printf("Parent Width: %d\n", getParentWidth());
        printf("Parent Height: %d\n", getParentHeight());
        int anchorX = tempAnchor.getX();
        int anchorW = getParentWidth();
        float fX = anchorX * (1.0f / anchorW);
        printf("fX: %f\n", fX);
        int anchorY = tempAnchor.getY();
        int anchorH = getParentHeight();
        float fY = anchorY * (1.0f / anchorH);
        juce::Rectangle<int> tempRect = conRect;
        switch(limitSet[i].lType)
        {
            case xFloor:
            {
                if(fX < limitSet[i].lVal)
                {
                    printf("hit xFloor\n");
                    float offsetX = getParentWidth() * (1.0 - limitSet[i].lVal);
                    int newX = conRect.getX() + offsetX;
                    printf("new x: %d\n", newX);
                    tempRect.setX(newX);
                }
                break;
            }
            case yFloor:
            {
                if(fY < limitSet[i].lVal)
                {
                    printf("hit Y floor\n");
                    float offsetY = getParentHeight() * (1.0 - limitSet[i].lVal);
                    int newY = conRect.getY() + offsetY;
                    tempRect.setY(newY);
                }
                break;
                
            }
            case xCeiling:
            {
                if(fX > limitSet[i].lVal)
                {
                    printf("hit x ceiling\n");
                    float offsetX = getParentWidth() * (1.0 - limitSet[i].lVal);
                    int newRight = conRect.getRight() + offsetX;
                    tempRect.setRight(newRight);
                }
                break;
                
            }
            case yCeiling:
            {
                if(fY > limitSet[i].lVal)
                {
                    printf("hit y ceiling\n");
                    float offsetY = getParentHeight() * (1.0 - limitSet[i].lVal);
                    int newBottom = conRect.getBottom() + offsetY;
                    tempRect.setBottom(newBottom);
                }
                break;
            }
        }
        conRect = tempRect;
        constrainer.setSizeLimits(conRect.getX(), conRect.getY(),
                                  conRect.getX() + conRect.getWidth(), conRect.getY() + conRect.getHeight());
        constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
    }
}

