/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "myVectorKnob.hpp"
#include "customKnobContainer.h"
#include "vectorScreenDisplay.h"
//==============================================================================
/**
*/
class TinyUIAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                    public juce::Timer
{
public:
    TinyUIAudioProcessorEditor (TinyUIAudioProcessor&);
    ~TinyUIAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
    void paintUI(juce::Graphics&);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TinyUIAudioProcessor& audioProcessor;
    
    
    std::unique_ptr<juce::Drawable> myScreen = juce::Drawable::createFromImageData(BinaryData::bg_svg,
                                                                                   BinaryData::bg_svgSize);
    std::unique_ptr<juce::Drawable> myOverlay = juce::Drawable::createFromImageData(BinaryData::screenOverlay_svg,
                                                                                   BinaryData::screenOverlay_svgSize);
    
    
    MyVectorKnob knobTest{0, 270, 1.0, 0.5, audioProcessor.apvts.getParameter("THRESHOLD"), ParameterType::DB, 1.0, "Threshold"};
    MyVectorKnob ratioKnob{90, 270, 1.0, 0.5, audioProcessor.apvts.getParameter("RATIO"), ParameterType::RATIO, 8.0, "Ratio"};
    MyVectorKnob attackKnob{180, 270, 1.0, 0.5, audioProcessor.apvts.getParameter("ATTACK"), ParameterType::MS, 100.0, "Attack"};
    MyVectorKnob releaseKnob{270, 270, 1.0, 0.5, audioProcessor.apvts.getParameter("RELEASE"), ParameterType::MS, 100.0, "Release"};
    
    VectorScreenDisplay scrn{0, 0, 0.5};
    std::vector<juce::RangedAudioParameter*> screenParams;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TinyUIAudioProcessorEditor)
};
