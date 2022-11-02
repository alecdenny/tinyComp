/*
  ==============================================================================

    vectorScreenDisplay.h
    Created: 5 Oct 2022 1:36:27pm
    Author:  Alec Denny

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class VectorScreenDisplay : public juce::Component

{
    /* controls parameters:
     * in gain, out gain, mix
     *
     * displays parameters:
     * -on Mouse Over: in gain, out gain, mix
     * -default: threshold/ratio
     *
     *
     */
public:
    VectorScreenDisplay(
                        int offsx,
                        int offsy,
                        float scale);
    
    ~VectorScreenDisplay(){};
    
    void prepParams(std::vector<juce::RangedAudioParameter*>& parameterPtrs);
    
    
    virtual void paint(juce::Graphics& g) override;
    
    void updateThresholdRatioAttenuation(float t, float r, float a);
    void updateTransform();
    
    
    void mouseEnter(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    //void mouseMove(const juce::MouseEvent &event) override;
    
    juce::Rectangle<int> boundingBox;
    juce::Rectangle<int> mouseOverBox;
    
    
    
private:
    
    struct screenInfo
    {
        float overlayOffsX      {43};
        float overlayOffsY      {25};
        float greenWidth        {570.0};
        float greenHeight       {570.0};
        float screenWidth       {497.44};
        float screenHeight      {497.44};
        int centerXoffset       {57};
        
        juce::Colour drawColor  {0, 63, 84};
        
    } mainScreen;
    
    enum screenParameters
    {
        IN_GAIN,
        OUT_GAIN,
        MIX,
        PARAMETERS
    };
    
    struct sliderStyleInfo
    {
        int vSliderHeight   {100};
        int vSliderWidth    {30};
        int hSliderWidth    {120};
        int hSliderHeight   {30};
        int fontSize        {12};
    }   iomInfo;
    
    void drawTRA(juce::Graphics& g);
    void drawIOM(juce::Graphics& g);
    
    int getMouseFocus(const juce::MouseEvent& event);
    
    
    juce::AffineTransform placeScreen;
    juce::AffineTransform placeOverlay;
    juce::AffineTransform placeOutline;
    
    juce::Font myFont  = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::ArialRoundedBold_ttf,
                                                                 BinaryData::ArialRoundedBold_ttfSize));
    
    
    std::unique_ptr<juce::Drawable> myScreen = juce::Drawable::createFromImageData(BinaryData::bg_svg,
                                                                                   BinaryData::bg_svgSize);
    std::unique_ptr<juce::Drawable> myOverlay = juce::Drawable::createFromImageData(BinaryData::screenOverlay_svg,
                                                                                   BinaryData::screenOverlay_svgSize);
    std::vector<std::unique_ptr<juce::ParameterAttachment>> attach;
    
    std::array<float, screenParameters::PARAMETERS> values{0.0, 0.0, 0.0};
    float threshold;
    float ratio;
    float attenuation;
    
    bool mouseOver;
    void setValue(float x, screenParameters y);
    
    juce::Rectangle<int> inSlider   {0, 0, iomInfo.vSliderWidth, iomInfo.vSliderHeight};
    juce::Rectangle<int> outSlider  {0, 0, iomInfo.vSliderWidth, iomInfo.vSliderHeight};
    juce::Rectangle<int> mixSlider  {0, 0, iomInfo.hSliderWidth, iomInfo.hSliderHeight};
    
    //juce::Rectangle<int> vSliderText{0, 0, iomInfo.vSliderWidth, iomInfo.vSliderHeight/6};
    juce::Rectangle<int> hSliderText{0, 0, iomInfo.hSliderWidth, iomInfo.hSliderHeight};    //justify text center
    
    juce::Rectangle<int> inSliderBounds {50 + mainScreen.centerXoffset, 50,  iomInfo.vSliderWidth, iomInfo.vSliderHeight};
    juce::Rectangle<int> outSliderBounds {200 + mainScreen.centerXoffset, 50,  iomInfo.vSliderWidth, iomInfo.vSliderHeight};
    juce::Rectangle<int> mixSliderBounds {75 + mainScreen.centerXoffset, 180, iomInfo.hSliderWidth, iomInfo.hSliderHeight};
    
    screenParameters focus;
    
    float prevY;
    bool dragging;
    float dragStart;
    
};
