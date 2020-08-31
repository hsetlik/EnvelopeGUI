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
    float aspectRatio = getParentWidth() / getParentHeight();
    setBoundsRelative(fXpos, fYpos, (int)(fWidth + aspectRatio), fWidth);
}

void AnchorPoint::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white);
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
    dragger.dragComponent(this, event, nullptr);
    printf("anchor x : %d\n", getX());
    printf("anchor y : %d\n", getY());
    //checkStaticLimits();
    //checkAnchorLimits();
}

int AnchorPoint::getCenterX()
{
    return getScreenX() + (getWidth() / 2);
}

int AnchorPoint::getCenterY()
{
    return getScreenY() + (getWidth() / 2);
}
//=============================================================


Anchor::Anchor(float pX, float pY, float pW) : child(pX, pY, pW)
{
    setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
    addAndMakeVisible(child);
    pChild = &child.pConstrainer;
    conRect.setSize(child.getWidth(), child.getWidth());
    printf("conRect x: %d\n", conRect.getX());
    printf("conRect y: %d\n", conRect.getY());
    child.addComponentListener(this);
}

void Anchor::paint(juce::Graphics &g)
{
    child.repaint();
    g.setColour(juce::Colours::white);
    g.drawRect(conRect, 1);
}

void Anchor::componentMovedOrResized(juce::Component &component, bool wasMoved, bool wasResized)
{
    conRect.setCentre(component.getX(),component.getY());
}

