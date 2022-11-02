/*
  ==============================================================================

    vectorScreenDisplay.cpp
    Created: 5 Oct 2022 1:36:27pm
    Author:  Alec Denny

  ==============================================================================
*/

#include "vectorScreenDisplay.h"

VectorScreenDisplay::VectorScreenDisplay(int offsx, int offsy, float scale)
{
    
    boundingBox = juce::Rectangle<int>(0, 0, 285, 285);
    mouseOverBox = juce::Rectangle<int>((37 * 0.5) + mainScreen.centerXoffset, 20 * 0.5, mainScreen.screenWidth * 0.5, mainScreen.screenHeight * 0.5);
    
    
    
    updateTransform();
    
    
}

void VectorScreenDisplay::prepParams(std::vector<juce::RangedAudioParameter*>& parameterPtrs)
{
    attach.push_back(std::make_unique<juce::ParameterAttachment>(*parameterPtrs[0], [this](auto x) {this->setValue(x, screenParameters::IN_GAIN);}, nullptr));
    
    attach.push_back(std::make_unique<juce::ParameterAttachment>(*parameterPtrs[1], [this](auto x) {this->setValue(x, screenParameters::OUT_GAIN);}, nullptr));
    
    attach.push_back(std::make_unique<juce::ParameterAttachment>(*parameterPtrs[2], [this](auto x) {this->setValue(x, screenParameters::MIX);}, nullptr));
    for(int a = 0; a < attach.size(); ++a)
        attach[a]->sendInitialUpdate();
}


void VectorScreenDisplay::updateTransform()
{
    placeScreen = juce::AffineTransform::scale(0.5, 0.5).followedBy(juce::AffineTransform::translation( mainScreen.centerXoffset, 0));
    placeOverlay = juce::AffineTransform::translation(mainScreen.overlayOffsX, mainScreen.overlayOffsY).
                            followedBy(juce::AffineTransform::scale(0.5, 0.5));
    placeOverlay = placeOverlay.followedBy(juce::AffineTransform::translation(mainScreen.centerXoffset, 0));
    
    placeOutline = juce::AffineTransform::scale(0.8, 0.8);
    
    auto xoffs = boundingBox.getCentreX() - (boundingBox.transformedBy(placeOutline).getWidth() / 2.0);
    auto yoffs = boundingBox.getCentreY() - ((boundingBox.transformedBy(placeOutline).getHeight() / 2.0) + 10.0);
    
    
    placeOutline = placeOutline.followedBy(juce::AffineTransform::translation(xoffs + mainScreen.centerXoffset, yoffs));
    
}

void VectorScreenDisplay::paint(juce::Graphics& g)
{
    myScreen->draw(g, 1.0, placeScreen);
    
    auto mPos = getMouseXYRelative();
    
    if(mouseOverBox.contains(mPos)  || dragging)
    {
        drawIOM(g);
    }
    else
    {
        drawTRA(g);
        myOverlay->draw(g, 1.0, placeOverlay);
    }
}

void VectorScreenDisplay::updateThresholdRatioAttenuation(float t, float r, float a)
{
    threshold = t;
    ratio = r;
    attenuation = a;
    updateTransform();
}

void VectorScreenDisplay::setValue(float x, screenParameters y)
{
    values[y] = x;
    updateTransform();
}


void VectorScreenDisplay::mouseEnter(const juce::MouseEvent &event)
{
    mouseOver = true;
}

void VectorScreenDisplay::mouseExit(const juce::MouseEvent &event)
{
    if(!mouseOverBox.contains(event.getPosition()))
        if(!dragging)
            mouseOver = false;
}

void VectorScreenDisplay::drawTRA(juce::Graphics& g)
{
    g.setColour(mainScreen.drawColor);
    
    auto subscrnsize = boundingBox.transformedBy(placeOutline);
    
    g.drawRect(subscrnsize, 10);
    
    //draw threshold line
    auto x1 = subscrnsize.getX();
    auto y1 = subscrnsize.getBottom();
    auto x2 = x1 + (subscrnsize.getWidth() * threshold);
    auto y2 = y1 - (subscrnsize.getHeight() * threshold);
    g.drawLine(x1 + 5, y1 - 5, x2, y2, 5);
    
    //drawRatioLine
    
    auto slope = 1.0 / ((ratio * 19.0) + 1.0);
    auto rx1 = x2;
    auto ry1 = y2;
    auto len = subscrnsize.getRight() - x2 - 10;
    auto rx2 = subscrnsize.getRight();
    auto ry2 = y2 - (len * slope);
    
    g.drawLine(rx1, ry1, rx2, ry2, 5);
    
    //draw attenuation amount
    g.drawRect(100, 30, attenuation * 80.0, 10);
}

void VectorScreenDisplay::drawIOM(juce::Graphics& g)
{
    
    g.setColour(mainScreen.drawColor);
    //draw in gain as rect
    
    auto inPlace = juce::AffineTransform::translation(inSliderBounds.getX(), inSliderBounds.getY());
    
    g.drawRect(inSlider.transformedBy(inPlace));
    
    auto iheight = -(values[screenParameters::IN_GAIN] - 0.5);

    auto insliderdisp = inSlider.transformedBy(juce::AffineTransform::scale(1.0, iheight));
    insliderdisp = insliderdisp.transformedBy(juce::AffineTransform::translation(0, inSlider.getCentreY()));
    insliderdisp = insliderdisp.transformedBy(inPlace);
    g.fillRect(insliderdisp);
    
    g.setFont(iomInfo.fontSize);  //* scale
    
    g.drawText("+6dB", inSliderBounds.getRight()
               , inSliderBounds.getY()
               , iomInfo.vSliderWidth
               , iomInfo.vSliderHeight / 6.0
               , juce::Justification::topLeft
               , false);
    
    g.drawText("0dB"
               , inSliderBounds.getRight()
               , inSliderBounds.getCentreY() - (iomInfo.fontSize / 2)
               , iomInfo.vSliderWidth
               , iomInfo.vSliderHeight / 6.0
               , juce::Justification::centredLeft
               , false);
    
    g.drawText("-6dB"
               , inSliderBounds.getRight()
               , inSliderBounds.getBottom() - iomInfo.fontSize
               , iomInfo.vSliderWidth
               , iomInfo.vSliderHeight / 6.0
               , juce::Justification::bottomLeft
               , false);
    
    //draw out gain as rect
    
    auto outPlace = juce::AffineTransform::translation(outSliderBounds.getX(), outSliderBounds.getY());
    
    g.drawRect(outSlider.transformedBy(outPlace));
    
    auto oheight = -(values[screenParameters::OUT_GAIN] - 0.5);
    auto outsliderdisp = outSlider.transformedBy(juce::AffineTransform::scale(1.0, oheight));
    outsliderdisp = outsliderdisp.transformedBy(juce::AffineTransform::translation(0,outSlider.getCentreY()));
    outsliderdisp = outsliderdisp.transformedBy(outPlace);
    g.fillRect(outsliderdisp);
    
    float textW = 30.0;
    g.drawText("+6dB"
               , outSliderBounds.getX() - textW, outSliderBounds.getY()
               , iomInfo.vSliderWidth, iomInfo.vSliderHeight / 6.0
               , juce::Justification::topLeft
               , false);
    
    g.drawText("0dB"
               , outSliderBounds.getX() - textW
               , outSliderBounds.getCentreY() - (iomInfo.fontSize / 2)
               , iomInfo.vSliderWidth, iomInfo.vSliderHeight / 6.0
               , juce::Justification::centredLeft
               , false);
    
    g.drawText("-6dB"
               , outSliderBounds.getX() - textW, outSliderBounds.getBottom() - iomInfo.fontSize
               , iomInfo.vSliderWidth
               , iomInfo.vSliderHeight / 6.0
               , juce::Justification::bottomLeft
               , false);
    
    //draw mix as rect horizontally
    
    auto mixPlace = juce::AffineTransform::translation(mixSliderBounds.getX(), mixSliderBounds.getY());
    
    g.drawRect(mixSlider.transformedBy(mixPlace));
    
    auto mwidth = values[screenParameters::MIX];
    auto mixsliderdisp = mixSlider.transformedBy(juce::AffineTransform::scale(mwidth, 1.0));
    mixsliderdisp = mixsliderdisp.transformedBy(mixPlace);
    g.fillRect(mixsliderdisp);
    
    /*
     * we have a relatively similar thing happen for each slider
     * so this is a pretty good candidate for future encapsulation however
     * considering there's only 3, and the alignment of text is different for each
     * its sort of an undertaking of its own
     *
     */
    if(focus == screenParameters::IN_GAIN && dragging)
    {
        float db = (values[0] - 0.5) * 12.0;

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << db;
        
        auto y = (iheight * iomInfo.vSliderHeight * (1.0 + (iomInfo.fontSize / iomInfo.vSliderHeight))) + inSliderBounds.getCentreY();
        
        g.setColour(juce::Colour(128, 185, 185));
        
        g.fillEllipse(inSliderBounds.getCentreX() - (iomInfo.vSliderWidth * 0.75), y - (iomInfo.fontSize * 0.6)
                      , iomInfo.vSliderWidth * 1.5
                      , (iomInfo.fontSize * 1.2));
        
        g.setColour(mainScreen.drawColor);
        
        g.drawEllipse(inSliderBounds.getCentreX() - (iomInfo.vSliderWidth * 0.75), y - (iomInfo.fontSize * 0.6)
                      , iomInfo.vSliderWidth * 1.5
                      , (iomInfo.fontSize * 1.2), 1.0);
        
        g.drawText(ss.str()
                   , inSliderBounds.getCentreX() - (iomInfo.vSliderWidth * 0.75)
                   , y - (iomInfo.fontSize * 0.6)
                   , iomInfo.vSliderWidth * 1.5
                   , (iomInfo.fontSize * 1.2)
                   , juce::Justification::centred
                   , false);
    }
    else if(focus == screenParameters::OUT_GAIN && dragging)
    {
        float db = (values[1] - 0.5) * 12.0;

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << db;
        
        auto y = (oheight * iomInfo.vSliderHeight * (1.0 + (iomInfo.fontSize / iomInfo.vSliderHeight))) + outSliderBounds.getCentreY();
        
        g.setColour(juce::Colour(128, 185, 185));
        
        g.fillEllipse(outSliderBounds.getCentreX() - (iomInfo.vSliderWidth * 0.75)
                      , y - (iomInfo.fontSize * 0.6)
                      , iomInfo.vSliderWidth * 1.5
                      , (iomInfo.fontSize * 1.2));
        
        g.setColour(mainScreen.drawColor);
        
        g.drawEllipse(outSliderBounds.getCentreX() - (iomInfo.vSliderWidth * 0.75)
                      , y - (iomInfo.fontSize * 0.6)
                      , iomInfo.vSliderWidth * 1.5
                      , (iomInfo.fontSize * 1.2), 1.0);
        
        g.drawText(ss.str()
                   , outSliderBounds.getCentreX() - (iomInfo.vSliderWidth * 0.75)
                   , y - (iomInfo.fontSize * 0.6)
                   , iomInfo.vSliderWidth * 1.5
                   , (iomInfo.fontSize * 1.2)
                   , juce::Justification::centred
                   , false);
    }
    else if(focus == screenParameters::MIX && dragging)
    {
        std::ostringstream ss;
        ss << static_cast<int>(values[2] * 100) << "%";
        auto eSize = iomInfo.hSliderHeight * 1.5;
        auto xoffs = mixsliderdisp.getRight() - eSize * 0.5;
        auto yoffs = mixsliderdisp.getCentreY() - eSize * 0.5;
        
        juce::Rectangle<float> mixbox = juce::Rectangle<float>(xoffs, yoffs, eSize, eSize);
        
        g.setColour(juce::Colour(128, 185, 185));
        g.fillEllipse(mixbox);
        g.setColour(mainScreen.drawColor);
        g.drawEllipse(mixbox, 1.0);
        g.drawText(ss.str(), mixbox, juce::Justification::centred, false);
    }
    
    
}

void VectorScreenDisplay::mouseDown(const juce::MouseEvent &event)
{
    //auto x = event.getMouseDownX();
    auto y = event.getMouseDownY();
    
    prevY = y;
    
    int f = getMouseFocus(event);
    
    if(f == -1)
        dragging = false;
    else
    {
        focus = static_cast<screenParameters>(f);
        attach[focus]->beginGesture();
        dragging = true;
        
    }
}

int VectorScreenDisplay::getMouseFocus(const juce::MouseEvent &event)
{
    auto x = event.getMouseDownX();
    auto y = event.getMouseDownY();
    if (inSliderBounds.contains(x, y))
    {
        return screenParameters::IN_GAIN;
    }
    else if (outSliderBounds.contains(x,y))
    {
        return screenParameters::OUT_GAIN;
    }
    else if (mixSliderBounds.contains(x, y))
    {
        return screenParameters::MIX;
    }
    else
    {
        return -1;
    }
}

void VectorScreenDisplay::mouseUp(const juce::MouseEvent &event)
{
    attach[focus]->endGesture();
    if(event.mouseWasClicked())
    {
        //this is a place holder i need my sliders to be a vector of rectangles
        //or maybe realistically
        //structs w some info
        //or maybe very seriously i just need to make the slider have its own
        getMouseFocus(event);
        
        if(focus == screenParameters::IN_GAIN || focus == screenParameters::OUT_GAIN)
            attach[focus]->setValueAsCompleteGesture(0.5);
        else
            attach[focus]->setValueAsCompleteGesture(1.0);
    }
    dragging = false;
}

void VectorScreenDisplay::mouseDrag(const juce::MouseEvent &event)
{
    dragStart = event.getMouseDownY();
    if(dragging)
    {
        //auto drag_y = event.getDistanceFromDragStartY();
   
        auto deltaY = event.y - prevY;
    
    
    
        if(values[focus] >= 0.0 && values[focus] <= 1.0)
        {
            values[focus] -= deltaY / 100.0;
        }
        if(values[focus] >= 1.0)
            values[focus] = 1.0;
        else if(values[focus] <= 0.0)
            values[focus] = 0.0;
        attach[focus]->setValueAsPartOfGesture(values[focus]);
        prevY = event.y;
    }
}



