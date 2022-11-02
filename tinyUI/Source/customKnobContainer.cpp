/*
  ==============================================================================

    customKnobContainer.cpp
    Created: 3 Oct 2022 12:00:55am
    Author:  Alec Denny

  ==============================================================================
*/

#include "customKnobContainer.h"

CustomKnobContainer::CustomKnobContainer(TinyUIAudioProcessor& processor)
    : 
    processor(processor)
{
    
}

void CustomKnobContainer::parameterChanged(const juce::String& parameterID, float newValue)
{
    
}


