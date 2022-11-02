//
//  myVectorKnob.hpp
//  tinyUI
//
//  Created by Alec Denny on 10/1/22.
//

#pragma once
#include <JuceHeader.h>


#ifndef myVectorKnob_hpp
#define myVectorKnob_hpp
#include <sstream>
//#include <stdio.h>


enum ParameterType
{
    DB,
    RATIO,
    MS,
    LITERAL,
    PARAMETER_TYPES
};

//i think we need to use the "ParameterAttachment" class at some point w the AVTS

class MyVectorKnob : public juce::Component
                    
{
    
public:
    MyVectorKnob(int offsx, int offsy, float scale, float initValue, juce::RangedAudioParameter* pFloat, ParameterType p, float pScale, juce::String label);
    
    ~MyVectorKnob(){};
    
    virtual void paint(juce::Graphics& g) override;
    
    juce::Rectangle<int> boundingBox;
    float getValue();
    void setValue(float val);
    
    
private:
    // should use circle diameter? i am actually a little confused
    struct svgKnobInfo
    {
        float indicatorXoffset      {67.0};
        float indicatorYoffset      {56.6};
        float indicatorDrawDiameter {34.0};
        float indicatorSize         {6.0};
        float centerKnobOffsetX     {30.0};
        float centerKnobOffsetY     {20.0};
        std::string knobPath {"/Users/alecdenny/Documents/ad_dev/tinyUI/Source/knobsvg.svg"};
        std::string indicatorPath {"/Users/alecdenny/Documents/ad_dev/tinyUI/Source/knobIndicator.svg"};
    
    } mainKnob;
    

    
    void updateTransform(float val);
    void mouseEnter(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    
    void updateSuffix(ParameterType p);
    
    juce::File knob_file{mainKnob.knobPath};
    juce::File indicator_file{mainKnob.indicatorPath};
    
    
    std::unique_ptr<juce::Drawable> knob;
    std::unique_ptr<juce::Drawable> indicator;
    
    //store a reference to the audio parameter we are using somehow or something
    
    //this is just translation
    juce::AffineTransform placeKnob;
    
    //this may end up changing every time  it gets used so idk if it is necessary
    //should be a translation then rotation or vis versa i guess
    juce::AffineTransform placeIndicator;
    float scale;
    bool mouseOver;
    bool dragging;
    float value;
    int prevY;
    int dragStart;
    juce::Font myFont;
    juce::String label;


    ParameterType knobType;
    
    
    
    std::unique_ptr<juce::ParameterAttachment> attach;


};
    
  






#endif /* myVectorKnob_hpp */
