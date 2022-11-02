//
//  myVectorKnob.cpp
//  tinyUI
//
//  Created by Alec Denny on 10/1/22.
//

#include "myVectorKnob.hpp"




MyVectorKnob::MyVectorKnob(int offsx, int offsy, float scale, float initValue, juce::RangedAudioParameter* pFloat, ParameterType p, float pScale, juce::String label)
: scale{scale}, value{initValue}, knobType{p}, label{label}
{
    knob = juce::Drawable::createFromImageData(BinaryData::knobsvg_svg, BinaryData::knobsvg_svgSize);
    indicator = juce::Drawable::createFromImageData(BinaryData::knobIndicator_svg, BinaryData::knobIndicator_svgSize);
    
    
    boundingBox = juce::Rectangle<int>(offsx + 30, offsy + 20, 74, 74);
    //updateTransform(initValue);
    placeKnob = juce::AffineTransform::scale(scale, scale);
    placeKnob = placeKnob.followedBy(juce::AffineTransform::translation(offsx, offsy));
    
    updateTransform(initValue);
    
    //i think this is actually how all of my file inclusions need to be generated!
    myFont  = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::ArialRoundedBold_ttf,
                                                                 BinaryData::ArialRoundedBold_ttfSize));
    
    attach = std::make_unique<juce::ParameterAttachment>(*pFloat, [this](auto x) {this->setValue(x);}, nullptr);
    attach->sendInitialUpdate();
}



//since we do not pass the graphics context in, and I dont think we want to.
//any "drawing" is just creating  a transform for when paint() is called

void MyVectorKnob::updateTransform(float val)
{
    //there is a distinct possibility i found an issue with the svg file, after coding all this that would make
    //simply adding a rotation transfomation with the CORRECT offset would work now :/
    //converting to svg changed where the rotation center point was for whatever reason
    //so i am doing all this geometry stuff because the center of the knob is not the center of the main knob svg
    
    auto theta = val * 6.28;
    auto displayAngle = (val + 0.25) * 6.28;
    //this should actually be ["radius" - (scale * indicatorsize) / 2] * scale
    //(need to re do a lot to make fully scalable gui)
    auto offsx = std::cos(displayAngle) * mainKnob.indicatorDrawDiameter * scale;
    auto offsy = std::sin(displayAngle) * mainKnob.indicatorDrawDiameter * scale;
    auto centerX = indicator->getWidth() / 2.0;
    auto centerY = indicator->getHeight() / 2.0;
    
    placeIndicator = juce::AffineTransform::rotation(theta, centerX * scale, centerY * scale);
    
    juce::Rectangle<float> bounds = indicator->getDrawableBounds();
    bounds = bounds.transformedBy(placeIndicator);
    
    centerX = bounds.getCentreX();
    centerY = bounds.getCentreY();
    
    //formating here is absolutely vile but it works
    //we are turning the little indicator and placing it at the correct angle around the circle
    
    placeIndicator = placeIndicator.followedBy(juce::AffineTransform::translation(
                                                                                  (mainKnob.indicatorXoffset * scale) + offsx - centerX
                                                                                  , (mainKnob.indicatorYoffset * scale) + offsy - centerY
                                                                                  ));
    //then translate to the position of the knob
    placeIndicator = placeIndicator.followedBy(placeKnob);
    mouseOver = false;
}




void MyVectorKnob::paint(juce::Graphics& g)
{
    knob->draw(g, 1.0, placeKnob);
    indicator->draw(g, 1.0, placeIndicator);
    g.setFont(myFont);
    //g.setColour(juce::Colour(14, 94, 0));
    //g.drawText(label, boundingBox.translated(0, -18), juce::Justification::centredTop, false);
    if(mouseOver || dragging)
    {
        g.setFont(myFont);
        g.setColour(juce::Colour(255, 182, 122));
        
        std::ostringstream ss;
        
        float x{value};
        if(knobType == ParameterType::DB)
        {
            //
            //this scales to linear dB on the knob but should probably be limited to go down to like
            //somewhere between -40 and -80 otherwise its jst crazy sounding
            //not sure if the attack and release should have a similar thing
            //i.e. more detail at lower (1-10ms) range and also not able to go down to 0ms (they dont actually anyways its 1 sample min)
            //
            x = pow(x, 2);
            if(x <= 0.000001)
                x = -120.0;
            else
                x = 20.0 * log10(abs(x));
            ss << static_cast<int>(x) << " db";
        }
        else if (knobType == ParameterType::RATIO)
        {
            ss << static_cast<int>((x * 19.0) + 1) << ":1";
        }
        else if (knobType == ParameterType::MS)
        {
            ss << static_cast<int>(x * 100.0) << " ms";
        }
        else
        {
            ss << value;
        }
        g.drawText(ss.str(), boundingBox, 4, false);
    }
    else
    {
        g.setColour(juce::Colour(255, 182, 122));
        g.drawText(label, boundingBox, 4, false);
    }
}

void MyVectorKnob::mouseEnter(const juce::MouseEvent &event)
{
    mouseOver = true;
}
void MyVectorKnob::mouseExit(const juce::MouseEvent &event)
{
    mouseOver = false;
}

void MyVectorKnob::mouseUp(const juce::MouseEvent &event)
{
    attach->endGesture();
    dragging = false;
}

void MyVectorKnob::mouseDrag(const juce::MouseEvent &event)
{
    
    dragStart = event.getMouseDownY();
    
    auto drag_y = event.getDistanceFromDragStartY();
    if(drag_y == 0)
    {
        prevY = dragStart;
        attach->beginGesture();
        dragging = true;
    }
    
    auto deltaY = event.y - prevY;
    
    if(value >= 0.0 && value <= 1.0)
    {
        value -= deltaY / 100.0;
        
    }
    if(value >= 1.0)
        value = 1.0;
    else if(value <= 0.0)
        value = 0.0;
    
    attach->setValueAsPartOfGesture(value);
    prevY = event.y;

}


float MyVectorKnob::getValue()
{
    return value;
}

void MyVectorKnob::setValue(float val)
{
    value = val;
    updateTransform(val);
}


