/*
  ==============================================================================

    customKnobContainer.h
    Created: 3 Oct 2022 12:00:55am
    Author:  Alec Denny

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "myVectorKnob.hpp"

// i think i need to implement the audio value tree state or whatever before i can really start on this
class CustomKnobContainer: public juce::Component,
                            public juce::AudioProcessorValueTreeState::Listener
{
public:
    
    /*
     we need:
     windowSize
     alignment / spacing info (skipping for now)
     
     APVTS: customknobcontainer is not an audio processor, nor do i want it to be
     ,so in actuality it should be quite obvious that this container should not own the apvts
     OOPS
     
     I guess knob svgs and fonts should then be declared within this object
     
     pointers to audioProcessor DSP parameters manipulated by APVTS
     or give audioProcessor a function to recieve raw parametervalues
     actually by retaining a reference to the pluginProcessor we can just set them like we were before
     
     first focus has to be making knobs line up w daw parameters OK
     
     > a separate class should be created with : parameter IDs as strings + names,
        all float to string functions for each parameter type,
        pointers to audioProcessor atomics
        stuff of this nature
     at some point need to use parameterAttachments,
     I believe that each knob is supposed to extend the parameterAttachment class, and start/end gestures when it is doing mouse listening / changin apvts values
     
     */
    CustomKnobContainer(TinyUIAudioProcessor& processor);
    ~CustomKnobContainer(){};
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
private:
    
    void makeKnobs();
    void updateProcessor();
    
    std::vector<MyVectorKnob> paramKnobs;
    
    
    
    TinyUIAudioProcessor& processor;
    
    

};
