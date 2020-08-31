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
}

void AnchorPoint::setToRelativeBounds()
{
    setBoundsRelative(fXpos, fYpos, fWidth, fWidth);
}

void AnchorPoint::paint(juce::Graphics &g)
{
    juce::Rectangle<float> anchorBounds = juce::Rectangle<float>(0.0, 0.0, getParentHeight() * fWidth, getParentHeight() * fWidth);
    printf("anchor x : %f\n", anchorBounds.getX());
    printf("anchor y : %f\n", anchorBounds.getY());
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
}
