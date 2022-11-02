/*
  ==============================================================================

    CompressorValueInfo.cpp
    Created: 4 Oct 2022 1:29:21pm
    Author:  Alec Denny

  ==============================================================================
*/

#include "CompressorValueInfo.h"

float CompressorValueInfo::toDB_uni(float x)
{
    return 1.0;
}

juce::String CompressorValueInfo::toStringDB_uni(float x, int ssize)
{
    return "fffff";
}

float CompressorValueInfo::toDB_bi(float x)
{
    return 1.0;
}

juce::String CompressorValueInfo::toStringDB_bi(float x, int ssize)
{
    return "fffff";
}

float CompressorValueInfo::toMS(float x)
{
    return 1.0;
}

juce::String CompressorValueInfo::toStringMS(float x, int ssize)
{
    return "fffff";
}

float CompressorValueInfo::toRatio(float x)
{
    return 1.0;
}

juce::String CompressorValueInfo::toStringRatio(float x, int ssize)
{
    return "fffff";
}

float CompressorValueInfo::toPercent(float x)
{
    return 1.0;
}

juce::String CompressorValueInfo::toStringPercent(float x, int ssize)
{
    return "fffff";
}
