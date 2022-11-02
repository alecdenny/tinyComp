/*
  ==============================================================================

    CompressorValueInfo.h
    Created: 4 Oct 2022 1:29:21pm
    Author:  Alec Denny

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


//this object stores values accessed for:
//  -initialization of rangedAudioParameter (s) in the apvts
//  -initialization of myVectorKnob (s)
//  -referencing apvts values in pluginProcessor and pluginEditor related components
//
//this object stores functions used for:
//  -converting 0to1 values to correct units
//  -supplying functions to display rangedAudioParameters in the host
//  -displaying rangedAudioParameters in UI elements
class CompressorValueInfo
{
public:
    /*
     *  enumeration for display types (dB, ms, percent, ratio)
     *
     *  enumeration for plugin parameters (threshold, ratio, attack, release, mix, in gain, out gain)
     *
     *  function pointers for each display type (ms, dB (unipolar), dB (bipolar), ratio, %)
     *  Optional: function pointers for type conversion so that they may be used by both valuetoString, UI, and processor if that makes sense.
     
     *  for all parameters the apvts should probably store 0 - 1 values, converted for display purposes
     *  the host is asking for 0 to 1 anyways so its a rational conclusion to reach I think
     
     *  min, max, string labels, ID strings
     *
     *  this info ^ is all used by both the UI, APVTS, and to some extent by the audioProcessor (conversion)
     *
     *  I think these are all floats, and automatable but there are of course other attributes that you could store in a class like this for another project
     *
     *  compressor value ranges are therefore decided by this object
     *  i.e. if attack and release range from 0.5ms to 600ms, that will hapen in the toMS conversion
     *
     *  the goal, as usual, is to declare the least amount of things, in the most legible way.
     *  a goal not achieved by the previous sentence.
     */
    
    enum displayType
    {
        DB_UNI,
        DB_BI,
        RATIO,
        MS,
        PERCENT,
        NUMDISPLAYTYPES
    };
    
    enum parameterType
    {
        THRESHOLD,
        RAT,
        ATTACK,
        RELEASE,
        MIX,
        IN_GAIN,
        OUT_GAIN,
        NUMPARAMETERS
    };
    
    float toDB_uni(float x);
    float toDB_bi(float x);
    float toMS(float x);
    float toPercent(float x);
    float toRatio(float x);
    
    juce::String toStringDB_uni(float x, int ssize);
    juce::String toStringDB_bi(float x, int ssize);
    juce::String toStringMS(float x, int ssize);
    juce::String toStringPercent(float x, int ssize);
    juce::String toStringRatio(float x, int ssize);
    
};
