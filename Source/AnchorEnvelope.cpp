//
//  AnchorEnvelope.cpp
//  EnvelopeGUI - App
//
//  Created by Hayden Setlik on 8/30/20.
//

#include "AnchorEnvelope.hpp"

AnchorPoint::AnchorPoint(float pX, float pY, float pW, juce::Component* pTop)
{
    fXpos = pX;
    fYpos = pY;
    fHeight = pW;
    pTopParent = pTop;
    pFirstParent = getParentComponent();
    int tX = pTopParent->getX();
    int tY = pTopParent->getY();
    int tW = pTopParent->getWidth();
    int tH = pTopParent->getHeight();
    int netX = (int)(fXpos * tX);
    int netY = (int)(fYpos * tY);
    int netW = (int)(fHeight * tW);
    int netH = (int)(fHeight * tH);
    setBounds(netX, netY, netW, netH);
    printf("fWidth: %f\n", fWidth);
    printf("fHeight: %f\n", fHeight);
    
}

void AnchorPoint::reInitWithParent()
{
    int tX = pTopParent->getX();
    int tY = pTopParent->getY();
    int tW = pTopParent->getWidth();
    int tH = pTopParent->getHeight();
    int netX = (int)(fXpos * tX);
    int netY = (int)(fYpos * tY);
    int netW = (int)(fHeight * tW);
    int netH = (int)(fHeight * tH);
    setBounds(netX, netY, netW, netH);
    conRect = pTopParent->getBounds().reduced(5);
    constrainer.setSizeLimits(conRect.getX(),
                              conRect.getY(),
                              conRect.getX() + conRect.getWidth(),
                              conRect.getY() + conRect.getHeight());
    constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
}
void AnchorPoint::setToRelativeBounds()
{
    fWidth = (fHeight * pTopParent->getHeight()) / pTopParent->getWidth();
    setBounds(fXpos * pTopParent->getX(),
              fYpos * pTopParent->getY(),
              fWidth * pTopParent->getWidth(),
              fHeight * pTopParent->getHeight());
}

void AnchorPoint::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::white);
    g.fillAll();
    juce::Rectangle<float> anchorBounds = juce::Rectangle<float>(0.0, 0.0, (pTopParent->getHeight()) * fHeight, (pTopParent->getHeight()) * fHeight);
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
AnchorBox::AnchorBox(float fX, float fY, float fW, float fH, float pX, float pY, float pW, juce::Component* pTop)
                                                    : child(pX, pY, pW, pTop)
{
    setBoundsRelative(fX, fY, fW, fH);
    addAndMakeVisible(child);
    
    child.reInitWithParent();
}
